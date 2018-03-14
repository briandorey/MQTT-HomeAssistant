#!/usr/bin/python
import datetime
import os
import subprocess
import threading
import time
import urllib2
import paho.mqtt.client as mqtt

#
# MQTT Server Log Upload
#
# Date: 2018/03/11
# Version: 1.0
# Description: read sensor values from the MQTT broker
#              and upload them to the remote server
#

DEBUG = False


def trace(output):
    if DEBUG:
        print(output)

server_url = ''

power_server_url = ''

mqtt_broker = ''
mqtt_user = ''
mqtt_pass = ''
mqtt_port = 1883

dict = {'/home/bathroom/temperature': 0,
        '/home/bedroom1/temperature': 0,
        '/home/bedroom2/temperature': 0,
        '/home/bedroom3/temperature': 0,
        '/home/kitchen/temperature': 0,
        '/home/livingroom/temperature': 0,
        '/home/loft/temperature': 0,
        '/home/outdoors/temperature': 0,
        '/home/outdoors/pressure': 0,
        '/home/outdoors/humidity': 0,
        '/home/water/solarcollector': 0,
        '/home/water/cylindertop': 0,
        '/home/water/cylinderbase': 0,
        '/home/water/solarpump': 0,
        '/home/power/gasmeter': 0,
        '/home/power/electricmeter': 0,
        '/home/power/mainscurrent': 0,
        '/home/power/avgmainscurrent': 0,
        '/home/power/maxmainscurrent': 0,
        }


trace("Logging Started")


# The callback for when the client receives a CONNACK response from the server.
def on_connect(mqttc, obj, flags, rc):
    trace("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.

    for key, value in dict.items():
        mqttc.subscribe(key)
        print("Subscribed to: ", key)


# The callback for when a PUBLISH message is received from the server.
def on_message(mqttc, obj, msg):
    trace(msg.topic+" "+str(msg.payload))
    for key, value in dict.items():
        if key == msg.topic:
            dict[key] = msg.payload


def post_to_server():
    #
    # Function to upload data to online server
    #

    # set a thread to repeat every 5 minutes
    t = threading.Timer(300.0, post_to_server)
    t.daemon = True
    t.start()

    if dict['/home/water/cylindertop'] > 0:
        # do server update
        try:
            req = str(server_url +
                      'watertop=' + str(dict['/home/water/cylindertop']) +
                      '&waterbase=' + str(dict['/home/water/cylinderbase']) +
                      '&waterpanel=' + str(dict['/home/water/solarcollector']) +
                      '&hometemp1=' + str(dict['/home/livingroom/temperature']) +
                      '&hometemp2=' + str(dict['/home/bedroom2/temperature']) +
                      '&hometemp3=' + str(dict['/home/bedroom1/temperature']) +
                      '&hometemp4=' + str(dict['/home/bedroom3/temperature']) +
                      '&hometemp5=' + str(dict['/home/kitchen/temperature']) +
                      '&hometemp6=' + str(dict['/home/bathroom/temperature']) +
                      '&hometemp7=' + str(dict['/home/loft/temperature']) +
                      '&outdoortemp=' + str(dict['/home/outdoors/temperature']) +
                      '&humidity=' + str(dict['/home/outdoors/humidity']) +
                      '&pressure=' + str(dict['/home/outdoors/pressure']) +
                      '&pump=' + str(dict['/home/water/solarpump']) + '')
            trace(req)
            resp = urllib2.urlopen(req)
        except:
            trace("http connection failed")

def post_power_to_server():
    #
    # Function to upload data to online server
    #

    # set a thread to repeat every 1 minutes
    t2 = threading.Timer(60.0, post_power_to_server)
    t2.daemon = True
    t2.start()

    if dict['/home/power/gasmeter'] > 0:
        # do server update
        try:
            req = str(power_server_url +
                      'gasmeter=' + str(dict['/home/power/gasmeter']) +
                      '&electricmeter=' + str(dict['/home/power/electricmeter']) +
                      '&mainscurrent=' + str(dict['/home/power/mainscurrent']) +
                      '&mainscurrentavg=' + str(dict['/home/power/avgmainscurrent']) +
                      '&mainscurrentmax=' + str(dict['/home/power/maxmainscurrent']) + '')
            trace(req)
            resp = urllib2.urlopen(req)
        except:
            trace("http connection failed")


def main():

    post_to_server()
    time.sleep(5)
    post_power_to_server()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(mqtt_user, mqtt_pass)

    client.connect(mqtt_broker, mqtt_port, 60)

    client.loop_forever()


if __name__ == "__main__":
    main()
