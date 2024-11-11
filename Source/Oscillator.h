#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Oscillator {
public:
    enum class WaveType {
        Sine,
        Square,
        Saw,
        Triangle
    };

    Oscillator() : phase(0.0f), frequency(440.0f), sampleRate(44100.0f), type(WaveType::Sine) {}

    void setFrequency(float freq) {
        frequency = freq;
    }

    void setSampleRate(float sr) {
        sampleRate = sr;
    }

    void setWaveType(WaveType waveType) {
        type = waveType;
    }

    float getNextSample() {
        constexpr float TWO_PI = 2.0f * static_cast<float>(M_PI);
        float sample = 0.0f;

        switch (type) {
        case WaveType::Sine:
            sample = std::sin(TWO_PI * phase);
            break;

        case WaveType::Square:
            sample = phase < 0.5f ? 1.0f : -1.0f;
            break;

        case WaveType::Saw:
            sample = 2.0f * phase - 1.0f;
            break;

        case WaveType::Triangle:
            sample = phase < 0.5f ?
                4.0f * phase - 1.0f :
                3.0f - 4.0f * phase;
            break;
        }

        // Increment and wrap phase
        phase += frequency / sampleRate;
        if (phase >= 1.0f)
            phase -= 1.0f;

        return sample;
    }

private:
    float phase;      // Current phase [0, 1]
    float frequency;  // Frequency in Hz
    float sampleRate; // Sample rate in Hz
    WaveType type;   // Current waveform type
};