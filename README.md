# 🧲 MagLev Toolbox 🧲

![MATLAB](https://img.shields.io/badge/MATLAB-R2025b-blue)
![Simulink](https://img.shields.io/badge/Simulink-Supported-orange)
![Platform](https://img.shields.io/badge/Hardware-Teensy%204.1-green)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

> Rapid Control Prototyping (RCP) Toolbox for the MagLev v4.0+ Magnetic Levitation System using MATLAB® and Simulink®.

## Overview

The **MagLev Toolbox** MATLAB®/Simulink® toolbox has been developed to support
**Rapid Control Prototyping (RCP)** for the [**MagLev v4.0+ Magnetic
Levitation platform**](https://github.com/Hansolini/Take-home-Maglev-lab).

The toolbox enables students and researchers to:

- Design control algorithms in Simulink
- Automatically generate embedded code for the target hardware
- Deploy controllers to the MagLev hardware
- Run models in **Standalone Mode** or **External Mode** (_Connected I/O Mode_ not supported yet). 
- Perform real-time monitoring and parameter tuning (External Mode)
- Run fully standalone real-time experiments
- Log experimental data via a dedicated host-side application (Data Logging App)

The toolbox bridges **control theory**, **embedded implementation**, and
**real-time experimentation** within a structured and reproducible
workflow.

## Rapid Control Prototyping Workflow

The toolbox supports the complete RCP cycle:

> **Model → Simulate → Deploy → Tune → Validate**

1. Develop and test controllers in simulation
2. Deploy to the target hardware
3. Tune gains and parameters in real time
4. Log experimental data
5. Validate performance and iterate

This approach makes the MagLev v4.0+ system an effective benchmark
platform for:

- Feedback control courses
- Embedded systems laboratories
- Advanced control research
- Real-time systems experimentation

## System Architecture

    +-------------------------+      USB Serial     +------------------------+
    | MATLAB / Simulink       | <-----------------> |  MagLev v4.0+ Hardware |
    | Host PC                 |                     |  Teensy 4.1 Target     |
    |                         |                     |                        |
    | - Control Design        |                     | - Magnetic Sensors     |
    | - Parameter Tuning      |                     | - Current Sensors      |
    | - Data Logging          |                     | - Current Drivers      |
    |                         |                     | - User LED             |
    +-------------------------+                     +------------------------+

## Key Features

- Custom Simulink blocks for:
  - Magnetic sensors
  - Current sensors
  - Coil current drivers
  - USB Serial binary packet bi-directional communication

- Support for:
  - Standalone deployment
  - External Mode (real-time tuning)

- Bi-directional binary serial communication with:
  - Optional COBS encoding
  - Optional null termination
  - Optional DTR-based transmission gating

- Dedicated MATLAB Serial Data Logger App

- Structured workflow suitable for teaching and research

## Requirements

### Software

- MATLAB® R2025b (or later recommended)
- Simulink®
- Simulink Coder™
- Embedded Coder®
- Arduino Support Package for MATLAB/Simulink (configured for Teensy 4.1)


### Supported PCB Versions

The MagLev Toolbox currently supports the following hardware revisions:

- **MagLev 4.0**
  - Equipped with **8 magnetic sensors** (Infineon TLV493D)
  - Full multi-sensor configuration for spatial magnetic field measurement

- **MagLev 4.3**
  - Equipped with **1 magnetic sensor** (Infineon TLV493D)
  - PCB traces rerouted to improve **noise immunity**
  - Simplified sensing architecture for improved signal quality

The toolbox automatically adapts:
- Available Simulink blocks
- Mask parameters
- Internal implementation

based on the selected PCB version.


### Hardware

- MagLev v4.0+ Magnetic Levitation System
- Teensy 4.1 development board
- USB connection to host PC

## Installation

1. Clone the repository:

``` bash
git clone https://github.com/riccardoantonello/MagLevTbx.git
```

2. In MATLAB, change directory to the `Matlab` folder inside the toolbox root:

```matlab
cd('Your-MagLev-Toolbox-Root-Folder/Matlab')
```

3. Run the setup script to:

- Select the PCB version to use (see below) 
- Compile all required C-MEX S-Functions
- Add the toolbox folders to the MATLAB path
- Save the updated path configuration

``` matlab
MagLevTbx_Setup
```

4. Restart MATLAB.



### Supported PCB Versions

The MagLev Toolbox currently supports the following hardware revisions:

- **MagLev 4.0**
  - Equipped with **8 magnetic sensors** (Infineon TLV493D)
  - Full multi-sensor configuration for spatial magnetic field measurement

- **MagLev 4.3**
  - Equipped with **1 magnetic sensor** (Infineon TLV493D)
  - PCB traces rerouted to improve **noise immunity**
  - Simplified sensing architecture for improved signal quality

The toolbox automatically adapts:
- Available Simulink blocks
- Mask parameters
- Internal implementation

based on the selected PCB version.


### Hardware Configuration (PCB Version)

During the setup procedure, a dialog window will open to allow the user to select the hardware (PCB) version:

- **MagLev 4.0**
- **MagLev 4.3**

This selection configures the toolbox behavior, enabling the appropriate Simulink blocks and hardware interfaces.

#### Reconfiguring Hardware Version

After installation, the hardware version can be changed at any time by running:

```matlab
MagLevTbx_HwConfig()
```

This will reopen the configuration dialog and update the toolbox settings accordingly.


## Quick Start

The toolbox includes a set of simple demo models to verify and explore the basic functionalities of the MagLev Toolbox blockset.

---

### 1. Open an Example Model

To access the demos, run:

```matlab
open_system('MagLevTbx_Demos')
```
and follow the links in the demo launcher. The available demos are:

**🔹 Magnetic Sensor Test** (_Standalone Mode, External Mode_):
Demonstrates how to read the magnetic field intensity from one of the eight magnetic sensors available on the MagLev platform.

**🔹 Current Driver & Sensor Test** (_Standalone Mode, External Mode_):
Demonstrates how to impose a specified current on the levitation coils and measure the resulting current using the onboard current sensors.

**🔹 User LED Test** (_Standalone Mode, External Mode_):
Demonstrates how to control the User LED for debugging and status indication purposes.

**🔹 USB Serial Data Stream Test** (_Standalone Mode_):
Demonstrates how to stream binary packet data via USB serial connection and log data in real time using the `serialDataLoggerApp`.

**🔹 USB Serial Loopback Test** (_Standalone Mode_):
Demonstrates bidirectional USB serial communication between the host PC and the embedded target.

---

### 2-A. Running an Example Model in Standalone Mode (Autonomous Execution)

### What It Is

In **Standalone Mode**, the Simulink model is converted into embedded C/C++ code, compiled, and deployed to the Teensy 4.1 target.
After deployment:
- The controller runs entirely on the embedded platform
- MATLAB/Simulink is no longer required (unless for data logging purposes)
- Execution is fully real-time and deterministic

### Working Principle

1. Simulink Coder generates C/C++ code from the model.
2. The code is compiled and uploaded to the Teensy board.
3. The target executes the control algorithm at the specified sample time.
4. Optional data streaming can be performed via USB serial.

### How To Run

1. Open the demo model.
2. Go to the **Hardware tab**.
3. Click **Run** on **Board ▸ Build, Deploy & Start**.
4. Wait for compilation and upload.
5. The model runs independently on the target. (If applicable) Launch the Serial Data Logger App to monitor data in real time (sent through the USB Serial Port).

---

### 2-B. Running an Example Model in External Mode (Real-Time Tuning)

### What It Is

In External Mode, the generated code runs on the target hardware, while Simulink remains connected to the model for:
- Real-time signal monitoring
- Parameter tuning
- Data logging


### Working Principle

1. Simulink generates and deploys the model to the target.
2. A communication link (USB) is established between host and target.
3. The control algorithm runs on the Teensy board.
4. Selected signals are streamed back to Simulink.
5. Parameters can be modified live without recompiling.
   
This mode is ideal for controller tuning and experimental validation.

### How To Run

1. Open the demo model.
2. Open the **Hardware** tab
3. Select **Run on Board ▸ Run on Board (External Mode)**
4. Click **Monitor & Tune**


## Intended Audience

### 🎓 Students

- Learning feedback control implementation
- Understanding real-time constraints
- Bridging theory and hardware experimentation

### 🧑‍🔬 Researchers

- Rapid controller validation
- Experimental benchmarking
- Embedded control prototyping

## Educational and Research Use

MagLev Toolbox is intended for academic and laboratory use.\
It provides a structured environment for experimenting with:

- PID control
- State-space control
- Observers
- Nonlinear control
- Advanced real-time control strategies

## License

The MagLev Toolbox is available under the BSD 2-Clause License. See the LICENSE file for more information.

## Citation

If you use this toolbox in research or publications, please cite:

**R. Antonello,  D. Varagnolo, "The MagLev Toolbox: Rapid Control Prototyping Framework for the MagLev v4.0+ Platform", Dept. of Information Engineering, University of Padua, 2026.**
