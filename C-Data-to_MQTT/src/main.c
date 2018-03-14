
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"
#include "srf_wireless.h"
#include "ow_sensors.h"
#include "mqtt.h"

void *srf_thread_worker(void *argument)
{ // SRF Sensor Thread
    start_srf_sensors();
    return NULL;
}

void *ow_thread_worker(void *argument)
{ // 1 Wire Sensor Thread
    start_ow_sensors();
    return NULL;
}

void *mqtt_thread_worker(void *argument)
{ // MQTT Processing Thread
    start_mqtt();
    return NULL;
}

int main(int argc, char *argv[])
{
    // initialise mqtt client

    pthread_t srf_sensor_thread, ow_sensor_thread, mqtt_sensor_thread;
    int thread_return;

    // MQTT sensor thread
    thread_return = pthread_create(&mqtt_sensor_thread, NULL, mqtt_thread_worker, NULL);
    if (thread_return)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", thread_return);
        exit(EXIT_FAILURE);
    }

    // SRF sensor thread
    thread_return = pthread_create(&srf_sensor_thread, NULL, srf_thread_worker, NULL);
    if (thread_return)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", thread_return);
        exit(EXIT_FAILURE);
    }

    // OW sensor thread
    thread_return = pthread_create(&ow_sensor_thread, NULL, ow_thread_worker, NULL);
    if (thread_return)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", thread_return);
        exit(EXIT_FAILURE);
    }

    // Keyboard listener - exit on q key
    char key;

    while (1)
    {

        key = getchar();
        if (key == 'Q' || key == 'q')
        {
            trace("Exiting", 3);
            destroy_mqtt();
            exit(EXIT_SUCCESS);
        }
        usleep(100000);
    }

    exit(EXIT_SUCCESS);
}