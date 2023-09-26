#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include <ZMPT101B.h>
#include <ACS712XX.h>

// Set storage size
#define EEPROM_SIZE 512
#define SIZE_STORAGE 10

// ########
// # PINS #
// ########
// Sensor temperature Trans1
#define temp_trans1_pin 26
// Sensor temperature Trans2
#define temp_trans2_pin 27
// Sensor temperature TransKeys
#define temp_trans_keys_pin 19
// Power - Invector v220 - On/Off
#define POWER 14
// Coolers - Fan
#define COOLERS 33
// Battery Voltage
#define BATTERY_VOLTAGE_PIN 32
// Battery Current
#define BATTERY_CURRENT_PIN A4
// AC voltage sensor
#define AC_VOLTAGE_PIN 36
#define AC_SENSITIVITY 500.0f
// AC current sensor
#define AC_CURRENT_PIN 39
// Temperatures
OneWire temp_trans1(temp_trans1_pin);
OneWire temp_trans2(temp_trans2_pin);
OneWire temp_trans_keys(temp_trans_keys_pin);

DallasTemperature sensor_trans1(&temp_trans1);
DallasTemperature sensor_trans2(&temp_trans2);
DallasTemperature sensor_trans_keys(&temp_trans_keys);
// AC
ZMPT101B ACSensorVoltage(AC_VOLTAGE_PIN, 50.0);
ACS712XX ACSensorCurrent(ACS712_30A, AC_CURRENT_PIN);

// Default sensors temperature
String trans1 = "0.0";
String trans2 = "0.0";
String trans_keys = "0.0";
String battery_voltage = "0.0";
String battery_current = "0.0";
String transall = "0.0";
String ac_voltage = "0.0";
String ac_current = "0.0";

int checkTrans1 = 0;
int goodTemp;
int badTemp;

bool powerEnabled = true;
bool coolersEnabled = false;

// Voltage settings
float ref_voltage = 3.3;
int adc_value = 0;
float adc_voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;
// WIFI settings
String ap_name = "";
String ap_password = "";
String wifi_name = "";
String wifi_password = "";
String hostname = "Backupower";
// Server settings
AsyncWebServer server(80);
DynamicJsonDocument settings(EEPROM_SIZE);
//

//
// Replaces string variables in HTML
//
String processor(const String &var) {
  return String("0.0");
}
//
int EEPROM_write(int index, String text) {
  for (int i = index; i < text.length() + index; ++i) {

    EEPROM.write(i, text[i - index]);
  }

  EEPROM.write(index + text.length(), 0);
  EEPROM.commit();

  return text.length() + 1;
}
//
String EEPROM_read(int index, int length) {
  String text = "";
  char ch = 1;

  for (int i = index; (i < (index + length)) && ch; ++i) {

    if (ch = EEPROM.read(i)) {

      text.concat(ch);
    }
  }

  return text;
}
//
// Get settings
//
void getSettings() {
  String jsonRead = EEPROM_read(SIZE_STORAGE,
                                EEPROM_read(0, SIZE_STORAGE).toInt());

  DeserializationError error = deserializeJson(settings, jsonRead);

  if (!error) {
    Serial.println("# Load settings #");

    ap_name = settings["ap_name"].as<String>();
    ap_password = settings["ap_password"].as<String>();

    wifi_name = settings["wifi_name"].as<String>();
    wifi_password = settings["wifi_password"].as<String>();

    powerEnabled = settings["power"];
    coolersEnabled = settings["coolers"];
    goodTemp = settings["mintemp"];
    badTemp = settings["maxtemp"];

  } else {
    Serial.println("# Fail settings. Restart... #");

    clearSettings();
    defaultSettings();
    ESP.restart();
  }
}
//
// Default settings
//
void defaultSettings() {

  String json;
  DynamicJsonDocument settings(EEPROM_SIZE);

  settings["ap"] = false;
  settings["ap_name"] = "Backupower";
  settings["ap_password"] = "backupower";
  settings["wifi_name"] = "";
  settings["wifi_password"] = "";
  settings["power"] = true;
  settings["coolers"] = false;
  settings["mintemp"] = 40;
  settings["maxtemp"] = 55;
  settings["battery_cap"] = 0;

  serializeJson(settings, json);

  EEPROM_write(0, (String)EEPROM_write(SIZE_STORAGE, json));

  Serial.println("########################");
  Serial.println("# Set default settings #");
  Serial.println("########################");
}
//
// Update settings
//
void updateSettings(DynamicJsonDocument settings) {

  String json;

  serializeJson(settings, json);

  EEPROM_write(0, (String)EEPROM_write(SIZE_STORAGE, json));

  Serial.println("####################");
  Serial.println("# Updated settings #");
  Serial.println("####################");
}
//
// Clear settings
//
void clearSettings() {
  for (int i = 0; i < 513; i++) {
    EEPROM.write(i, 0);
  }

  EEPROM.commit();

  Serial.println("#########################");
  Serial.println("# Clearing storage done #");
  Serial.println("#########################");
}
//
// Get trans1
//
String getTrans1() {
  sensor_trans1.requestTemperatures();

  float temp = sensor_trans1.getTempCByIndex(0);

  if (temp == -127.00) {
    Serial.println("Error load senseor Trans1!");

    return "0.0";
  }

  return String(temp);
}
//
// Get trans2
//
String getTrans2() {
  sensor_trans2.requestTemperatures();

  float temp = sensor_trans2.getTempCByIndex(0);

  if (temp == -127.00) {
    Serial.println("Error load senseor Trans2!");

    return "0.0";
  }

  return String(temp);
}
//
// Get trans keys
//
String getTransKeys() {

  sensor_trans_keys.requestTemperatures();

  float temp = sensor_trans_keys.getTempCByIndex(0);

  if (temp == -127.00) {
    Serial.println("Error load senseor TransKeys!");

    return "0.0";
  }

  return String(temp);
}
//
// Get battery voltage
//
String getBatteryVoltage() {

  float voltage;

  adc_value = analogRead(BATTERY_VOLTAGE_PIN);
  adc_voltage = (adc_value * ref_voltage) / 4096.0;

  voltage = adc_voltage / (R2 / (R1 + R2));

  if (voltage < 5) {
    return "0.0";
  }

  return String(voltage);
}
//
// Get battery current
//
String getBatteryCurrent() {

  float current = ((ref_voltage / 4096.0) * analogRead(BATTERY_CURRENT_PIN) - 2.5) / 0.022;

  return String(current);
}
//
// Get state
//
bool getState(int pin) {
  if (digitalRead(pin) == LOW) {
    return false;
  }

  return true;
}
//
// Get trans all
//
String getTransAll() {

  float temp = (trans1.toFloat() + trans2.toFloat()) / 2.0;

  return String(temp);
}
//
// Wifi type
//
String wifiType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}
//
// Get AC voltage
//
String getACVoltage() {

  float voltage = ACSensorVoltage.getRmsVoltage();

  if (voltage < 190.0) {
    return "0.0";
  }

  return String(voltage);
}
//
// Get AC current
//
String getACCurrent() {

  float current = ACSensorCurrent.getAC(50);

  return String(current);
}

//
// Route
//
void route() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");

  server.on("/relay", HTTP_GET, [](AsyncWebServerRequest *request) {
    String name = request->getParam("name")->value();
    String state = request->getParam("state")->value();

    if (name == "power") {
      digitalWrite(POWER, state.toInt());

    } else if (name == "coolers") {

      coolersEnabled = !coolersEnabled;
      digitalWrite(COOLERS, coolersEnabled);
    }

    request->send(200, "application/json", "{\"status\": true}");
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<512> json;
    String data;

    json["status"] = true;

    json["power"] = getState(POWER);
    json["coolers"] = getState(COOLERS);

    json["bv"] = battery_voltage.c_str();
    json["bc"] = battery_current.c_str();
    json["acv"] = ac_voltage.c_str();
    json["acc"] = ac_current.c_str();

    json["transall"] = transall.c_str();
    json["trans1"] = trans1.c_str();
    json["trans2"] = trans2.c_str();
    json["transkeys"] = trans_keys.c_str();

    serializeJson(json, data);

    request->send(200, "application/json", data);
  });

  server.on(
    "/settings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<EEPROM_SIZE> obj;
      DeserializationError error = deserializeJson(obj, (const char *)data, len);

      if (!error) {
        String type = obj["type"];

        if (type == "get") {

          String json;

          obj = settings;

          obj["wifi_password"] = "**********";
          obj["status"] = true;

          serializeJson(obj, json);

          request->send(200, "application/json", json);

        } else if (type == "wifi") {

          String data;
          StaticJsonDocument<1024> json;
          JsonArray list = json.createNestedArray("list");

          WiFi.mode(WIFI_AP_STA);
          int n = WiFi.scanNetworks();

          for (int i = 0; i < n; i++) {
            JsonObject item = list.createNestedObject();
            item["name"] = WiFi.SSID(i);
            item["type"] = wifiType(WiFi.encryptionType(i));
          }

          json["status"] = true;

          serializeJson(json, data);

          request->send(200, "application/json", data);

        } else if (type == "update") {

          obj["wifi_name"] = settings["wifi_name"];
          obj["wifi_password"] = settings["wifi_password"];

          if (obj["mintemp"].as<int>() < 30 || obj["mintemp"].as<int>() > 60) {
            obj["mintemp"] = settings["mintemp"];
          }

          if (obj["maxtemp"].as<int>() > 60 || obj["maxtemp"].as<int>() < 30) {
            obj["maxtemp"] = settings["maxtemp"];
          }

          settings = obj;

          updateSettings(settings);

          request->send(200, "application/json", "{\"status\": true}");

        } else if (type == "wifi_update") {

          settings["ap"] = false;
          settings["wifi_name"] = obj["wifi_name"];
          settings["wifi_password"] = obj["wifi_password"];

          updateSettings(settings);

          request->send(200, "application/json", "{\"status\": true}");

          delay(1000);

          ESP.restart();

        } else if (type == "factory_reset") {

          request->send(200, "application/json", "{\"status\": true}");

          clearSettings();
          ESP.restart();
        }
      }

      request->send(200, "application/json", "{\"status\": false}");
    });

  // Start
  server.begin();
}
//
// INIT AP
//
void initAP() {

  Serial.println();

  Serial.println("# Connect to wifi failed. Init AP Wifi #");

  IPAddress ip(192, 168, 1, 1);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_name, ap_password);

  settings["ap"] = true;

  updateSettings(settings);

  Serial.println("########################################");
}
//
// Setup
//
void setup(void) {

  Serial.begin(115200);
  Serial.flush();

  Serial.println();
  Serial.println("#######################");
  Serial.println("# Backupower - v1.0.0 #");
  Serial.println("#######################");
  Serial.println();

  Serial.println("#######################");

  if (!LittleFS.begin()) {

    Serial.println("# Error init webinterface!");

  } else {

    Serial.println("# Init webinterface");
  }

  Serial.println("#######################");

  sensor_trans1.begin();
  sensor_trans2.begin();
  sensor_trans_keys.begin();

  pinMode(POWER, OUTPUT);
  pinMode(COOLERS, OUTPUT);

  if (powerEnabled) {
    digitalWrite(POWER, HIGH);
  }

  // AC init
  ACSensorVoltage.setSensitivity(AC_SENSITIVITY);
  ACSensorCurrent.setSensitivity(66);
  ACSensorCurrent.setOffset(2890.60);

  //
  // INIT interface control
  //
  EEPROM.begin(EEPROM_SIZE);

  getSettings();

  WiFi.setHostname(hostname.c_str());

  if (wifi_name && wifi_password) {

    WiFi.begin(wifi_name, wifi_password);

    Serial.println("# Connecting to WiFi #");

    int counter = 0;
    int timeout = 5;
    bool retry_connect = true;

    while (retry_connect && WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");

      counter++;

      if (counter >= timeout * 5) {
        retry_connect = false;

        initAP();
      }
    }

    if (WiFi.status() == WL_CONNECTED) {

      Serial.println();

      Serial.println("#######################");
      Serial.print("My IP interface => ");
      Serial.println(WiFi.localIP());
      Serial.println("#######################");
      Serial.println();

      if (settings["ap"]) {
        settings["ap"] = false;

        updateSettings(settings);
      }
    }

  } else {
    initAP();
  }

  route();
}
//
// Loop
//
void loop() {

  if ((millis() - lastTime) > timerDelay) {
    trans1 = getTrans1();
    trans2 = getTrans2();

    trans_keys = getTransKeys();

    battery_voltage = getBatteryVoltage();
    battery_current = getBatteryCurrent();
    ac_voltage = getACVoltage();
    ac_current = getACCurrent();

    transall = getTransAll();

    if (trans1.toFloat() == 0) {
      checkTrans1 += 1;
    } else if (checkTrans1 < 3) {
      checkTrans1 = 0;
    }

    lastTime = millis();
  }

  controlCoolers();
}
//
// Control coolers
//
void controlCoolers() {
  if (!coolersEnabled) {
    if (trans1.toFloat() >= badTemp || (trans1.toFloat() == 0 && checkTrans1 >= 3)) {
      digitalWrite(COOLERS, HIGH);
    } else if (trans1.toFloat() <= goodTemp && trans1.toFloat() > 0) {
      digitalWrite(COOLERS, LOW);
    }

  } else {
    digitalWrite(COOLERS, HIGH);
  }
}
