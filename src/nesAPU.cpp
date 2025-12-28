#include "nesAPU.h"

// Length Counter Lookup Table
const uint8_t nesAPU::length_table[] = {
    10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
    12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30
};

nesAPU::nesAPU() {
    reset();
}

nesAPU::~nesAPU() {
}

void nesAPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
    case 0x4000: // Pulse 1 Duty/Volume/Envelope
        pulse1.duty_mode = (data & 0xC0) >> 6;
        pulse1.halt = (data & 0x20);
        pulse1.envelope_enabled = !(data & 0x10);
        pulse1.volume = (data & 0x0F);
        pulse1.envelope_period = (data & 0x0F);
        break;
    case 0x4001: // Pulse 1 Sweep
        pulse1.sweep_enabled = (data & 0x80);
        pulse1.sweep_period = (data & 0x70) >> 4;
        pulse1.sweep_negate = (data & 0x08);
        pulse1.sweep_shift = (data & 0x07);
        pulse1.sweep_reload = true;
        break;
    case 0x4002: // Pulse 1 Timer Low
        pulse1.timer_reload = (pulse1.timer_reload & 0xFF00) | data;
        break;
    case 0x4003: // Pulse 1 Timer High / Length
                pulse1.timer_reload = (pulse1.timer_reload & 0x00FF) | ((uint16_t)(data & 0x07) << 8);
                pulse1.length_counter = length_table[(data & 0xF8) >> 3];
                pulse1.envelope_start = true;
                pulse1.sequence_pos = 0;
                break;
        
            case 0x4004: // Pulse 2 Duty/Volume/Envelope
                pulse2.duty_mode = (data & 0xC0) >> 6;
                pulse2.halt = (data & 0x20);
                pulse2.envelope_enabled = !(data & 0x10);
                pulse2.volume = (data & 0x0F);
                pulse2.envelope_period = (data & 0x0F);
                break;
            case 0x4005: // Pulse 2 Sweep
                pulse2.sweep_enabled = (data & 0x80);
                pulse2.sweep_period = (data & 0x70) >> 4;
                pulse2.sweep_negate = (data & 0x08);
                pulse2.sweep_shift = (data & 0x07);
                pulse2.sweep_reload = true;
                break;
            case 0x4006: // Pulse 2 Timer Low
                pulse2.timer_reload = (pulse2.timer_reload & 0xFF00) | data;
                break;
            case 0x4007: // Pulse 2 Timer High / Length
                pulse2.timer_reload = (pulse2.timer_reload & 0x00FF) | ((uint16_t)(data & 0x07) << 8);
                pulse2.length_counter = length_table[(data & 0xF8) >> 3];
                pulse2.envelope_start = true;
                pulse2.sequence_pos = 0;
                break;
        
            case 0x4008: // Triangle Linear Counter
                triangle.halt = (data & 0x80);
                triangle.linear_counter_reload = (data & 0x7F);
                break;
            case 0x400A: // Triangle Timer Low
                triangle.timer_reload = (triangle.timer_reload & 0xFF00) | data;
                break;
            case 0x400B: // Triangle Timer High / Length
                triangle.timer_reload = (triangle.timer_reload & 0x00FF) | ((uint16_t)(data & 0x07) << 8);
                triangle.length_counter = length_table[(data & 0xF8) >> 3];
                triangle.linear_counter_reload_flag = true;
                break;
                
            case 0x400C: // Noise Volume/Envelope
                noise.halt = (data & 0x20);
                noise.envelope_enabled = !(data & 0x10);
                noise.volume = (data & 0x0F);
                noise.envelope_period = (data & 0x0F);
                break;
            case 0x400E: // Noise Mode/Period
                noise.mode = (data & 0x80);
                {
                    uint16_t noise_periods[] = { 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 };
                    noise.timer_reload = noise_periods[data & 0x0F];
                }
                break;
            case 0x400F: // Noise Length
                noise.length_counter = length_table[(data & 0xF8) >> 3];
                noise.envelope_start = true;
                break;
    case 0x4015: // Status
        pulse1.enabled = (data & 0x01);
        if (!pulse1.enabled) pulse1.length_counter = 0;
        pulse2.enabled = (data & 0x02);
        if (!pulse2.enabled) pulse2.length_counter = 0;
        triangle.enabled = (data & 0x04);
        if (!triangle.enabled) triangle.length_counter = 0;
        noise.enabled = (data & 0x08);
        if (!noise.enabled) noise.length_counter = 0;
        break;

    case 0x4017: // Frame Counter
        frame_counter_mode = (data & 0x80) >> 7;
        irq_inhibit = (data & 0x40);
        // Reset sequencer? (usually yes)
        break;
    }
}

uint8_t nesAPU::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;
    if (addr == 0x4015) {
        if (pulse1.length_counter > 0) data |= 0x01;
        if (pulse2.length_counter > 0) data |= 0x02;
        if (triangle.length_counter > 0) data |= 0x04;
        if (noise.length_counter > 0) data |= 0x08;
    }
    return data;
}

void nesAPU::clock() {
    bool quarter_frame = false;
    bool half_frame = false;

    globalTime++;

    // This is a simplified frame sequencer (approximate)
    // Mode 0: 4-step sequence
    // Mode 1: 5-step sequence
    // Using a simple interval for now roughly corresponding to 240Hz
    if (globalTime % 2 == 0) { // APU clock is CPU / 2 (approx) - actually CPU / 1 for steps, but some things clock slower
        // Pulse 1
        if (pulse1.timer > 0) pulse1.timer--;
        else {
            pulse1.timer = pulse1.timer_reload + 1;
            pulse1.sequence_pos++;
            pulse1.sequence_pos &= 0x07;
        }

        // Pulse 2
        if (pulse2.timer > 0) pulse2.timer--;
        else {
            pulse2.timer = pulse2.timer_reload + 1;
            pulse2.sequence_pos++;
            pulse2.sequence_pos &= 0x07;
        }

        // Noise
        if (noise.timer > 0) noise.timer--;
        else {
            noise.timer = noise.timer_reload;
            uint8_t feedback = (noise.shift_register & 0x01) ^ ((noise.mode ? (noise.shift_register >> 6) : (noise.shift_register >> 1)) & 0x01);
            noise.shift_register >>= 1;
            noise.shift_register |= (feedback << 14);
        }

        // Triangle
        if (triangle.timer > 0) triangle.timer--;
        else {
            triangle.timer = triangle.timer_reload;
            if (triangle.linear_counter > 0 && triangle.length_counter > 0) {
                 triangle.sequence_pos++;
                 triangle.sequence_pos &= 0x1F;
            }
        }
    }

    // Frame Counter Logic (Simplified 240Hz roughly)
    // Real NES runs at ~1.79MHz, APU frame counter ~240Hz.
    // If we call clock() every CPU cycle (which we should from bus), we divide.
    // Assuming clock() is called at CPU rate (~1.789773 MHz)
    // 7457 cycles ~= 240Hz

    // Using a modulo for simplicity in this prototype
    if ((globalTime / 7457) > frame_counter_mode) {
        // Just driving it by raw counter for now to get sound out
        // Proper sequencer implementation needs state machine
    }

    // Hacky 240Hz / 120Hz triggers
    if (globalTime % 3728 == 0) { // Approx quarter frame
        quarter_frame = true;
    }
    if (globalTime % 7456 == 0) { // Approx half frame
        half_frame = true;
    }

    if (quarter_frame) {
        // Update Envelopes
        auto UpdateEnvelope = [](auto& chan) {
            if (chan.envelope_start) {
                chan.envelope_start = false;
                chan.envelope_decay_count = 15;
                chan.envelope_divider_counter = chan.envelope_period;
            } else {
                if (chan.envelope_divider_counter > 0) {
                    chan.envelope_divider_counter--;
                } else {
                    chan.envelope_divider_counter = chan.envelope_period;
                    if (chan.envelope_decay_count > 0) {
                        chan.envelope_decay_count--;
                    } else if (chan.halt) {
                        chan.envelope_decay_count = 15;
                    }
                }
            }
        };
        UpdateEnvelope(pulse1);
        UpdateEnvelope(pulse2);
        UpdateEnvelope(noise);

        // Update Triangle Linear Counter
        if (triangle.linear_counter_reload_flag) {
            triangle.linear_counter = triangle.linear_counter_reload;
        } else if (triangle.linear_counter > 0) {
            triangle.linear_counter--;
        }
        if (!triangle.halt) triangle.linear_counter_reload_flag = false;
    }

    if (half_frame) {
        // Update Length Counters
        auto UpdateLength = [](auto& chan) {
            if (!chan.halt && chan.length_counter > 0) {
                chan.length_counter--;
            }
        };
        UpdateLength(pulse1);
        UpdateLength(pulse2);
        UpdateLength(triangle);
        UpdateLength(noise);

        // Sweep Logic
        auto UpdateSweep = [](auto& chan) {
             if (chan.sweep_reload) {
                 if (chan.sweep_enabled && chan.sweep_timer == 0) {
                    // Apply sweep
                 }
                 chan.sweep_timer = chan.sweep_period;
                 chan.sweep_reload = false;
             } else if (chan.sweep_timer > 0) {
                 chan.sweep_timer--;
             } else {
                 chan.sweep_timer = chan.sweep_period;
                 if (chan.sweep_enabled) {
                      uint16_t change = chan.timer_reload >> chan.sweep_shift;
                      if (chan.sweep_negate) {
                          chan.timer_reload -= change;
                          if (chan.timer_reload < 8) chan.timer_reload = 0; // mute
                      } else {
                          chan.timer_reload += change;
                      }
                 }
             }
        };
        UpdateSweep(pulse1);
        UpdateSweep(pulse2);
    }
}

void nesAPU::reset() {
    globalTime = 0;
    pulse1.enabled = false;
    pulse2.enabled = false;
    triangle.enabled = false;
    noise.enabled = false;
    noise.shift_register = 1;
}

double nesAPU::GetOutputSample() {
    double p1_out = 0, p2_out = 0, t_out = 0, n_out = 0;

    auto PulseSample = [](uint8_t sequence, uint8_t duty) {
        static const uint8_t sequences[4][8] = {
            {0, 1, 0, 0, 0, 0, 0, 0}, // 12.5%
            {0, 1, 1, 0, 0, 0, 0, 0}, // 25%
            {0, 1, 1, 1, 1, 0, 0, 0}, // 50%
            {1, 0, 0, 1, 1, 1, 1, 1}  // 25% negated
        };
        return sequences[duty][sequence];
    };

    if (pulse1.enabled && pulse1.timer_reload >= 8 && pulse1.length_counter > 0) {
        if (PulseSample(pulse1.sequence_pos, pulse1.duty_mode)) {
            p1_out = (double)(pulse1.envelope_enabled ? pulse1.envelope_decay_count : pulse1.volume);
            // std::cout << "p1_out: " << p1_out << std::endl;
        }
    }

    if (pulse2.enabled && pulse2.timer_reload >= 8 && pulse2.length_counter > 0) {
        if (PulseSample(pulse2.sequence_pos, pulse2.duty_mode)) {
            p2_out = (double)(pulse2.envelope_enabled ? pulse2.envelope_decay_count : pulse2.volume);
        }
    }

    if (triangle.enabled && triangle.length_counter > 0 && triangle.linear_counter > 0) {
         // Triangle wave is 32 steps: 15, 14, ... 0, 1, ... 15
         // sequence_pos 0-31
         uint8_t s = triangle.sequence_pos;
         t_out = (double)(s < 16 ? (15 - s) : (s - 16));
    }

    if (noise.enabled && noise.length_counter > 0) {
        if (!(noise.shift_register & 0x01)) {
            n_out = (double)(noise.envelope_enabled ? noise.envelope_decay_count : noise.volume);
        }
    }

    double pulse_total = p1_out + p2_out;
    double pulse_out = pulse_total > 0 ? (95.88 / ((8128.0 / pulse_total) + 100.0)) : 0.0;

    double tnd_total = (t_out / 8227.0) + (n_out / 12241.0);
    double tnd_out = tnd_total > 0 ? (159.79 / ((1.0 / tnd_total) + 100.0)) : 0.0;

    return (pulse_out + tnd_out);
}
