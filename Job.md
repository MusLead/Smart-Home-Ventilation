# Job Distribution Overview 
Written by Agha M. Aslam (15 November 2025)

## Phase 1 — Core Hardware Validation (Sensors + Actuators)

**Responsible:** Jatin
**Support:** Agha

### Goals
- Test all primary sensors (temperature, humidity, etc.)  
- Test core actuators (fan, vent motor / flap)  
- Validate data format, signal stability, update rates, and noise levels  
- Document behaviour and limitations for controller development  
- Prepare architectural “hooks” for future extension nodes:
  - Wind sensor  
  - Humidifier / dehumidifier components  

### Note:
Jatin helps with obtaining and testing sensors/actuators and with wiring/test setup, ensuring the plan is realistic.
Enrico provided us with the fan and showed us how to use the pins to operate it as a ventilator.

---

## Phase 1.2 — Simulation Layer (Recommended)

**Responsible:** Jatin 
**Support:** Agha

### Goals
- Create an MQTT-based simulator for early testing  
- Publish synthetic values (humidity, temperature, wind)  
- Support WebApp (REST API through HTML) and controller testing before hardware is ready  
- Provide fallback behaviour during demos  

## Phase 1.3 — Demo Scenarios & Behaviour Specification

**Responsible:** Agha
**Support:** Jatin and Enrico

### Goals
Define clear, realistic demo scenarios that determine the requirements for the controller, MQTT communication, and WebApp design. Also we want a Real Life Application such as integration with the thermostat eve

### Demo Scenarios

#### 1. **Scenario: “Too Humid Inside”**
- Rising humidity above threshold  
- Controller triggers “Ventilation On”  
- Fan increases speed, vent opens  
- WebApp displays warning and status  

#### 2. **Scenario: “Eco Night Mode”**
- Outside temp too low, but humidity still high
- Controller chooses eco-friendly trade-off
- Reduced fan speed or turn it off, vent opens wide

### Scenario Parameter Definition
For each scenario, define and document:  
- Sensor units, valid ranges, noise expectations  
- Numeric thresholds (humidity %, temperature °C, wind levels)  
- Actuator behaviour curves (fan 0–3 levels, vent 0–100%)  
- Timing parameters (debounce, averaging, delays)  
- Fallback behaviour when a sensor is unavailable  
- MQTT topics and JSON structure  
- Required API endpoints  
- Required UI elements (graphs, toggles, alerts)

These specifications ensure Phase 2 and Phase 4 can be developed without ambiguity.

### Note:
Agha will be responsible for managing the app's diagrams, schemas, and wireframes.

⸻

## Phase 2 — Communication & Controller System (MQTT + Logic + API)

**Responsible:** Agha & Jatin  
**Support:** Enrico (hardware alignment)

### Goals
- Set up MQTT broker and define topic hierarchy  
- Implement controller logic:
  - Automatic mode  
  - Manual mode  
  - Eco mode  
- Process sensor data, apply scenario rules, and send actuator commands  
- Define REST API early based on Phase 1.2 scenario requirements  
- Create a modular architecture for extension nodes  

### Data Format Standardization
- JSON schema for all sensor and actuator messages  
- Naming conventions  
- Units (°C, %, m/s, Boolean states)  
- Timestamp format (ISO 8601)  
- Error message structure  

### Validation Tasks
- Test MQTT with both real sensors and simulated data  
- Validate controller timing and message order  
- Test fallback logic (sensor failure, communication loss)

### Additional Task
- Agha develops the mini-humidifier (with Enrico's assistance if needed)

⸻

## Phase 3 — Miniature Construction (Physical Model)

**Responsible:** Enrico  
**Support:** Agha & Jatin (model design)  
**Resources:** HS Fulda Makerspace

### Goals
- Build miniature: rooms, airflow system, mounting points  
- Use appropriate materials:
  - Birch plywood  
  - MDF  
  - Acrylic (windows)  
  - PET-G or PVC sheets  
- Ensure stable structure, safe wiring, and clean internal layout  
- Integrate sensors and actuators into the model  

### Critical Makerspace Tasks
- Book laser cutter and tools early  
- Verify material availability  
- Prepare alternatives if tools are booked  
- Coordinate with Makerspace staff for fabrication and safety  

This ensures the miniature can be built without unexpected delays.

⸻

## Phase 4 — Application (WebApp / UI / Security)

**Responsible:** Enrico (Web App)
**Support:** Jatin (Optional: Android App), Agha

### Goals
- Develop dashboard interface for:
  - Live sensor data  
  - Actuator states  
  - Scenario-specific warnings  
  - Mode switching (Automatic, Manual, Eco)  
- Integrate WebApp with REST API  
- Enable real-time data visualization  
- Ensure responsive layout (desktop & mobile)

### Security Requirements
- Validate all API inputs  
- No exposed MQTT endpoints  
- Use HTTPS locally / TLS-ready architecture  
- No credentials stored in code  
- Provide local-only access mode  
- Rate limiting & sanitization for user inputs  
- Protect against:
  - XSS  
  - CSRF  
  - Injection attacks  
  - Directory traversal  
  - Broken authentication  

### Testing Requirements
- Test UI with simulated and real sensor data  
- Validate error handling & UI stability  
- Perform basic manual security testing  
- Test across different devices (laptop, phone, tablet)


⸻

## Phase 5 — Final Integration & Demo

**Responsible:** All Team Members

### Goals
- Combine sensors, controller, miniature, and WebApp  
- Test all demo scenarios end-to-end  
- Perform stress tests (rapid changes, hardware limits)  
- Use simulator as fallback for unreliable sensors  
- Document final architecture, decisions, and limitations  
- Prepare demo flow & presentation materials  

---
