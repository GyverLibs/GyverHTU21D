This is an automatic translation, may be incorrect in some places. See sources and examples!

# GyverHTU21D
Lightweight library for temperature and humidity sensor HTU21D
- Asynchronous reading
- Blocking read
- Permission setting
- Voltage test
- Heater control

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **GyverHTU21D** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/GyverHTU21D/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
GyverHTU21Dhtu;
```

<aid="usage"></a>
## Usage
```cpp
boolbegin(); // start and check for the presence of the sensor
void setHeater(bool state); // Turn on / off the heating
bool powerGood(); // Power state
void setResolution(uint8_t mode); // Set permission

// Permission:
HTU21D_RES_HIGH // High resolution (see table below) - default
HTU21D_RES_MEDIUM // Medium resolution (see table below)
HTU21D_RES_LOW // Low resolution (see table below)

bool readTick(uint16_t prd = 100); // asynchronous read ticker, true if ready. You can specify your period. By default 100ms

bool requestTemperature(); // Request temperature (true if successful)
bool requestHumidity(); // Request humidity (true if successful)

bool readTemperature(); // Read temperature (true if successful)
bool readHumidity(); // Read humidity (true if successful)

float getTemperature(); // Get temperature
float getHumidity(); // Get humidity

float getTemperatureWait(); // read temperature (blocking function)
float getHumidityWait(); // read flag (blocking function)
```

### Resolution and conversion time
Mode | Resolution, temp. | Permission, wet. | | Reading time, temp. | Reading time, humidity. | |
----------------------|-------------------|------- ------------|----------------------------------|----------------- ------|
**HTU21D_RES_LOW** |12 bit |8 bit |13 ms |3 ms |
**HTU21D_RES_MEDIUM** |11 bit |11 bit |4 ms |8 ms |
**HTU21D_RES_HIGH** |14 bit |12 bit |50 ms |16 ms |

### Blocking read
The values ​​can simply be obtained from `getTemperatureWait()` and `getHumidityWait()`. The functions run for the time indicated in the table above.

### Asyncreading (manually)
The asynchronous read mode allows you not to wait for the conversion and return to the poll later without blocking the program.
In this mode, you need to request a read, wait a time according to the permission, and then read the value. Request
only one measured value at a time is possible: requested temperature - read temperature. Requested humidity -
read humidity. You can't ask for both at the same time!
- Request a measurement: call `requestTemperature()` or `readTemperature()`. Will return `true` on successful request.
- Read measurement: `readTemperature()` or `readHumidity()`. Returns `true` on successful read.
- Get result: `getTemperature()` or `getHumidity()`.

### Asynchronous read (automatically)
In this mode, the library itself interrogates and reads the sensor using the built-in timer, all that remains is to pick up the result.
Just call `readTick()` or `readTick(custom period in ms)` in the loop. If the function returns `true`, the data has been updated.

<a id="example"></a>
## Example
### Blocking read
```cpp
#include <GyverHTU21D.h>
GyverHTU21Dhtu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // execution time depends on resolution, see doc
  float temp = htu.getTemperatureWait();
  float hum = htu.getHumidityWait();
  
  Serial.print("Temp: ");
  Serial print(temp);
  Serial.println("*C");
  
  Serial.print("Hum: ");
  Serial print(hum);
  Serial.println("%");
  
  Serial.println();
  delay(1000);
}
```

### Manual asynchronous reading
```cpp
#include <GyverHTU21D.h>
GyverHTU21Dhtu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // Read temperature
  htu.requestTemperature(); // Request a transformation
  delay(100); // Wait for completion (see doc)
  if (htu.readTemperature()) { // Read the temperature from the sensor and check
    Serial.print("Temp: ");
    Serial.print(htu.getTemperature()); // If successful, output the temperature to the port
    Serial.println("*C");
  }

  // Read humidity
  htu.requestHumidity(); // Request a transformation
  delay(100); // Wait for completion (see doc)
  if (htu.readHumidity()) { // Read the humidity from the sensor and check
    Serial.print("Hum: ");
    Serial.print(htu.getHumidity()); // If successful, output the humidity to the port
    Serial.println("%");
  }

  Serial.println();
  delay(1000);
}
```

### Automatic asynchronous reading
```cpp
#include <GyverHTU21D.h>
GyverHTU21Dhtu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // the function polls the sensor according to its timer
  if (htu.readTick()) {
    // you can take values ​​here or elsewhere in the program
    Serial.println(htu.getTemperature());
    Serial.println(htu.getHumidity());
    Serial.println();
  }
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - esp8266 compatibility

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!