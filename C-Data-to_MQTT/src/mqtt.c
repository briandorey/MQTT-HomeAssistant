#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <unistd.h>
#include "mqtt.h"
#include "utils.h"
#include "main.h"

// mqtt objects

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int rc;

// message stack for storing messages while they wait to be sent to the mqtt broker

int MAXSIZE = 20; // message stack size

struct Message
{
    char topic[100];
    char message[100];
};

struct Message message_stack[20];
int top = -1;

int isempty()
{
    // check if the stack is empty
    if (top == -1)
        return 1;
    else
        return 0;
}

int isfull()
{
    // check if the stack is full
    if (top == MAXSIZE)
        return 1;
    else
        return 0;
}

void start_mqtt(void)
{
    //
    //  Configure and connect to the MQTT broker
    //

    MQTTClient_create(&client, MQTT_ADDRESS, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.username = MQTT_USER;
    conn_opts.password = MQTT_PASSWORD;
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = 0;

    // message loop
    while (1)
    {
        // check if the message stack contains any messages
        if (!isempty())
        {
            // send message

            if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
            {
                trace("Failed to connect to mqtt broker\n", 3);
                exit(-1);
            }

            pubmsg.payload = &message_stack[top].message;
            pubmsg.payloadlen = strlen(message_stack[top].message);

            MQTTClient_publishMessage(client, message_stack[top].topic, &pubmsg, &token);
            trace("Waiting for mqtt publication\n", 1);
            rc = MQTTClient_waitForCompletion(client, token, MQTT_TIMEOUT);
            trace("MQTT message delivered\n", 1);
            MQTTClient_disconnect(client, 10000);
            top = top - 1;
        }

        usleep(10000); // sleep 10ms between messages
    }
}

void destroy_mqtt(void)
{
    // Destroy the MQTT client
    MQTTClient_destroy(&client);
}

void mqtt_send_message(char *topic, char *child, char *message)
{
    //
    // Add a message onto the MQTT message stack
    //

    // build topic string
    char target[strlen(topic) + strlen(child) + 1];
    strcpy(target, topic);
    strcat(target, "/");
    strcat(target, child);

    // push the message onto the stack
    if (!isfull())
    {
        top = top + 1;
        strcpy(message_stack[top].topic, target);
        strcpy(message_stack[top].message, message);
    }
    else
    {
        trace("Could not insert data, Stack is full.\n", 3);
    }

    return;
}