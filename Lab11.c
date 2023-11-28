#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"
#include "bmp180.h"

#define ADDRESS     "Your Broker address"
#define CLIENTID    "Ra"
#define TOPIC       "RaspiMQTT"
#define USERNAME    "Your Username"
#define PASSWORD    "Your Password"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    // Initialize MQTT Client
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Initialize BMP180 sensor
    char *i2c_device = "/dev/i2c-1";
    int address = 0x77;
    void *bmp = bmp180_init(address, i2c_device);

    if(bmp != NULL){
        while(1) {
            float temperature = bmp180_temperature(bmp);
            long pressure = bmp180_pressure(bmp);
            long led = 0.0;
            long switch = 0.09;
        

            char payload[10]; // Adjust the size as necessary
            sprintf(payload, "{\"hardware\": \"RaspiMQTT\", \"payload\": {\"temperature\": %.2f, \"pressure\": %ld, \"led\": %.2f, \"switch\": %.2f} }", 
                    temperature, pressure, led, switch);

            pubmsg.payload = payload;
            pubmsg.payloadlen = strlen(payload);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;
            MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

            printf("Published data: %s\n", payload);
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Message with delivery token %d delivered\n", token);

            sleep(1); // Sleep for a second before next reading
        }
    } else {
        printf("BMP180 sensor initialization failed\n");
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
