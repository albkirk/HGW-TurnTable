# HGW TurnTable - ESP32 Based project


In this project it is created a turnatable to rotate the Fixed Network Homegateway from 0º to 360º on Z axis, in steps of 30º to execute Wi-Fi throuput perform tests. This movment is triggered remotely via cable medium Ethernet or Serial in order to not disturb the 2.4GHz band.



## Supported Features:
	1. Project should run locally, even when LAN (WiFi) newtork is down (it seems obvious, but ...)
	2. Remote upgrade Over-the-Air (OTA) or/and HTTP Update.
	3. Local Flash store configuration (ex.: DeviceName, Location, ...) 
	4. MQTT Publish/Subscribe support
        	4.1.  ALl data is sent under "/<clientID>/<location>/<device name>/*telemetry*/<topic name>" in string format
        	4.2.  ALl data received sent under "/<clientID>/<location>/<device name>/*configure*/<topic name>" in string format
	5. User commands feedback by flashing the "internal" LED and/or adittional active buzzer.
	6. OTA, TELNET, LED, RESET, REBOOT, STORE and DEEPSLEEP functionalities can be remotely enabled/disabled/triggered via defined "true"/"false" MQTT commands.
	7. Battery and Status sent via MQTT (with "will message" defined)
	8. DeepSleep support with configurable On-Time(+ extended time) and Sleep-Time
	9. Remote "Debug" using Telnet (enabled via MQTT)
	10. Long operational live (it will be running 365 days a year, so, it recovers from Wifi or MQTT loss)
	11. Date / Time with NTP sync
	12. Web Page for Initial configuration
	13. Power consumption optimization
	14. Secure communication (https or other methods)

## --Dependencies--
You must install the following libraries (under arduino or Platformio)
	1. **ArduinoJson**�by�Benoit Blanchon 
	2. **PubSubClient**�by�Nick O'Leary 

## Future features!!
	1. WiFi "Air" sniffing for APs, Registered Stations and "Unregistered" Stations (It kinda works....)
