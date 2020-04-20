/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "../../Maximilian/src/maximilian.h" // (for exmple)
#include "LFdspDistortion.h"
//#include "LFdspDistortion.cpp"

//==============================================================================
/**
*/
class SpectralDistortionAudioProcessor  : public AudioProcessor,
    public AudioProcessorValueTreeState::Listener        //declares Listener (to the broadcast of controls used in editor) -> processor will inherit from the AudioProcessorValueTreeState::Listener     
{
public:
    //==============================================================================
    SpectralDistortionAudioProcessor();
    ~SpectralDistortionAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override; // samplesPerBlock double elsewhere?
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // get valuetree state as 'pass by reference' (not by copy, reference more efficient)
    // AudioProcessorValueTreeState& getState(); // not used?

    // parameterchanged function  
    //void parameterChanged(const String& parameterID, float newValue) override; // remove for now? (this caused issues with new instance of plugin at AudioProcessor* JUCE_CALLTYPE createPluginFilter()

    // updateFilter function 
    void updateFilter(); 

    // declare vts (not sure if better in puclic or private; some examples declare it here - e.g. AP (https://www.youtube.com/watch?v=lm5BxqXJ4mo)
    AudioProcessorValueTreeState treeState; 

//=======================================================================================================
private:

    double lastSampleRate; 

    // buffers for high and low pass filter
    AudioSampleBuffer LPBuffer;
    AudioSampleBuffer HPBuffer;

    // trying to solve cracke problem - needs dsp function and rotating through channels
    LFdspDistortion LFdspDistortion;
    static const int kChannels = 2;
    float LFout[kChannels];
    float HFout[kChannels];

    // coefficients for the high and low pass filter
    IIRCoefficients LPCoefficients;
    IIRCoefficients HPCoefficients;

    // trying cascaded IIR appraoch for Linkwitz-Riely (flattest response)
    IIRFilter LPFilter1[2]; //can use {} for audio tags..
    IIRFilter LPFilter2[2];
    IIRFilter HPFilter1[2];
    IIRFilter HPFilter2[2];

    // the audioprogrammer's method: duplicate for stereo as dsp's IIRs are mono -- not sure about this (source https://www.youtube.com/watch?v=lWOJhuCDQqo)	

    dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients <float>> lowPassFilter; // l or 2??
    dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients <float>> highPassFilter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralDistortionAudioProcessor)
};
