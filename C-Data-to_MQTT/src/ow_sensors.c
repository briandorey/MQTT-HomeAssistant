#include <stdbool.h>
//#include <stdint.h>  // C99 fixed data types
#include <stdio.h>  // Standard input/output definitions
#include <stdlib.h> // C standard library
#include <unistd.h> // UNIX standard function definitions
#include "ow_sensors.h"
#include "main.h"
#include "mqtt.h"
#include "utils.h"

char ow_temperature[50];
FILE *fp;

void read_ow_sensor(char *filename, char *value)
{
    //
    // Read the value from the specified file
    //

    fp = fopen(filename, "r"); // open file for input

    if (fp) // If no error occurred read the data from the file
    {
        fgets(value, sizeof(value), fp);
        trace(value, 1);
    }
    else
    {
        trace("An error occurred while opening the file.\n", 3);
    }
    fclose(fp); // close the file
}

void start_ow_sensors(void)
{

    //
    // Read the values from the ow sensors at the preset interval
    //

    while (1)
    {
        // solar collector
        read_ow_sensor(SOLAR_COLLECTOR, ow_temperature);
        mqtt_send_message(WATER, "solarcollector", ow_temperature);

        // cylinder top
        read_ow_sensor(CYLINDER_TOP, ow_temperature);
        mqtt_send_message(WATER, "cylindertop", ow_temperature);

        // cylinder base
        read_ow_sensor(CYLINDER_BASE, ow_temperature);
        mqtt_send_message(WATER, "cylinderbase", ow_temperature);

        usleep(OW_REFRESH);
    }
}
