/*
 * * Main File to trigger all C code using "arduino" style.
 * * It contains all necessary function calls on setup and loop functions
 * * HOW TO USE THIS TEMPLATE:
 * * -- Adjust the parameter below to your project.
 * *    Parameters on struct "config" will be store on memory.
 * *    Don't forget to customize the Mem read and write actions on "lib/project/custostore.h"
 * * -- Use the "// **** Normal code ..." zones to add you own definition, functions, setup and loop code
 * * -- You can also add you own MQTT actions on "lib/project/customqtt.h"
 * * -- Suggest to use "lib/project/" to add your own h files
 */

// Libraries to INCLUDE
#include <ESP8266WiFi.h>

//#include <secure_credentials.h>
#include <storage.h>
#include <hw8266.h>
#include <mywifi.h>
#include <telnet.h>
#include <ntp.h>
#include <mqtt.h>
#include <hassio.h>
#include <ota.h>
//#include <web.h>
#include <global.h>
#include <project.h>
#include <mqttactions.h>                                // Added later because functions from project are called here.


void setup() {
// Starting with WiFi interface shutdown in order to save energy
    WiFi.mode(WIFI_SHUTDOWN);

  // Start Serial interface
      Serial.begin(74880);                  // This odd baud speed will shows ESP8266 boot diagnostics too.
      //Serial.begin(115200);               // For faster communication use 115200

      Serial.println("");
      Serial.println("Hello World!");
      Serial.println("My ID is " + ChipID + " and I'm running version " + SWVer);
      Serial.println("Reset reason: " + ESPWakeUpReason());

  // Start Storage service and read stored configuration
      storage_setup();

  // Start ESP specific features, such as: Serial Number, ESP_LED, internal ADC, ...
      hw_setup();

  //  Project HW initialization, such as: GPIO config, Sensors, Actuators, ...  
      project_hw();

  // Start WiFi service (Station or/and as Access Point)
      wifi_setup();

  // Start TELNET service
      if (config.TELNET) telnet_setup();

  // Start NTP service
      ntp_setup();

 // Start MQTT service
      mqtt_setup();

  // Start OTA service
      if (config.OTA) ota_setup();
      ota_http_upg();

  // Start ESP Web Service
      //if (config.WEB) web_setup();

  // **** Project SETUP Sketch code here...
      project_setup();


 /*
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size: %u bytes\n\n", realSize);

  Serial.printf("Flash ide  size: %u bytes\n", ideSize);
  Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  if (ideSize != realSize) {
    Serial.println("Flash Chip configuration wrong!\n");
  } else {
    Serial.println("Flash Chip configuration ok.\n");
  }
*/


  // Last bit of code before leave setup
      ONTime_Offset = millis() + 100;       //  100ms after finishing the SETUP function it starts the "ONTime" countdown.
                                            //  it should be good enough to receive the MQTT "ExtendONTime" msg

} // end of setup()


void loop() {
  // allow background process to run.
      yield();

  // Hardware handling, namely the ESP_LED
      hw_loop();

  // WiFi handling
      wifi_loop();

  // TELNET handling
      if (config.TELNET) telnet_loop();

  // NTP handling
      ntp_loop();

  // MQTT handling
      mqtt_loop();

  // OTA request handling
      if (config.OTA) ota_loop();

  // ESP Web Server requests handling
      //if (config.WEB) web_loop();

  // **** Project LOOP Sketch code here ...
      project_loop();

  // Global loops handling
      deepsleep_loop();
      if (BattPowered && ((millis() - 3500) % 60000 < 5)) Batt_OK_check();    // If Batt LOW, it will DeepSleep forever!

}  // end of loop()
