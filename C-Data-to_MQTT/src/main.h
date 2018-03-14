
#ifndef _INCL_MAIN
#define _INCL_MAIN


#define DEBUG true
#define DEBUG_LEVEL 3 // 1 = all messages, 2 = important messages, 3 = error messages

// MQTT Config

#define MQTT_ADDRESS     "tcp://localhost:1883"
#define MQTT_CLIENTID    "TemperatureSensors"
#define MQTT_QOS         1
#define MQTT_TIMEOUT     10000L
#define MQTT_USER       ""
#define MQTT_PASSWORD   ""

// SRF Config
#define SRF_SERIAL_PORT "/dev/ttyACM0"
#define SRF_BAUD_RATE B9600

// MQTT threads

#define BATHROOM "/home/bathroom"
#define BEDROOM1 "/home/bedroom1"
#define BEDROOM2 "/home/bedroom2"
#define BEDROOM3 "/home/bedroom3"
#define KITCHEN "/home/kitchen"
#define LIVINGROOM "/home/livingroom"
#define LOFT "/home/loft"
#define OUTDOORS "/home/outdoors"
#define WATER "/home/water"

// 1 Wire Temperature Sensors

#define SOLAR_COLLECTOR "/mnt/1wire/28.5B7DC4030000/temperature"
#define CYLINDER_BASE "/mnt/1wire/28.8C37C4030000/temperature"
#define CYLINDER_TOP "/mnt/1wire/28.8945C4030000/temperature"

#define OW_REFRESH 30000000 // 30 seconds


#endif