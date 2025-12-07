# Smart-Home-Ventilation
This is a Semester Project for Internet of Things Project Hochschule Fulda WS25/26, Germany

## Project Architecture

This project consists of multiple components:

### ESP-GET_READY
The [ESP-GET_READY](https://github.com/MusLead/ESP-GET_READY) submodule is a repository dedicated to managing communication between ESP microcontrollers and various sensors and actuators. It provides different HTTP REST-API endpoints that enable the application to access and control the ESP devices through the internet.

### ESP_Communcation_Center
The [ESP_Communcation_Center](https://github.com/MusLead/ESP_Communcation_Center) submodule serves as the communication server/broker between ESP sensors/actuators and the client/front-end server. It acts as a central communication hub that receives data from ESP-based devices, processes and routes messages, and provides an interface for the front-end server.

### SHVS_Front_End_Server
The [SHVS_Front_End_Server](https://github.com/MusLead/SHVS_Front_End_Server) submodule is the server with UI/UX that allows clients to access and control the entire Smart Home Ventilation System (SHVS).

## Submodules
Clone with submodules:
```
git clone --recurse-submodules https://github.com/MusLead/Smart-Home-Ventilation
```

Or initialize after cloning:
```
git submodule update --init --recursive
```
