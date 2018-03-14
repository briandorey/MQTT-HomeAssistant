# MQTT-HomeAssistant

This repo contains config files for Home Assistant https://home-assistant.io/ to access an MQTT broker to report data from a home logging system. 

The Home Assistant config also displays status data from six Nest Smoke alarms and also a Nest Thermostat.

The /Python-Uploader directory contains a Python script which subscribes to the MQTT server and uploads to a remote server at timed intervals.

The /C-Data-to_MQTT directory contains a Linux C application which logs data from a UART based wireless interface and also OWFS temperature sensors which are published to the MQTT broker.

More details about this project are on my blog at https://www.briandorey.com/post/mosquitto-mqtt-and-home-assistant-data-logging