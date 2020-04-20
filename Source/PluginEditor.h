/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/** 
*/
class SpectralDistortionAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SpectralDistortionAudioProcessorEditor (SpectralDistortionAudioProcessor&,
        AudioProcessorValueTreeState&); // value tree
    ~SpectralDistortionAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    // missing something like? void sliderValueChanged (Slider* sliderThatWasMoved) override;
    // all slider attachements are in editor.cpp ...

private:
    // G U I  E L E M E N T S
    //FILTER
    LookAndFeel_V4 LFLookAndFeel; // colour coding for the LF controls
    Slider cutoffDialLF;
    Slider resDialLF;

    LookAndFeel_V4 HFLookAndFeel; // colour coding for the HF side
    Slider cutoffDialHF;
    Slider resDialHF;

    //DISTORTION
    ComboBox distSelLF;

    Slider driveDialLF;
    Slider thresDialLF;
    Slider drywetDialLF;
    Slider volDialLF;

    ComboBox distSelHF;

    Slider driveDialHF;
    Slider thresDialHF;
    Slider drywetDialHF;
    Slider volDialHF;

    //P O I N T E R S
    //FILTER
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> cutoffValueLF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> resValueLF;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> cutoffValueHF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> resValueHF;

    //DISTORTION
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> distSelChoiceLF;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> driveValueLF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> thresValueLF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> drywetValueLF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> volValueLF;

    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> distSelChoiceHF;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> driveValueHF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> thresValueHF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> drywetValueHF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> volValueHF;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SpectralDistortionAudioProcessor& processor; // juce default: reference to value tree attachement class in processor
    AudioProcessorValueTreeState& treeState;    // reference variable to treestate

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralDistortionAudioProcessorEditor)
};
