# Arduino Sensor Source code

## Overview

Transmits temperature, humidity, and pressure data to a known endpoint via JSON REST API.

Runs on Arduino [MKR-WIFI-1010](https://store-usa.arduino.cc/products/arduino-mkr-wifi-1010?selectedStore=us) with the [MKR Env Shield](https://store.arduino.cc/collections/sensors-environment/products/arduino-mkr-env-shield-rev2). The implementation is optimized for battery life; with the board LED's removed it'll run for about 2 months with 3000 mAh Lithium Ion battery. It wakes up from deep sleep every 15 minutes, reads its sensors, transmits them to the server via WIFI, and goes back into deep sleep.

## Usage

Update the IP Address, SSID, and password in the single source file as appropriate. Build in the Arduino IDE and upload to your MKR-WIFI-1010.