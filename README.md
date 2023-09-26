![header](https://github.com/martinjack/backupower/blob/master/doc/logo.png)

# Backupower - Backup power v220 control inverter

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

![hardware](https://github.com/martinjack/backupower/blob/master/doc/hardware.jpg)

## Build web interface
```ssh
npm i && npm run build
```

## Upload web interface(LittleFS)
```text
https://github.com/lorol/arduino-esp32fs-plugin
```
![upload_webinterface](https://github.com/martinjack/backupower/blob/master/doc/upload_webinterface.png)

## WEB Interface
![interface](https://github.com/martinjack/backupower/blob/master/doc/interface.gif)