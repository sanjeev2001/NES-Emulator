#pragma once

#include <cstdint>
#include <functional>

class nesAPU {
public:
    nesAPU();
    ~nesAPU();

    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);
    void clock();
    void reset();

    double GetOutputSample();

    // Sequencer
    uint8_t frame_counter_mode = 0x00;
    bool irq_inhibit = false;

private:
    uint32_t globalTime = 0;

    struct Sequencer {
        uint32_t sequence = 0;
        uint32_t timer = 0;
        int32_t reload = 0;
        uint8_t output = 0;
        uint32_t new_sequence = 0;
    };

    struct Oscillators {
        double frequency = 0.0;
        double duty_cycle = 0.0;
        double amplitude = 1.0;
        double harmonics = 20.0;
    };
    
    struct PulseChannel {
        bool enabled = false;
        bool halt = false; // also envelope loop
        bool envelope_enabled = false;
        uint8_t volume = 0;
        uint8_t envelope_period = 0;
        uint8_t envelope_decay_count = 0;
        bool envelope_start = false;
        
        uint8_t duty_mode = 0;
        
        bool sweep_enabled = false;
        uint8_t sweep_period = 0;
        bool sweep_negate = false;
        uint8_t sweep_shift = 0;
        uint8_t sweep_timer = 0;
        bool sweep_reload = false;
        
        uint16_t timer = 0;
        uint16_t timer_reload = 0;
        
        uint8_t length_counter = 0;
        
        uint8_t output = 0;
        
        // Internal helpers
        uint8_t sequence_pos = 0;
    } pulse1, pulse2;

    struct TriangleChannel {
        bool enabled = false;
        bool halt = false; // also linear counter control
        
        uint8_t linear_counter_reload = 0;
        uint8_t linear_counter = 0;
        bool linear_counter_reload_flag = false;
        
        uint16_t timer = 0;
        uint16_t timer_reload = 0;
        
        uint8_t length_counter = 0;
        
        uint8_t output = 0;
        uint8_t sequence_pos = 0;
    } triangle;

    struct NoiseChannel {
        bool enabled = false;
        bool halt = false;
        bool envelope_enabled = false;
        uint8_t volume = 0;
        uint8_t envelope_period = 0;
        uint8_t envelope_decay_count = 0;
        bool envelope_start = false;

        uint16_t timer = 0;
        uint16_t timer_reload = 0;

        bool mode = false; // Loop noise
        uint16_t shift_register = 1;

        uint8_t length_counter = 0;
        uint8_t output = 0;
    } noise;
    
    // Length Counter Lookup Table
    static const uint8_t length_table[32];
};
