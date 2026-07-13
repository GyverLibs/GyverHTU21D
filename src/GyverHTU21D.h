/*
    Лёгкая библиотека для датчика температуры и влажности HTU21D
    Документация:
    GitHub: https://github.com/GyverLibs/GyverHTU21D
    Возможности:
    - Асинхронное чтение
    - Блокирующее чтение
    - Установка разрешения
    - Проверка напряжения
    - Управление нагревателем

    Nich1con, AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - совместимость esp8266
*/

#pragma once

#include <Arduino.h>
#include <Wire.h>

/* Команды управления HTU21D */
#define HTU21D_IIC_ADDR 0x40
#define HTU21D_RESET_CMD 0xFE
#define HTU21D_CFG_READ_CMD 0xE7
#define HTU21D_CFG_WRITE_CMD 0xE6
#define HTU21D_TRIG_T_CMD 0xF3
#define HTU21D_TRIG_H_CMD 0xF5
#define HTU21D_TRIG_T_HOLD_CMD 0xE3
#define HTU21D_TRIG_H_HOLD_CMD 0xE5

/* Варианты разрешения датчика */
enum HTURes_t : uint8_t {
    HTU21D_RES_HIGH = 0b00000000,    // RH = 12Bit, T = 14Bit
    HTU21D_RES_MEDIUM = 0b10000001,  // RH = 11Bit, T = 11Bit
    HTU21D_RES_LOW = 0b00000001,     // RH = 8Bit, T = 12Bit
};

class GyverHTU21D {
   public:
    // запуск и проверка наличия датчика
    bool begin() {
        Wire.begin();
        _hum = _tmp = NAN;  // сброс
        return reset();
    }

    // сброс датчика
    bool reset() {
        return _request(HTU21D_RESET_CMD);
    }

    // Установка разрешения
    void setResolution(HTURes_t res) {
        uint8_t reg = _readCfgReg() & 0b01111110;
        _writeCfgReg(reg | res);
    }

    // Включение / выключение подогрева
    void setHeater(bool state) {
        uint8_t reg = _readCfgReg() & 0b11111011;
        _writeCfgReg(reg | (state ? 0b100 : 0b000));
    }

    // Состояние питания
    bool powerGood() {
        return !(_readCfgReg() & 0b01000000);
    }

    // Запросить температуру
    bool requestTemperature() {
        return _request(HTU21D_TRIG_T_CMD);
    }

    // Запросить влажность
    bool requestHumidity() {
        return _request(HTU21D_TRIG_H_CMD);
    }

    // Прочитать температуру
    bool readTemperature() {
        uint16_t data = _readValue();
        if (data == 0xffff) return false;

        _tmp = 0.002681f * (data & 0xfffc) - 46.85f;
        return true;
    }

    // Прочитать влажность
    bool readHumidity() {
        uint16_t data = _readValue();
        if (data == 0xffff) return false;

        _hum = ((0.001907 * (data & 0xfffc)) - 6);
        _hum = constrain(_hum, 0, 100);
        return true;
    }

    // Получить температуру
    float getTemperature() {
        return _tmp;
    }

    // Получить влажность
    float getHumidity() {
        return _hum;
    }

    // прочитать температуру (блокирующая функция)
    float getTemperatureWait(uint16_t tout = 100) {
        _request(HTU21D_TRIG_T_HOLD_CMD);
        if (_waitSCL(tout)) readTemperature();
        return _tmp;
    }

    // прочитать флажность (блокирующая функция)
    float getHumidityWait(uint16_t tout = 100) {
        _request(HTU21D_TRIG_H_HOLD_CMD);
        if (_waitSCL(tout)) readHumidity();
        return _hum;
    }

    // асинхронный тикер чтения, true если готово. Можно указать свой период
    bool readTick(uint16_t prd = 100) {
        if (uint16_t(uint16_t(millis()) - _tmr) >= prd) {
            _tmr = millis();
            if (_reqTemp) {
                readTemperature();
                requestHumidity();
            } else {
                readHumidity();
                requestTemperature();
            }
            _reqTemp = !_reqTemp;
            return (!isnan(_hum) && !isnan(_tmp));  // пропуск непрочитанных
        }
        return false;
    }

   private:
    float _hum = NAN, _tmp = NAN;
    uint16_t _tmr = 0;
    bool _reqTemp = 0;

    uint16_t _readValue() {
        if (Wire.requestFrom(HTU21D_IIC_ADDR, 3) != 3) return 0xffff;
        uint16_t data = (Wire.read() << 8) | Wire.read();
        if (_checkCRC(data) != Wire.read()) return 0xffff;
        return data;
    }

    // запрос по команде
    bool _request(uint8_t cmd) {
        Wire.beginTransmission(HTU21D_IIC_ADDR);  // Начинаем передачу
        Wire.write(cmd);                          // Отправляем команду
        return (!Wire.endTransmission());
    }

    // Запись регистра конфигурации
    void _writeCfgReg(uint8_t reg) {
        Wire.beginTransmission(HTU21D_IIC_ADDR);
        Wire.write(HTU21D_CFG_WRITE_CMD);
        Wire.write(reg);
        Wire.endTransmission();
    }

    // Чтение регистра конфигурации
    uint8_t _readCfgReg() {
        Wire.beginTransmission(HTU21D_IIC_ADDR);
        Wire.write(HTU21D_CFG_READ_CMD);
        if (Wire.endTransmission()) return 0;
        Wire.requestFrom(HTU21D_IIC_ADDR, 1);
        return Wire.read();
    }

    bool _waitSCL(uint16_t tout) {
        uint32_t _tmr = millis();
        while (!digitalRead(SCL)) {
            if (millis() - _tmr >= tout) return false;
            yield();
        }
        return true;
    }

    // Расчет CRC по полиному HTU21D
    uint8_t _checkCRC(uint16_t data) {
        for (uint8_t i = 0; i < 16; i++) {
            if (data & 0x8000) data = (data << 1) ^ 0x13100;
            else data <<= 1;
        }
        return (data >> 8);
    }
};
