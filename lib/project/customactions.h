// Function to insert customized MQTT Configuration actions
void custom_mqtt(String command, String cmd_value) {
  if ( command == "Info" && bool(cmd_value.toInt()) ) {
    //status_report();
    if (BattPowered) mqtt_publish(mqtt_pathtele, "Battery", String(getBattLevel(),0));
    mqtt_publish(mqtt_pathtele, "DateTime", String(curDateTime()));
    mqtt_publish(mqtt_pathtele, "NTP_Sync", String(NTP_Sync));
    if (BattPowered) { Serial.printf("Power: BATT  -  Level: %.0f\t", getBattLevel()); }
    else { telnet_print("Power: MAINS\t", true); }
    telnet_print("Current Date/Time: " + curDateTime(), true);
    Serial.printf("\t NTP Sync: %d\n", NTP_Sync);
    hassio_attributes();
    //ambient_data();
    #ifdef ESP8266
        telnet_println("Flash: " + Flash_Size() + "  -  CPU Clock: " + String(CPU_Clock()) + " MHz  -  WiFi State: " + WIFI_state_Name[WIFI_state] + " - Phy Mode: " + WIFI_PHY_Mode_Name[WiFi.getPhyMode()] + "  -  MQTT State: " + MQTT_state_string(), true);
    #else
        telnet_println("Flash: " + Flash_Size() + "  -  CPU Clock: " + String(CPU_Clock()) + " MHz  -  WiFi State: " + WIFI_state_Name[WIFI_state] + "  -  MQTT State: " + MQTT_state_string(), true);
    #endif
    #ifdef Modem_WEB_TELNET
        telnet_println("Modem State: " + Modem_state_Name[Modem_state] + "\t REG State: " + RegStatus_string(modem.getRegistrationStatus()));
    #endif
    telnet_println("Angle: " + String(Angle), true);
  }
  if ( command == "Config" && bool(cmd_value.toInt()) ) {
    mqtt_publish(mqtt_pathtele, "OTA", String(config.OTA));
    mqtt_publish(mqtt_pathtele, "TELNET", String(config.TELNET));
    mqtt_publish(mqtt_pathtele, "WEB", String(config.WEB));
    mqtt_publish(mqtt_pathtele, "Interval", String(config.SLEEPTime));   
    mqtt_publish(mqtt_pathtele, "Temp_Corr", String(config.Temp_Corr));
    mqtt_publish(mqtt_pathtele, "LDO_Corr", String(config.LDO_Corr));
    config_backup();
    storage_print();
    telnet_println("Velocity: " + String(config.Velocity) + "\tMINAngle: " + String(config.MINAngle) + "\tMAXAngle: " + String(config.MAXAngle), true);
  }
  
  if ( command == "CPU_Boost" ) { CPU_Boost(bool(cmd_value.toInt())); delay(10); telnet_println("CPU Clock: " + String(CPU_Clock()) + " MHz"); }
  if ( command == "CPU_Clock" && bool(cmd_value.toInt()) ) telnet_println("CPU Clock: " + String(CPU_Clock()) + " MHz");
  #ifdef ESP8266
      if ( command == "PHY_Mode" ) {WiFi.setPhyMode((WiFiPhyMode_t)cmd_value.toInt()); wifi_connect(); }
      // WIFI_PHY_MODE_11B = 1, WIFI_PHY_MODE_11G = 2, WIFI_PHY_MODE_11N = 3
  #endif
  //if ( command == "send_Telemetry" && bool(cmd_value.toInt())) { gps_update(); print_gps_data(); send_Telemetry(); }

  if ( command == "BattPowered" ) BattPowered = bool(cmd_value.toInt());
  
  if ( command == "BckpRstr" ) {
      if (cmd_value == "") telnet_println("Restore data is empty");
      else {
          DeserializationError error = deserializeJson(config_doc, cmd_value); //Deserialize string to JSON data
          if (error) {telnet_print("JSON DeSerialization failure: "); telnet_println(error.f_str());}
          else {

                strcpy(config.Location, config_doc["Location"]);
/*
                config.LOWER_LEVEL =    config_doc["LOWER_Pos"];
                config.UPPER_LEVEL =    config_doc["UPPER_Pos"];
*/
                storage_write();
                hassio_attributes();
                bckup_rstr_flag = false;
          }
      }
  }
  if ( command == "MoveTo" ) Angle = cmd_value.toInt();
  if ( command == "Velocity" && cmd_value.toFloat() > 0 && cmd_value.toFloat() <= MAXVelocity ) {
        config.Velocity = cmd_value.toFloat();
        stepper.setMaxSpeed(config.Velocity);
        storage_write();
        mqtt_publish(mqtt_pathtele, "Velocity", String(config.Velocity));
  }
  if ( command == "MINAngle" && cmd_value.toInt() > -360 && cmd_value.toInt() < 0 ) {
        config.MINAngle = cmd_value.toInt();
        storage_write();
        mqtt_publish(mqtt_pathtele, "MINAngle", String(config.MINAngle));
  }
  if ( command == "MAXAngle" && cmd_value.toInt() > 0 && cmd_value.toInt() < 360 ) {
        config.MAXAngle = cmd_value.toInt();
        storage_write();
        mqtt_publish(mqtt_pathtele, "MAXAngle", String(config.MAXAngle));
  }
  if ( command == "Zero" && bool(cmd_value.toInt()) ) ReturnToZero();
  if ( command == "Angle" && bool(cmd_value.toInt()) ) print_angle();
}

void custom_update(){
    yield();
    //mqtt_dump_data(mqtt_pathtele, "Telemetry");
}



// TELNET related actions


void parse_at_command(String msg) {
    #ifdef Modem_WEB_TELNET
        msg.remove(0, 2);       // to remove "AT"
        modem.sendAT(msg);
        String res = "";
        unsigned long timeout = millis();
        while ( (millis() - timeout < 1000) && modem.stream.available() < 0 ) yield();
        timeout = millis();
        while ((millis() - timeout < 1000)) {
            if (modem.stream.available() > 0) {
                res = modem.stream.readStringUntil('\n');
                telnet_println(res);
                timeout = millis();
            }
        }
    #endif
    yield();
}
