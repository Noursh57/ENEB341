#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
// Include your BMP180 sensor C library header here
// For example: #include "bmp180.h"

#define ADDRESS     "tcp://broker.things.ph:1883"
#define CLIENTID    "YourClientID"
#define TOPIC       "YourTopic"
#define USERNAME    "YourUsername"
#define PASSWORD    "YourPassword"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    char client_id[24]; // Ensure this is large enough for the ID and null terminator.
    int random_id = 0;

    // Initialize random number generator
    srand((unsigned) time(NULL));

    // Generate a random number between 0 and 1000
    random_id = rand() % 1001;

    // Create the client ID with the 'publish-' prefix and the random number
    sprintf(client_id, "publish-%d", random_id);

    // Initialize BMP180 sensor
    // bmp180_init(); // Use your actual BMP180 init function

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

    while(1) {
        char payload[100]; // Adjust the size as necessary

        // Read data from the BMP180 sensor
        float temperature = 0.0; // Use your actual BMP180 reading function
        float pressure = 0.0; // Use your actual BMP180 reading function
        sprintf(payload, "{\"temperature\": %.2f, \"pressure\": %.2f}", temperature, pressure);

        pubmsg.payload = payload;
        pubmsg.payloadlen = strlen(payload);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

        printf("Waiting for up to %ld seconds for publication of %s\n"
                "on topic %s for client with ClientID: %s\n",
                TIMEOUT/1000, payload, TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);

        sleep(1); // Sleep for a second before next reading
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
