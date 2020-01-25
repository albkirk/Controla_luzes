// Function to insert customized MQTT Configuration actions

void  mqtt_custom(String reqtopic, String reqparam, StaticJsonDocument<200> data) {
  if ( reqparam == "Info" && bool(data["value"])) {
    //status_report();
    mqtt_publish(mqtt_pathtele(), "Switch", String(SWITCH));
    mqtt_publish(mqtt_pathtele(), "Switch2", String(SWITCH2));
    mqtt_publish(mqtt_pathtele(), "Timer", String(timer_current));
  }
  if ( reqparam == "Config" && bool(data["value"])) {
    mqtt_publish(mqtt_pathtele(), "OTA", String(config.OTA));
    mqtt_publish(mqtt_pathtele(), "TELNET", String(config.TELNET));
  }
  if ( reqparam == "Switch2") SWITCH2 = bool(data["value"]);
  if ( reqparam == "Switch" && SWITCH_Last == bool(data["value"])) mqtt_publish(mqtt_pathtele(), "Switch", String(SWITCH));;
  if ( reqparam == "Switch2" && SWITCH2_Last == bool(data["value"])) mqtt_publish(mqtt_pathtele(), "Switch2", String(SWITCH2));;
}
