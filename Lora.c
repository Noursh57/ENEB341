#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <string.h>

// Define LoRa registers and constants
#define REG_OPMODE 0x01
#define REG_FIFO 0x00
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_TX_BASE_ADDR 0x0E
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define REG_PA_CONFIG 0x09
#define REG_IRQ_FLAGS 0x12
#define REG_PAYLOAD_LENGTH 0x22

// Define modes and other settings
#define OPMODE_SLEEP 0x00
#define OPMODE_STANDBY 0x01
#define OPMODE_TX 0x03
#define OPMODE_RXCONTINUOUS 0x05
#define PA_BOOST 0x80

// Function declarations
void setupLoRa();
void writeRegister(uint8_t reg, uint8_t value);
uint8_t readRegister(uint8_t reg);
void sendMessage(const char *message);
void receiveMessage(char *buffer);

void setupLoRa() {
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed. Maybe you haven't installed it yet?");
        return;
    }

    int fd = wiringPiSPISetupMode(0, 500000, 0);
    if (fd == -1) {
        printf("SPI setup failed.");
        return;
    }

    // Initial setup for your LoRa modem
    writeRegister(REG_OPMODE, OPMODE_SLEEP);
    writeRegister(REG_PA_CONFIG, PA_BOOST);
    // ... Set frequency, spreading factor, and other settings
    writeRegister(REG_OPMODE, OPMODE_STANDBY);
}

void writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[0] = reg | 0x80;
    buffer[1] = value;
    wiringPiSPIDataRW(0, buffer, 2);
}

uint8_t readRegister(uint8_t reg) {
    uint8_t buffer[2];
    buffer[0] = reg & 0x7F;
    buffer[1] = 0x00;
    wiringPiSPIDataRW(0, buffer, 2);
    return buffer[1];
}

void sendMessage(const char *message) {
    writeRegister(REG_OPMODE, OPMODE_STANDBY);
    writeRegister(REG_FIFO_ADDR_PTR, 0x00);
    writeRegister(REG_PAYLOAD_LENGTH, strlen(message));

    for (int i = 0; i < strlen(message); i++) {
        writeRegister(REG_FIFO, message[i]);
    }

    writeRegister(REG_OPMODE, OPMODE_TX);

    // Wait for transmission to complete
    while ((readRegister(REG_IRQ_FLAGS) & 0x08) == 0) {
        // Waiting for TX done flag
    }
    // Clear IRQ flags
    writeRegister(REG_IRQ_FLAGS, 0xFF);
}

void receiveMessage(char *buffer) {
    writeRegister(REG_OPMODE, OPMODE_RXCONTINUOUS);

    // Wait for reception to complete
    while ((readRegister(REG_IRQ_FLAGS) & 0x40) == 0) {
        // Waiting for RX done flag
    }
    // Clear IRQ flags
    writeRegister(REG_IRQ_FLAGS, 0xFF);

    int len = readRegister(REG_PAYLOAD_LENGTH);
    writeRegister(REG_FIFO_ADDR_PTR, 0x00);

    for (int i = 0; i < len; i++) {
        buffer[i] = readRegister(REG_FIFO);
    }
    buffer[len] = '\0';  // Null-terminate the received string
}

int main() {
    setupLoRa();

    char buffer[64];
    while (1) {
        printf("Sending message...\n");
        sendMessage("Hello LoRa!");

        printf("Waiting to receive message...\n");
        receiveMessage(buffer);
        printf("Received: %s\n", buffer);
    }

    return 0;
}
