#ifndef _INCL_MQTT
#define _INCL_MQTT


void start_mqtt(void);
void destroy_mqtt(void);
void mqtt_send_message(char *topic, char*child, char *message);

#endif