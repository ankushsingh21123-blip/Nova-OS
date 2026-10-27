# Nova-OS Python System Daemon

High-performance asynchronous backend service for Nova-OS web desktop.

## Features
- **Host Telemetry API (`/api/system/stats`):** Streams real-time CPU architecture, load averages, memory consumption, disk capacity, and uptime.
- **Sandboxed Virtual Terminal (`/api/terminal/exec`):** Secure command evaluation and parsing for the web terminal app.
- **Virtual File System Service (`/api/fs`):** Node-based hierarchy manager for virtual disks, user directories, and system binaries.
- **Process Supervisor (`/api/processes`):** In-memory task manager reflecting real and simulated system threads.

## Quickstart

```bash
pip install -r requirements.txt
python main.py
```
Server runs at `http://localhost:8000`.
