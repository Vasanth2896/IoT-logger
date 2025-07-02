# NodeMCU UART IoT Logger

This project implements a lightweight IoT logger using a **NodeMCU (ESP32)** board. It reads log messages over **UART** from an STM32-based password authentication system and forwards them to a remote cloud service using **MQTT**.

## Overview

The STM32 sends access-related messages (e.g., "Access Granted", "Invalid Password") over UART. The NodeMCU receives this data and publishes it to a cloud MQTT broker for monitoring, logging, or integration with other systems.

## Features

- UART communication with STM32 (9600 baud rate or configurable)
- MQTT publishing to a configurable topic and broker
- Automatic Wi-Fi connection
- Basic error handling and reconnection logic

## Hardware Setup

### Connections

| STM32 TX | → | NodeMCU RX (GPIO3 / D9) |
|----------|---|-------------------------|
| GND      | → | GND                     |

- Ensure both devices share a **common ground**.
- UART baud rate must match on both sides (default: 9600).

## Dependencies

- Platform: Arduino
- Board: NodeMCU (ESP32)


## Configuration

Sample Config given below
```cpp
#define WIFI_SSID     "your-ssid"
#define WIFI_PASSWORD "your-password"

#define MQTT_BROKER   "broker.hivemq.com"
#define MQTT_PORT     1883
#define MQTT_TOPIC    "stm32/auth/logs"
