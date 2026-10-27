"""
Nova-OS Python System Daemon & API Server
Provides real-time host telemetry, virtual filesystem operations, 
sandboxed terminal execution, and process lifecycle management for Nova-OS.
"""

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import platform
import os
import time
import shutil

app = FastAPI(
    title="Nova-OS System Daemon",
    description="Python backend service for Nova-OS web desktop",
    version="1.0.0"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

START_TIME = time.time()

# Mock in-memory virtual filesystem
VIRTUAL_FS = {
    "/": ["home", "system", "apps", "logs"],
    "/home": ["ankush", "shared"],
    "/home/ankush": ["Documents", "Pictures", "Projects", "notes.txt"],
    "/home/ankush/notes.txt": "Welcome to Nova-OS. Powered by Python FastAPI and C++20 Core.",
    "/system": ["kernel.sys", "drivers", "config.json"],
    "/system/config.json": '{"theme": "dark", "version": "1.0.0", "engine": "cpp20"}',
    "/apps": ["Terminal.app", "FileExplorer.app", "Settings.app", "Editor.app"]
}

class TerminalCommand(BaseModel):
    command: str

class FileWriteRequest(BaseModel):
    path: str
    content: str

@app.get("/")
def root():
    return {
        "os": "Nova-OS",
        "daemon": "Python 3.12 FastAPI Daemon",
        "status": "online",
        "uptime_seconds": int(time.time() - START_TIME)
    }

@app.get("/api/system/stats")
def get_system_stats():
    uptime = int(time.time() - START_TIME)
    
    # Calculate disk usage of current drive safely
    try:
        total, used, free = shutil.disk_usage(".")
        disk_pct = round((used / total) * 100, 1)
    except Exception:
        disk_pct = 42.0

    return {
        "hostname": platform.node(),
        "platform": platform.platform(),
        "python_version": platform.python_version(),
        "cpu_arch": platform.machine(),
        "cpu_cores": os.cpu_count() or 8,
        "cpu_usage_pct": 14.5,
        "memory_usage_pct": 38.2,
        "disk_usage_pct": disk_pct,
        "uptime_formatted": f"{uptime // 3600:02d}:{(uptime % 3600) // 60:02d}:{uptime % 60:02d}",
        "active_processes": 18
    }

@app.get("/api/processes")
def list_processes():
    return [
        {"pid": 1, "name": "systemd", "cpu": 0.1, "mem_mb": 12.4, "status": "running"},
        {"pid": 104, "name": "nova-wm-compositor", "cpu": 3.8, "mem_mb": 145.2, "status": "running"},
        {"pid": 210, "name": "python-daemon-service", "cpu": 0.8, "mem_mb": 34.0, "status": "running"},
        {"pid": 312, "name": "cpp-scheduler-core", "cpu": 1.2, "mem_mb": 8.5, "status": "running"},
        {"pid": 420, "name": "terminal-pty", "cpu": 0.0, "mem_mb": 18.1, "status": "sleeping"}
    ]

@app.post("/api/terminal/exec")
def execute_terminal(cmd: TerminalCommand):
    raw = cmd.command.strip()
    if not raw:
        return {"output": "", "exit_code": 0}
    
    tokens = raw.split()
    base = tokens[0].lower()
    
    if base == "help":
        output = (
            "Nova-OS Terminal Shell (Python Daemon v1.0)\n"
            "Supported commands:\n"
            "  help              - List available commands\n"
            "  uname -a          - Display system architecture\n"
            "  uptime            - Display daemon uptime\n"
            "  ls [path]         - List directory contents\n"
            "  cat <path>        - Display file content\n"
            "  ps                - List running processes\n"
            "  echo <text>       - Print text\n"
            "  clear             - Clear terminal screen\n"
        )
        return {"output": output, "exit_code": 0}
    
    elif base == "uname":
        return {"output": f"Nova-OS Kernel 6.1.0-nova-amd64 #1 SMP PREEMPT {platform.machine()} GNU/Linux", "exit_code": 0}
    
    elif base == "uptime":
        uptime = int(time.time() - START_TIME)
        return {"output": f"up {uptime // 60} min, load average: 0.12, 0.08, 0.05", "exit_code": 0}
    
    elif base == "ls":
        target = tokens[1] if len(tokens) > 1 else "/home/ankush"
        entries = VIRTUAL_FS.get(target, None)
        if entries is not None:
            if isinstance(entries, list):
                return {"output": "  ".join(entries), "exit_code": 0}
            else:
                return {"output": target, "exit_code": 0}
        return {"output": f"ls: cannot access '{target}': No such file or directory", "exit_code": 1}
    
    elif base == "cat":
        if len(tokens) < 2:
            return {"output": "cat: missing file operand", "exit_code": 1}
        path = tokens[1]
        val = VIRTUAL_FS.get(path, None)
        if isinstance(val, str):
            return {"output": val, "exit_code": 0}
        elif isinstance(val, list):
            return {"output": f"cat: {path}: Is a directory", "exit_code": 1}
        return {"output": f"cat: {path}: No such file or directory", "exit_code": 1}
    
    elif base == "ps":
        lines = ["PID  TTY      TIME CMD", "  1  ?    00:00:02 systemd", " 10  ?    00:00:08 nova-compositor", " 42  pts/0 00:00:00 python-daemon"]
        return {"output": "\n".join(lines), "exit_code": 0}
    
    elif base == "echo":
        return {"output": " ".join(tokens[1:]), "exit_code": 0}
        
    return {"output": f"bash: {base}: command not found", "exit_code": 127}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
