/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectralDistortionAudioProcessorEditor::SpectralDistortionAudioProcessorEditor 
(SpectralDistortionAudioProcessor& p, AudioProcessorValueTreeState& vts) // added value tree state (vts) here
    : AudioProcessorEditor (&p), processor (p), treeState(vts)          // .. and here ->links processor vts to editor vts reference 
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //was: 400, 300
    //setSize (450, 600); //increased size but looks the same in reaper/clipped (alas .. this always wasn't working (there's online discussions about this issue) and ...size is ok in standalone)
 

    // solution to at least see the full size when dragging it out from https://forum.juce.com/t/vst3-resizing-issue-in-reaper/31051/11
    setResizable(true, true);
    setResizeLimits(450, 600, 600, 800); // ... now factor 1.3 on second sizes (which is not needed, it should be w 450, h 600) 
    //(above is not neccessary as all gui positions inthe editor are coded for 450x600, but it seems to prevent the bug that the plugin doesn't expand to that size when opened in reaper
    //it used to be clipped and stayed that way when dragged out, it now shows the stuff that used to be clipped)
    getConstrainer()->setFixedAspectRatio(0.75); //450/600 = 0.75, right?
    setSize(450, 600);

    //global look and feels
    LFLookAndFeel.setColour(Slider::ColourIds::thumbColourId, Colours::blueviolet); 
    //LFLookAndFeel.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::orangered);
    HFLookAndFeel.setColour(Slider::ColourIds::thumbColourId, Colours::darkmagenta);
    //HFLookAndFeel.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::indianred);

    // F I L T E R 
    // Cutoff Frequency LPF
    cutoffValueLF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "cutoffLF", cutoffDialLF);
    cutoffDialLF.setSliderStyle(Slider::RotaryVerticalDrag);
    cutoffDialLF.setRange(20.0f, 5000.0f, 0.01f); // interval shan't be 0.0f
    cutoffDialLF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    cutoffDialLF.setLookAndFeel(&LFLookAndFeel);
    addAndMakeVisible(&cutoffDialLF);

    // Resonance LPF
    resValueLF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "resLF", resDialLF);
    resDialLF.setSliderStyle(Slider::RotaryVerticalDrag);
    resDialLF.setRange(0.0f, 5.0f, 0.01f); //(20.0f, 20000.0f, 0.01f), nononono -- this just range for dial... value range in processor (but guess easier if ranges agree 1:1)
    resDialLF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    resDialLF.setLookAndFeel(&LFLookAndFeel);
    addAndMakeVisible(&resDialLF);


    // Cutoff Frequency HPF
    cutoffValueHF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "cutoffHF", cutoffDialHF);
    cutoffDialHF.setSliderStyle(Slider::RotaryVerticalDrag);
    cutoffDialHF.setRange(500.0f, 10000.0f, 0.01f);
    cutoffDialHF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    cutoffDialHF.setLookAndFeel(&HFLookAndFeel);
    addAndMakeVisible(&cutoffDialHF);

    // Resonance HPF
    resValueHF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "resHF", resDialHF);
    resDialHF.setSliderStyle(Slider::RotaryVerticalDrag);
    resDialHF.setRange(0.0f, 5.0f, 0.01f);
    resDialHF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    resDialHF.setLookAndFeel(&HFLookAndFeel);
    addAndMakeVisible(&resDialHF);


    // D I S T O R T I O N
    // Distortion Type Selection LF
    distSelChoiceLF = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(treeState,
        "distortionTypeLF", distSelLF);
    distSelLF.addItem("Hard Clipping", 1); // item id 1, but selecting in processor index is starting with 0
    distSelLF.addItem("Soft Clipping", 2);
    distSelLF.addItem("Half Wave",     3);
    distSelLF.addItem("Overdrive",     4);
    distSelLF.addItem("None",          5); // default: don't apply distortion
    addAndMakeVisible(&distSelLF);

    // Drive LF
    driveValueLF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "driveLF", driveDialLF);
    driveDialLF.setSliderStyle(Slider::RotaryVerticalDrag);
    driveDialLF.setRange(1.0f, 20.0f, 0.1f);  
    driveDialLF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    driveDialLF.setLookAndFeel(&LFLookAndFeel);
    addAndMakeVisible(&driveDialLF);

    // Threshold LF
    thresValueLF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "thresLF", thresDialLF);
    thresDialLF.setSliderStyle(Slider::RotaryVerticalDrag);
    thresDialLF.setRange(0.0f, 1.0f, 0.005f); 
    thresDialLF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    thresDialLF.setLookAndFeel(&LFLookAndFeel);
    addAndMakeVisible(&thresDialLF);

    // Dry/Wet LF
    drywetValueLF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "drywetLF", drywetDialLF);
    drywetDialLF.setSliderStyle(Slider::RotaryVerticalDrag);
    drywetDialLF.setRange(0.00f, 1.0f, 0.005f);
    drywetDialLF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    drywetDialLF.setLookAndFeel(&LFLookAndFeel);
    addAndMakeVisible(&drywetDialLF);

    // Volume LF
    volValueLF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "volLF", volDialLF);
    volDialLF.setSliderStyle(Slider::RotaryVerticalDrag);
    volDialLF.setRange(0.0f, 5.0f, 0.005f);    // might be a bit much but will show it works (so i hope)
    volDialLF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volDialLF.setLookAndFeel(&LFLookAndFeel);
    addAndMakeVisible(&volDialLF);


    // HIGHS
    // Distortion Type Selection HF
    distSelChoiceHF = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(treeState,
        "distortionTypeHF", distSelHF);
    distSelHF.addItem("Hard Clipping", 1);
    distSelHF.addItem("Soft Clipping", 2);
    distSelHF.addItem("Half Wave",     3);
    distSelHF.addItem("Overdrive",     4);
    distSelHF.addItem("None",          5);
    addAndMakeVisible(&distSelHF);

    // Drive HF
    driveValueHF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "driveHF", driveDialHF);
    driveDialHF.setSliderStyle(Slider::RotaryVerticalDrag);
    driveDialHF.setRange(1.0f, 20.0f, 0.1f);
    driveDialHF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    driveDialHF.setLookAndFeel(&HFLookAndFeel);
    addAndMakeVisible(&driveDialHF);

    // Threshold HF
    thresValueHF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "thresHF", thresDialHF);
    thresDialHF.setSliderStyle(Slider::RotaryVerticalDrag);
    thresDialHF.setRange(0.0f, 1.0f, 0.005f); //(-1.0f, 0.0f, 0.05f);
    thresDialHF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    thresDialHF.setLookAndFeel(&HFLookAndFeel);
    addAndMakeVisible(&thresDialHF);

    // Dry/Wet HF
    drywetValueHF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "drywetHF", drywetDialHF);
    drywetDialHF.setSliderStyle(Slider::RotaryVerticalDrag);
    drywetDialHF.setRange(0.0f, 1.0f, 0.005f);
    drywetDialHF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    drywetDialHF.setLookAndFeel(&HFLookAndFeel);
    addAndMakeVisible(&drywetDialHF);

    // Volume HF
    volValueHF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(treeState,
        "volHF", volDialHF);
    volDialHF.setSliderStyle(Slider::RotaryVerticalDrag);
    volDialHF.setRange(0.0f, 5.0f, 0.005f);
    volDialHF.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volDialHF.setLookAndFeel(&HFLookAndFeel);
    addAndMakeVisible(&volDialHF);

}

SpectralDistortionAudioProcessorEditor::~SpectralDistortionAudioProcessorEditor()
{
    // destructor bit goes here (heard somewhere it destructs from bottom up..?)
}

//==============================================================================
void SpectralDistortionAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // ... must? could be nice to have a partly opaque component floating about, maybe not distortion though ;)
    g.fillAll(Colours::lightcoral);
    g.setColour(Colours::orangered);
    g.drawRoundedRectangle(20, 20, 410, 560, 20, 10);
    //g.fillRoundedRectangle(Colours::transparentWhite);
    //g.setGradientFill(ColourGradient(Colours::transparentWhite, 300, 400, Colours::maroon, 300, 400.5f, true));
       /* static_cast<float> ((getWidth() / 2)), static_cast<float> ((getHeight() / 2)),
        Colours::maroon,
        static_cast<float> (getWidth()), 0.0f,
        true));*/

    // Headlines
    g.setFont(28.0f);                                        
    g.setColour(Colours::black);       //x   y  width height         number of lines & scale
    g.drawFittedText("S P E C T R A L  D I S T O R T I O N", 65 , 35, 320, 28, Justification::left, 1, 0.2f); //x 125

    g.setFont(17.0f);
    g.setColour(Colours::blueviolet);
    g.drawFittedText("LOW PASS",                 90, 70, 100, 17, Justification::left, 1, 0.0f); // 100, 70, 100, 17 LOWS
    g.setColour(Colours::darkmagenta);
    g.drawFittedText("HIGH PASS",               300, 70, 100, 17, Justification::left, 1, 0.0f); // 315, 70, 100, 17, HIGHS

    // Filter Dial Labels
    g.setFont(15.0f);           //  x   y   w   h
    g.setColour(Colours::blueviolet);
    g.drawFittedText("CUT-OFF",    40, 180, 100, 16,  Justification::left, 2, 0.0f);
    g.drawFittedText("RESONANCE", 132, 180, 100, 16,  Justification::left, 2, 0.0f);
    g.setColour(Colours::darkmagenta);
    g.setFont(15.0f);           //  x   y   w   h
    g.drawFittedText("CUT OFF",   255, 180, 100, 16,  Justification::left, 2, 0.0f);
    g.drawFittedText("RESONANCE", 340, 180, 100, 16,  Justification::left, 2, 0.0f);

    // Distortion Dial Labels
    g.setColour(Colours::blueviolet);
    g.drawFittedText("DRIVE",      45, 350, 100, 16, Justification::left, 2, 0.0f);
    g.drawFittedText("THRESHOLD", 135, 350, 100, 16, Justification::left, 2, 0.0f);   
    g.setColour(Colours::darkmagenta);
    g.drawFittedText("DRIVE",     260, 350, 100, 16, Justification::left, 2, 0.0f);
    g.drawFittedText("THRESHOLD", 340, 350, 100, 16, Justification::left, 2, 0.0f);

    g.setColour(Colours::blueviolet);
    g.drawFittedText("DRY/WET",    40, 460, 100, 16, Justification::left, 2, 0.0f);
    g.drawFittedText("VOLUME",    145, 460, 100, 16, Justification::left, 2, 0.0f);
    g.setColour(Colours::darkmagenta);
    g.drawFittedText("DRY/WET",   255, 460, 100, 16, Justification::left, 2, 0.0f); //255, 460, 400, 16
    g.drawFittedText("VOLUME",    355, 460, 100, 16, Justification::left, 2, 0.0f);

    // Volume warning 
    g.setColour(Colours::black);
    g.drawFittedText("Howdy! Extreme settings may harm your speakers.", 40, 500, 400, 20, Justification::left, 2, 0.0f);
    g.drawFittedText("Hence there's volume knobs for piece of mind.", 40, 530, 400, 20, Justification::left, 2, 0.0f);

    /*
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Bye Bye World!", getLocalBounds(), Justification::centred, 1);
    */
}

void SpectralDistortionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // F I L T E R
    cutoffDialLF.setBounds  (20, 90, 100, 100);
    resDialLF.setBounds    (120, 90, 100, 100);

    cutoffDialHF.setBounds (235, 90, 100, 100);
    resDialHF.setBounds    (330, 90, 100, 100);

    // D I S T O R T I O N
    distSelLF.setBounds      (40, 225, 150, 25);
    distSelHF.setBounds     (265, 225, 150, 25);

    driveDialLF.setBounds  (20, 255, 100, 100);
    thresDialLF.setBounds (120, 255, 100, 100);
    drywetDialLF.setBounds (20, 365, 100, 100);
    volDialLF.setBounds   (120, 365, 100, 100);

    driveDialHF.setBounds (235, 255, 100, 100);
    thresDialHF.setBounds (330, 255, 100, 100);
    drywetDialHF.setBounds(235, 365, 100, 100);
    volDialHF.setBounds   (330, 365, 100, 100);

}
