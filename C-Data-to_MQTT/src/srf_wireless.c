#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <termios.h>
#include <unistd.h>
#include "srf_wireless.h"
#include "utils.h"
#include "mqtt.h"
#include "main.h"

// serial variables
const int bytes_expected = 11;
int bytes_available = 0;
char data_received = 0;
char data_in[16]; // buffer for serial input

int fd; // File descriptor for the port

int open_port(void)
{
    //
    // Open usb-serial port for reading & writing
    //
    fd = open(SRF_SERIAL_PORT, O_RDWR | O_NOCTTY);

    if (fd == -1)
    {
        trace("open_port: Unable to open SRF serial port\n", 3);
        exit(EXIT_FAILURE);
    }

    return fd;
}

void clear_buffer()
{
    //
    // flush the serial  port buffer
    //
    int tmp_size = 0;
    ioctl(fd, FIONREAD, &tmp_size);
    read(fd, &data_in, tmp_size);

    for (int i = 0; i < 16; i++)
    {
        data_in[i] = 0;
    }
}

void parse_sensor(char *target_room, bool readtemp)
{
    //
    // Parse the temperature sensor data
    //

    if (data_in[3] == 'T' && data_in[4] == 'E' && readtemp == true)
    { // temperature
        char value[6];
        str_find_num(data_in, 12, value);
        mqtt_send_message(target_room, "temperature", value);
    }
    else if (data_in[3] == 'B' && data_in[4] == 'A' && data_in[5] == 'T')
    { // battery

        if (data_in[7] == 'L' && data_in[8] == 'O')
        { // Low battery
            mqtt_send_message(target_room, "batterylow", "true");
        }
        else
        {
            char value[6];
            str_find_num(data_in, 12, value);
            mqtt_send_message(target_room, "battery", value);
        }
    }
    else if (data_in[3] == 'B' && data_in[4] == 'A' && data_in[5] == 'R')
    { // barometric pressure
        char value[6];
        str_find_num(data_in, 12, value);
        mqtt_send_message(target_room, "pressure", value);
    }
    else if (data_in[3] == 'R' && data_in[4] == 'H')
    { // humidity
        char value[6];
        str_find_num(data_in, 12, value);
        mqtt_send_message(target_room, "humidity", value);
    }
}

void parse_data_packet()
{
    //
    // Parse the received data to find the room
    //

    if (data_in[0] == 'a')
    { // valid sensor packets start with an 'a'
        if (data_in[1] == 'T' && data_in[2] == 'A')
        { // Bedroom 2
            parse_sensor(BEDROOM2, true);
        }
        else if (data_in[1] == 'T' && data_in[2] == 'B')
        { // Living Room
            parse_sensor(LIVINGROOM, true);
        }
        else if (data_in[1] == 'T' && data_in[2] == 'C')
        { // Bedroom 1
            parse_sensor(BEDROOM1, true);
        }
        else if (data_in[1] == 'T' && data_in[2] == 'D')
        { // Bedroom 3
            parse_sensor(BEDROOM3, true);
        }
        else if (data_in[1] == 'T' && data_in[2] == 'E')
        { // Kitchen
            parse_sensor(KITCHEN, true);
        }
        else if (data_in[1] == 'T' && data_in[2] == 'F')
        { // Bathroom
            parse_sensor(BATHROOM, true);
        }
        else if (data_in[1] == 'T' && data_in[2] == 'G')
        { // Loft
            parse_sensor(LOFT, true);
        }
        else if (data_in[1] == 'H' && data_in[2] == 'A')
        { // Outdoor Humidity Sensor
            parse_sensor(OUTDOORS, true);
        }
        else if (data_in[1] == 'P' && data_in[2] == 'A')
        { // Outdoor Pressure Sensor
            parse_sensor(OUTDOORS, false);
        }
    }
}

void signal_handler_IO(int status)
{
    //
    // Interupt handler for serial input
    //

    ioctl(fd, FIONREAD, &bytes_available);
    if (bytes_available > 11)
    {
        if (bytes_available < 16)
        { // stop buffer overlow in data_in
            read(fd, &data_in, bytes_available);
            trace(data_in, 1);
            data_received = 0;
            parse_data_packet();
        }
    }
}

void start_srf_sensors()
{

    //
    // Start monitoring for SRF sensor input on the serial port
    //

    struct termios options; // Terminal options
    int rc;                 // Return value

    struct sigaction serint;

    fd = open_port(); // Open tty device for RD and WR

    // Get the current options for the port
    if ((rc = tcgetattr(fd, &options)) < 0)
    {
        trace("Failed to get attributes for SRF serial port\n", 3);
        exit(EXIT_FAILURE);
    }

    // serial interrupt handler
    serint.sa_handler = signal_handler_IO;
    serint.sa_flags = 0;
    serint.sa_restorer = NULL;
    sigaction(SIGIO, &serint, NULL);

    fcntl(fd, F_SETFL, FNDELAY);
    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, O_ASYNC);

    // Set the baud rates
    cfsetispeed(&options, SRF_BAUD_RATE);
    cfsetospeed(&options, SRF_BAUD_RATE);

    cfmakeraw(&options);
    options.c_cflag |= (CLOCAL | CREAD); // Enable the receiver and set local mode
    options.c_cflag &= ~CSTOPB;          // 1 stop bit
    options.c_cflag &= ~CRTSCTS;         // Disable hardware flow control
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    // Set the new attributes
    if ((rc = tcsetattr(fd, TCSANOW, &options)) < 0)
    {
        trace("Failed to set attributes for SRF serial port\n", 3);
        exit(EXIT_FAILURE);
    }

    clear_buffer(); // flush the serial buffer
}
