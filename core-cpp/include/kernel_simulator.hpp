#pragma once

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <list>
#include <cstdint>

namespace nova::kernel {

enum class ProcessState {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

struct ProcessControlBlock {
    uint32_t pid{0};
    std::string name;
    uint32_t priority{1}; // 1 (lowest) - 10 (highest)
    uint32_t burst_time_ms{0};
    uint32_t remaining_time_ms{0};
    ProcessState state{ProcessState::READY};
};

// Virtual Memory LRU Paging Engine
class VirtualMemoryManager {
public:
    explicit VirtualMemoryManager(size_t frame_capacity);
    bool access_page(uint32_t page_id);
    size_t get_page_faults() const { return page_faults_; }
    size_t get_total_accesses() const { return total_accesses_; }
    double get_hit_rate() const;

private:
    size_t capacity_;
    size_t page_faults_{0};
    size_t total_accesses_{0};
    std::list<uint32_t> lru_list_;
    std::unordered_map<uint32_t, std::list<uint32_t>::iterator> page_map_;
};

// Preemptive Multitasking Process Scheduler
class KernelScheduler {
public:
    explicit KernelScheduler(uint32_t time_slice_ms = 10);
    
    void spawn_process(uint32_t pid, const std::string& name, uint32_t priority, uint32_t burst_time_ms);
    void run_simulation(size_t max_cycles = 1000);
    
    size_t completed_count() const { return completed_processes_.size(); }
    uint64_t total_context_switches() const { return context_switches_; }

private:
    uint32_t time_slice_ms_;
    uint64_t context_switches_{0};
    std::queue<ProcessControlBlock> ready_queue_;
    std::vector<ProcessControlBlock> completed_processes_;
};

} // namespace nova::kernel
