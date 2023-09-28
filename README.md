<p align="center">
    <img src="https://github.com/martinjack/backupower/blob/master/doc/logo.png?raw=true"/>
</p>

<p align="center">
    Backup power inverter v220 control on based ESP32
    <br/>
    <br/>
    <img src="https://img.shields.io/github/v/tag/martinjack/backupower?include_prereleases&label=version" alt="version"/>
    <img src="https://img.shields.io/github/license/martinjack/backupower" alt="version"/>
</p>

## Requirements
* Arduino IDE
* OneWire
* DallasTemperature
* WiFi
* ESPAsyncWebServer
* ArduinoJson

## Libraries
* [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [LittleFS](https://github.com/littlefs-project/littlefs)
* ZMPT101B(Robojax)
* [ACS712XX](https://github.com/ElectroRush4u/ACS712)

## Hardware
* Tataliken 3500W 12v
* ESP32D CH9102X
* ACS712 (30A)
* WCS1600 (100A)
* ZMPT101B
* DS18B20
* Voltage Sensor
* Relay module (12V 2 Channel)

![hardware](https://github.com/martinjack/backupower/blob/master/doc/hardware.jpg?raw=true)

## Build web interface
```ssh
npm i && npm run build
```

## Upload web interface(LittleFS)
```text
https://github.com/lorol/arduino-esp32fs-plugin
```
![upload_webinterface](https://github.com/martinjack/backupower/blob/master/doc/upload_webinterface.png?raw=true)

## WEB Interface
![interface](https://github.com/martinjack/backupower/blob/master/doc/interface.gif?raw=true)