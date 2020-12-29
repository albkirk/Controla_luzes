// Function to insert customized MQTT Configuration actions

void  mqtt_custom(String command, String cmd_value) {

  if ( command == "Info" && bool(cmd_value.toInt())) {
    //status_report();
    if (BattPowered) mqtt_publish(mqtt_pathtele, "Battery", String(getBattLevel(),0));
    mqtt_publish(mqtt_pathtele, "Switch", String(SWITCH));
    mqtt_publish(mqtt_pathtele, "Switch2", String(SWITCH2));
    mqtt_publish(mqtt_pathtele, "Timer", String(TIMER));
    mqtt_publish(mqtt_pathtele, "Timer2", String(TIMER));
    mqtt_publish(mqtt_pathtele, "NTP_Sync", String(NTP_Sync));
    mqtt_publish(mqtt_pathtele, "DateTime", String(curDateTime()));
    hassio_attributes();
    //ambient_data();
  }
  if ( command == "Config" && bool(cmd_value.toInt())) {
    mqtt_publish(mqtt_pathtele, "OTA", String(config.OTA));
    mqtt_publish(mqtt_pathtele, "TELNET", String(config.TELNET));
    mqtt_publish(mqtt_pathtele, "WEB", String(config.WEB));
    mqtt_publish(mqtt_pathtele, "Interval", String(config.SLEEPTime));   
    mqtt_publish(mqtt_pathtele, "Temp_Corr", String(config.Temp_Corr));
    mqtt_publish(mqtt_pathtele, "LDO_Corr", String(config.LDO_Corr));
  }

  if ( command == "Exterior") {
      if ( SWITCH_Last == bool(cmd_value.toInt()) ) mqtt_publish(mqtt_pathtele, "Exterior", String(SWITCH));
      else SWITCH = bool(cmd_value.toInt());
  }
  if ( command == "Interior") {
      if ( SWITCH2_Last == bool(cmd_value.toInt()) ) mqtt_publish(mqtt_pathtele, "Interior", String(SWITCH2));
      else SWITCH2 = bool(cmd_value.toInt());
  }
  if ( command == "Timer2") TIMER2 = (ulong)abs(atol(cmd_value.c_str()));
}
