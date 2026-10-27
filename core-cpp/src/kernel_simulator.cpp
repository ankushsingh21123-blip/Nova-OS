#include "kernel_simulator.hpp"
#include <iostream>

namespace nova::kernel {

// Virtual Memory Manager Implementation
VirtualMemoryManager::VirtualMemoryManager(size_t frame_capacity)
    : capacity_(frame_capacity) {}

bool VirtualMemoryManager::access_page(uint32_t page_id) {
    total_accesses_++;
    auto it = page_map_.find(page_id);
    
    // Page Hit
    if (it != page_map_.end()) {
        lru_list_.erase(it->second);
        lru_list_.push_front(page_id);
        page_map_[page_id] = lru_list_.begin();
        return true;
    }

    // Page Fault
    page_faults_++;
    if (lru_list_.size() >= capacity_) {
        uint32_t victim = lru_list_.back();
        lru_list_.pop_back();
        page_map_.erase(victim);
    }

    lru_list_.push_front(page_id);
    page_map_[page_id] = lru_list_.begin();
    return false;
}

double VirtualMemoryManager::get_hit_rate() const {
    if (total_accesses_ == 0) return 0.0;
    return (1.0 - (static_cast<double>(page_faults_) / total_accesses_)) * 100.0;
}

// Kernel Scheduler Implementation
KernelScheduler::KernelScheduler(uint32_t time_slice_ms)
    : time_slice_ms_(time_slice_ms) {}

void KernelScheduler::spawn_process(uint32_t pid, const std::string& name, uint32_t priority, uint32_t burst_time_ms) {
    ProcessControlBlock pcb{
        pid,
        name,
        priority,
        burst_time_ms,
        burst_time_ms,
        ProcessState::READY
    };
    ready_queue_.push(pcb);
}

void KernelScheduler::run_simulation(size_t max_cycles) {
    size_t cycles = 0;
    while (!ready_queue_.empty() && cycles < max_cycles) {
        ProcessControlBlock current = ready_queue_.front();
        ready_queue_.pop();
        current.state = ProcessState::RUNNING;
        context_switches_++;

        uint32_t execute_time = std::min(current.remaining_time_ms, time_slice_ms_);
        current.remaining_time_ms -= execute_time;

        if (current.remaining_time_ms == 0) {
            current.state = ProcessState::TERMINATED;
            completed_processes_.push_back(current);
        } else {
            current.state = ProcessState::READY;
            ready_queue_.push(current);
        }
        cycles++;
    }
}

} // namespace nova::kernel
