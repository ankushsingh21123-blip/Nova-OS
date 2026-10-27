#include "kernel_simulator.hpp"
#include <iostream>
#include <random>

int main() {
    std::cout << "========================================================\n";
    std::cout << "   NOVA-OS C++20 KERNEL SCHEDULER & MEMORY SIMULATOR   \n";
    std::cout << "========================================================\n\n";

    // 1. Process Scheduler Benchmark
    nova::kernel::KernelScheduler scheduler(20); // 20ms quantum
    std::cout << "[*] Spawning simulated OS processes...\n";
    scheduler.spawn_process(1, "init_system", 10, 50);
    scheduler.spawn_process(2, "window_compositor", 8, 120);
    scheduler.spawn_process(3, "python_daemon_service", 6, 80);
    scheduler.spawn_process(4, "browser_sandbox", 5, 200);
    scheduler.spawn_process(5, "terminal_pty", 4, 40);

    scheduler.run_simulation();
    std::cout << "[+] Scheduler Simulation Finished.\n";
    std::cout << "    - Completed Processes : " << scheduler.completed_count() << "\n";
    std::cout << "    - Context Switches   : " << scheduler.total_context_switches() << "\n\n";

    // 2. Virtual Memory LRU Paging Benchmark
    std::cout << "[*] Running Virtual Memory Manager (Capacity: 8 Physical Frames)...\n";
    nova::kernel::VirtualMemoryManager vm(8);

    std::mt19937 rng(42);
    std::uniform_int_distribution<uint32_t> page_dist(1, 32);

    for (int i = 0; i < 5000; ++i) {
        vm.access_page(page_dist(rng));
    }

    std::cout << "[+] Memory Paging Stress Test Finished:\n";
    std::cout << "    - Total Memory Accesses : " << vm.get_total_accesses() << "\n";
    std::cout << "    - Page Faults           : " << vm.get_page_faults() << "\n";
    std::cout << "    - Cache Hit Rate        : " << vm.get_hit_rate() << "%\n\n";

    std::cout << "[+] Nova-OS C++20 Kernel Core verified operational.\n";
    return 0;
}
