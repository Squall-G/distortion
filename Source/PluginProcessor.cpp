/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessor::DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
audioTree(*this, nullptr, Identifier("PARAMETERS"),
    {std::make_unique<AudioParameterFloat>("inputGain","InputGain",NormalisableRange<float>(1,48,0.01),0),
     std::make_unique<AudioParameterFloat>("outputGain","OutputGain",NormalisableRange<float>(-48,12,0.01),0),
     std::make_unique<AudioParameterFloat>("toneControl","ToneControl",NormalisableRange<float>(20,20000,1),20000),
     std::make_unique<AudioParameterFloat>("distortionType","DistortionType",NormalisableRange<float>(1,5,1),1),
     std::make_unique<AudioParameterInt>("switchTone", "SwitchTone", 0, 1, 0)
    }),
lowPassFilter(dsp::IIR::Coefficients<float>::makeLowPass((44100*4), 20000.0))
#endif
{
    audioTree.addParameterListener("inputGain", this);
    audioTree.addParameterListener("outputGain", this);
    audioTree.addParameterListener("toneControl", this);
    audioTree.addParameterListener("distortionType", this);
    audioTree.addParameterListener("switchTone", this);
    
    outputGainValue = *audioTree.getRawParameterValue("outputGain");
    gainSmoothed = outputGainValue;
    
    inputGainValue = 1.0;
    outputGainValue = 1.0;
    toneControlValue = 20000;

    distortionTypeValue = 1;
    
    sampling.reset (new dsp::Oversampling<float> (2, 2, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, false));
}

DistortionAudioProcessor::~DistortionAudioProcessor()
{
    sampling.reset();
}

//==============================================================================
const String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate=sampleRate;
    
    dsp::ProcessSpec spec;
    //Sample Rate of the filter must be 4 times because the Oversampling
    spec.sampleRate = sampleRate*4;
    spec.maximumBlockSize = samplesPerBlock*3;
    spec.numChannels = getTotalNumOutputChannels();

    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
    
    sampling->reset();
    sampling->initProcessing(static_cast<size_t> (samplesPerBlock));
}

void DistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistortionAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == "inputGain")
    {
        inputGainValue = Decibels::decibelsToGain(newValue);
    }
    else if (parameterID == "outputGain")
    {
        outputGainValue = Decibels::decibelsToGain(newValue);
    }
    else if (parameterID == "toneControl")
    {
        toneControlValue = newValue;
        //needsUpdate=true;
    }
    else if (parameterID == "distortionType")
    {
        distortionTypeValue = newValue;
    }
    else if (parameterID == "switchTone")
    {
        switchToneValue = newValue;
    }
}

void DistortionAudioProcessor::updateParameters()
{
    //be sure to update params for your dsp here
    float frequency = 44100 * 4;
    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(frequency, toneControlValue);
    needsUpdate=false;
}

void DistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //data structure that points to the buffer
    dsp::AudioBlock<float> blockInput (buffer);
    dsp::AudioBlock<float> blockOutput = sampling->processSamplesUp(blockInput);
    
    if (switchToneValue==0)
        lowPassFilter.process(dsp::ProcessContextReplacing<float> (blockOutput));
    
    for (int channel = 0; channel < blockOutput.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < blockOutput.getNumSamples(); ++sample)
        {
            float in = blockOutput.getSample(channel, sample);
            in *= inputGainValue;
            
            float out;
            
            // Apply distortion based on type
            if(distortionTypeValue == 1)
            {
            // Simple hard clipping
                float threshold = 1.0f;
                if(in > threshold)
                    out = threshold;
                else if(in < -threshold)
                    out = -threshold;
                else
                    out = in;
            }
            else if(distortionTypeValue == 2)
            {
            // Soft clipping based on quadratic function
                float threshold1 = 1.0f/3.0f;
                float threshold2 = 2.0f/3.0f;
                if(in > threshold2)
                    out = 1.0f;
                else if(in > threshold1)
                    out = (3.0f - (2.0f - 3.0f*in) *
                                  (2.0f - 3.0f*in))/3.0f;
                else if(in < -threshold2)
                    out = -1.0f;
                else if(in < -threshold1)
                    out = -(3.0f - (2.0f + 3.0f*in) *
                                   (2.0f + 3.0f*in))/3.0f;
                else
                    out = 2.0f* in;
            }
            else if(distortionTypeValue == 3)
            {
            // Soft clipping based on exponential function
                if(in > 0)
                    out = 1.0f - expf(-in);
                else
                    out = -1.0f + expf(in);
            }
            else if(distortionTypeValue == 4)
            {
            // Full-wave rectifier (absolute value)
                out = fabsf(in);
            }
            else if(distortionTypeValue == 5)
            {
            // Half-wave rectifier
                if(in > 0)
                    out = in;
                else
                    out = 0;
            }
            out *= 1/(log(inputGainValue+1)+1);
            //out /= inputGainValue;
            
            gainSmoothed=gainSmoothed-0.004*(gainSmoothed-outputGainValue);
            out *= gainSmoothed;

            blockOutput.setSample(channel, sample, out);
        }
    }
    //lowPassFilter
    //if(needsUpdate)
    updateParameters();
    
    if (switchToneValue==1)
        lowPassFilter.process(dsp::ProcessContextReplacing<float> (blockOutput));
    
    sampling->processSamplesDown(blockInput);

}

//==============================================================================
bool DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this, audioTree);
}

//==============================================================================
void DistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state=audioTree.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr)
        if (xml->hasTagName (audioTree.state.getType()))
            audioTree.replaceState (ValueTree::fromXml (*xml));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}
