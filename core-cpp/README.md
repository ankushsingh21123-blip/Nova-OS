# Nova-OS C++20 Kernel Core

Underlying C++ simulation engine for Nova-OS operating system concepts.

## Modules
1. **Preemptive Round-Robin Process Scheduler:** Simulates OS process dispatching, context switching, remaining quantum execution, and process state transitions (READY, RUNNING, BLOCKED, TERMINATED).
2. **Virtual Memory LRU Paging Subsystem:** O(1) hash-linked list LRU cache architecture modeling physical page frames, page replacement algorithms, and page fault rates.

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
./nova_kernel_sim
```
