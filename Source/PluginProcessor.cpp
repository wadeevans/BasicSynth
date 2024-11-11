#include "PluginProcessor.h"

//==============================================================================
BasicSynthAudioProcessor::BasicSynthAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < 16; ++i) {
        synth.addVoice(new SynthVoice());
    }
}

BasicSynthAudioProcessor::~BasicSynthAudioProcessor()
{
}

//==============================================================================
const juce::String BasicSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicSynthAudioProcessor::acceptsMidi() const
{
    return true;
}

bool BasicSynthAudioProcessor::producesMidi() const
{
    return false;
}

bool BasicSynthAudioProcessor::isMidiEffect() const
{
    return false;
}

double BasicSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int BasicSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicSynthAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String BasicSynthAudioProcessor::getProgramName(int index)
{
    return {};
}

void BasicSynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void BasicSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->setSampleRate(static_cast<float>(sampleRate));
        }
    }
}

void BasicSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool BasicSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Only mono or stereo is supported
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void BasicSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool BasicSynthAudioProcessor::hasEditor() const
{
    return false; // For now, we don't have a custom editor
}

juce::AudioProcessorEditor* BasicSynthAudioProcessor::createEditor()
{
    return nullptr; // For now, we don't have a custom editor
}

//==============================================================================
void BasicSynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use it to save the current plugin state:
    /*
    auto state = params.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
    */
}

void BasicSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block.
    // Here's an example of how you can use it to restore the plugin state:
    /*
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(params.state.getType()))
            params.replaceState(juce::ValueTree::fromXml(*xmlState));
    */
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicSynthAudioProcessor();
}