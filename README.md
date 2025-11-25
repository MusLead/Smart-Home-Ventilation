# Smart-Home-Ventilation
This is a Semester Project for Internet of Things Project Hochschule Fulda WS25/26, Germany

## Project Architecture

This project consists of multiple components:

### ESP-GET_READY
The [ESP-GET_READY](https://github.com/Mr1JS/ESP-GET_READY) submodule is a repository dedicated to managing communication between ESP microcontrollers and various sensors and actuators. It provides different HTTP REST-API endpoints that enable the application to access and control the ESP devices through the internet.

### Application Side
A separate repository exists for the Application side, which will interact with the ESP devices through the REST-API provided by ESP-GET_READY.
