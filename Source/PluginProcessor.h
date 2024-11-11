#pragma once
#include <JuceHeader.h>
#include "Oscillator.h" 

class SynthVoice : public juce::SynthesiserVoice {
public:
    SynthVoice() {
        osc.setSampleRate(44100.0f);
    }

    bool canPlaySound(juce::SynthesiserSound*) override { return true; }

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override {
        isPlaying = true;
        float frequency = 440.0f * std::pow(2.0f, (midiNoteNumber - 69.0f) / 12.0f);
        osc.setFrequency(frequency);
        level = velocity;
    }

    void stopNote(float velocity, bool allowTailOff) override {
        isPlaying = false;
    }

    void pitchWheelMoved(int value) override {}
    void controllerMoved(int controllerNumber, int newValue) override {}

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        if (isPlaying) {
            for (int sample = 0; sample < numSamples; ++sample) {
                float currentSample = osc.getNextSample() * level;

                for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
                    outputBuffer.addSample(channel, startSample + sample, currentSample);
                }
            }
        }
    }

    Oscillator& getOscillator() { return osc; }
    void setSampleRate(float sampleRate) {
        osc.setSampleRate(sampleRate);
    }

private:
    Oscillator osc;
    float level = 0.0f;
    bool isPlaying = false;
};

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class BasicSynthAudioProcessor : public juce::AudioProcessor {
public:
    BasicSynthAudioProcessor();
    ~BasicSynthAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    bool hasEditor() const override;
    juce::AudioProcessorEditor* createEditor() override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    juce::Synthesiser synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BasicSynthAudioProcessor)
};