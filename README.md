# Smart-Home-Ventilation
This is a Semester Project for Internet of Things Project Hochschule Fulda WS25/26, Germany

## Project Architecture

This project consists of multiple components:

### ESP-GET_READY
The [ESP-GET_READY](https://github.com/MusLead/ESP-GET_READY) submodule is a repository dedicated to managing communication between ESP microcontrollers and various sensors and actuators. It provides different HTTP REST-API endpoints that enable the application to access and control the ESP devices through the internet.

### ESP_Sensors_Actuators
The [ESP_Sensors_Actuators](https://github.com/MusLead/ESP_Sensors_Actuators) submodule contains the firmware and integration code for indoor and outdoor sensor/actuator nodes used by SHVS. It includes `ESP_Indoor_Sensors_Actuators` (e.g., BME680, window servo, absorber) and `ESP_Outdoor_Sensors_Actuators` (e.g., BME680, anemometer, door servo, relay switch), and it exchanges data and control messages via MQTT through `ESP_Communcation_Center`.

### ESP_Communcation_Center
The [ESP_Communcation_Center](https://github.com/MusLead/ESP_Communcation_Center) submodule serves as the communication server/broker between ESP sensors/actuators and the client/front-end server. It acts as a central communication hub that receives data from ESP-based devices, processes and routes messages, and provides an interface for the front-end server.

### SHVS_Front_End_Server
The [SHVS_Front_End_Server](https://github.com/MusLead/SHVS_Front_End_Server) submodule is the server with UI/UX that allows clients to access and control the entire Smart Home Ventilation System (SHVS).

### WebApp_EJS
`SHVS_Front_End_Server/WebApp_EJS` is the new Express + EJS frontend application. It duplicates and modernizes the frontend/proxy responsibilities of the older implementation, but follows the standard Express generator structure (`app.js`, `bin/www`, `routes`, `views`, `public`).

## Submodules
Clone with submodules:
```
git clone --recurse-submodules https://github.com/MusLead/Smart-Home-Ventilation
```

Or initialize after cloning:
```
git submodule update --init --recursive
```

Git Pull Submodules
````
git pull --recurse-submodules
````


## Frontend Notes

- `SHVS_Front_End_Server/WebApp_EJS` is the new generated Express app you should use going forward.
- `SHVS_Front_End_Server/WebApp_HTML` is the older HTML-oriented frontend inside the same submodule and remains for reference/history.
- The browser talks to the Express app, and the Express app forwards `/api/v1/...` requests to the ESP32 HTTP API.
- The system should only be used after `MQTT Connected` appears in the monitor of `ESP_Communcation_Center`.
