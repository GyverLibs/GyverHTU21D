This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverHTU21D.svg?color=brightgreen)](https://github.com/GyverLibs/GyverHTU21D/releases/latest/download/GyverHTU21D.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverHTU21D.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverHTU21D)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverHTU21D?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverHTU21D
Lightweight library for HTU21D temperature and humidity sensor
- Asynchronous reading
- Blocking reading
- Installation of authorization
- Voltage check
- Heater control

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Installation](#install)
- [Initialization](#init)
- [Use of use](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **GyverHTU21D** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/GyverHTU21D/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="init"></a>
## Initialization
```cpp
GyverHTU21D htu;
```

<a id="usage"></a>
## Use of use
```cpp
bool begin();                       // launch and check for sensor availability
void setHeater(bool state);         // Turning on/off heating
bool powerGood();                   // Nutrition status
void setResolution(uint8_t mode);   // Installation of authorization

// Permission:
HTU21D_RES_HIGH     // High resolution (see table below) - by default
HTU21D_RES_MEDIUM   // Average resolution (see table below)
HTU21D_RES_LOW      // Low resolution (see table below)

bool readTick(uint16_t prd = 100);  // Asynchronous reading ticker, true if ready. You can specify your period. Shut up. 100 ms.

bool requestTemperature();          // Temperature (true if successful)
bool requestHumidity();             // Ask for humidity (true if successful)

bool readTemperature();             // Read the temperature (true if successful)
bool readHumidity();                // Read the humidity (true if successful)

float getTemperature();             // Get the temperature.
float getHumidity();                // Getting humidity

float getTemperatureWait();         // Read the temperature (blocking function)
float getHumidityWait();            // read the flag (blocking function)
```

### Permission and conversion time
Mode | Permission, pace. | Permission, wet. Reading time, pace. Reading time, wet. |
----------------------|-------------------|-------------------|-------------------|-----------------------|
**HTU21D_RES_LOW**    |12 bit             |8 bit              |13 ms              |3 ms                   |
**HTU21D_RES_MEDIUM** |11 bit             |11 bit             |4 ms               |8 ms                   |
**HTU21D_RES_HIGH**   |14 bit             |12 bit             |50 ms              |16 ms                  |

### Blocking reading
Values can simply be obtained from`getTemperatureWait()`and`getHumidityWait()`. Functions are performed by the time specified in the table above.

### Asynchronous reading (by hand)
Asynchronous reading mode allows you not to wait for the conversion and return to the survey later, without blocking the program.
In this mode, you need to request a read, wait the time according to the resolution, and then read the value. Request.
You can only measure one thing at a time: ask for temperature, read the temperature. We asked for humidity.
read the humidity. You can't ask for both at once!
- Request measurement: call`requestTemperature()`or`readTemperature()`. Come back.`true`upon successful request.
- Read the measurement:`readTemperature()`or`readHumidity()`. Come back.`true`with successful reading.
- Get the result:`getTemperature()`or`getHumidity()`.

### Asynchronous reading (automatically)
In this mode, the library itself polls and reads the sensor on the built-in timer, it remains only to pick up the result.
It's just in the loop.`readTick()`or`readTick(свой период в мс)`. If the function returned`true`- the data has been updated.

<a id="example"></a>
## Example
### Blocking reading
```cpp
#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // Execution time depends on permission, see dock
  float temp = htu.getTemperatureWait();
  float hum = htu.getHumidityWait();
  
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Hum: ");
  Serial.print(hum);
  Serial.println(" %");
  
  Serial.println();
  delay(1000);
}
```

### Manual asynchronous reading
```cpp
#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // Reading the temperature.
  htu.requestTemperature();                 // Requesting conversion
  delay(100);                               // Waiting for the end (see dock)
  if (htu.readTemperature()) {              // Read the temperature from the sensor and check
    Serial.print("Temp: ");
    Serial.print(htu.getTemperature());     // If successful, bring the temperature to the port.
    Serial.println(" *C");
  }

  // Reading humidity
  htu.requestHumidity();                    // Requesting conversion
  delay(100);                               // Waiting for the end (see dock)
  if (htu.readHumidity()) {                 // Read the humidity from the sensor and check
    Serial.print("Hum: ");
    Serial.print(htu.getHumidity());        // If successful, we remove humidity to the port.
    Serial.println(" %");
  }

  Serial.println();
  delay(1000);
}
```

### Automatic asynchronous reading
```cpp
#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // function polls the sensor by its timer
  if (htu.readTick()) {
    // You can pick up values here or elsewhere in the program.
    Serial.println(htu.getTemperature());
    Serial.println(htu.getHumidity());
    Serial.println();
  }
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - compatibility of esp8266

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
