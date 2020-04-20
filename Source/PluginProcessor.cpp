/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "LFdspDistortion.h"

//==============================================================================
// C O N S T R U C T O R
SpectralDistortionAudioProcessor::SpectralDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),

    // end of juce default/buses, 
    // now: initialisation of all apvts parameters       
            // Joshua Hodge aka the Audioprogrammer (tutorial 63) does this with a snazzy function: createParameters()    
            // AP(juce tutorial 44) more helpful here, idea of using #define in .h instead of entering each string  

    treeState(*this, nullptr, Identifier("PARAMETERS"),  
        {  ///  make_unique allocates memory >> param ID, param name(display), min, max, default
          std::make_unique<AudioParameterFloat>("cutoffLF", "LPF Cutoff", NormalisableRange<float>(20.0f, 5000.0f), 2500.0f), 
          std::make_unique<AudioParameterFloat>("resLF", "LPF Resonance", 0.01f, 5.0f, 0.5f),                                 
          std::make_unique<AudioParameterChoice>("distortionTypeLF", "LPF Distortion Type", StringArray("Hard Clipping", "Soft Clipping",
          "Half-Wave", "Overdrive", "None"), 4),                                                                              
          std::make_unique<AudioParameterFloat>("driveLF", "LPF Drive", 1.0f, 20.0f, 1.0f),   
          std::make_unique<AudioParameterFloat>("thresLF", "LPF Threshold", 0.01f, 1.0f, 0.01f), 
          std::make_unique<AudioParameterFloat>("drywetLF", "LPF Dry-Wet", 0.0f, 1.0f, 0.5f),    
          std::make_unique<AudioParameterFloat>("volLF", "LPF Volume", 0.0f, 5.0f, 1.0f),        

          std::make_unique<AudioParameterFloat>("cutoffHF", "HPF Cutoff", NormalisableRange<float>(500.0f, 10000.0f), 800.0f),
          std::make_unique<AudioParameterFloat>("resHF", "HPF Resonance", 0.01f, 5.0f, 0.5f),
          std::make_unique<AudioParameterChoice>("distortionTypeHF", "HPF Distortion Type", StringArray("Hard Clipping", "Soft Clipping",
          "Half-Wave", "Overdrive", "None"), 4),
          std::make_unique<AudioParameterFloat>("driveHF", "HPF Drive", 1.0f, 20.0f, 1.0f),
          std::make_unique<AudioParameterFloat>("thresHF", "HPF Threshold", 0.01f, 1.0f, 0.01f), 
          std::make_unique<AudioParameterFloat>("drywetHF", "HPF Dry-Wet", 0.0f, 1.0f, 0.5f),
          std::make_unique<AudioParameterFloat>("volHF", "HPF Volume", NormalisableRange<float>(0.0f, 5.0f), 1.0f),
        }), 

         // declare & initialise filter, incl. what type of filter it shall be (there's 2 filters per each though?)
         lowPassFilter(dsp::IIR::Coefficients <float>::makeLowPass(lastSampleRate, 20000.0f, 0.1f)),   
         highPassFilter(dsp::IIR::Coefficients <float>::makeHighPass(lastSampleRate, 20000.0f, 0.1f))   
{
#endif
    
        // Declare Listeners for each parameter in vts array
        const StringArray params = { "cutoffLF", "resLF", "distortionTypeLF", "driveLF", "thresLF", "drywetLF", "volLF",
                                     "cutoffHF", "resHF", "distortionTypeHF", "driveHF", "thresHF", "drywetHF", "volHF" };
        for (int i = 0; i <= 13; ++i) 
        {
            treeState.addParameterListener(params[i], this);
        }

}


SpectralDistortionAudioProcessor::~SpectralDistortionAudioProcessor()
{

}

//==============================================================================
const String SpectralDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpectralDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpectralDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpectralDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpectralDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpectralDistortionAudioProcessor::getNumPrograms()
{
    return 1;
}
               

int SpectralDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpectralDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String SpectralDistortionAudioProcessor::getProgramName (int index)
{
    return {"Spectral Distortion v1.0"}; // hello world in a lockdown ;)
}

void SpectralDistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
   
}

//==============================================================================
void SpectralDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    lastSampleRate = sampleRate; 

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    lowPassFilter.prepare(spec);
    lowPassFilter.reset(); 

    highPassFilter.prepare(spec);
    highPassFilter.reset();

 
    
    // F I L T E R   C O E F F I C I E N T S
    // calcualte coefficients..
    LPCoefficients = IIRCoefficients::makeLowPass(sampleRate, *treeState.getRawParameterValue("cutoffLF"), *treeState.getRawParameterValue("resLF")); //wants a <double> frequency for LP filter.. and/or Q (there's both in the juce documentation, gees)
    HPCoefficients = IIRCoefficients::makeHighPass(sampleRate, *treeState.getRawParameterValue("cutoffHF"), *treeState.getRawParameterValue("resHF"));

    //now, set-up the coefficients.. (or write in 1 line: 'filtername'.setCoefficients(coefficients.makeLowPass(sampleRate, cutoff frequency etc)
    LPFilter1[0].setCoefficients(LPCoefficients);
    LPFilter2[0].setCoefficients(LPCoefficients);
    LPFilter1[1].setCoefficients(LPCoefficients);
    LPFilter2[1].setCoefficients(LPCoefficients);

    HPFilter1[0].setCoefficients(HPCoefficients);
    HPFilter2[0].setCoefficients(HPCoefficients);
    HPFilter1[1].setCoefficients(HPCoefficients);
    HPFilter2[1].setCoefficients(HPCoefficients);

    // buffer size set-up, stereo - 2 channels each
    LPBuffer.setSize(2, samplesPerBlock);
    HPBuffer.setSize(2, samplesPerBlock);

    // clear buffers
    LPBuffer.clear();
    HPBuffer.clear();

    for(int i = 0; i < kChannels; i++)
    {
        LFout[i] = 0.0f;
        HFout[i] = 0.0f;
    }
}

void SpectralDistortionAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpectralDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void SpectralDistortionAudioProcessor::updateFilter()
{
    // making variables from tree ref used for updating coeffs
    float freqLF = *treeState.getRawParameterValue("cutoffLF"); 
    float resLF = *treeState.getRawParameterValue("resLF");

    float freqHF = *treeState.getRawParameterValue("cutoffHF");
    float resHF = *treeState.getRawParameterValue("resHF");

    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freqLF, resLF); 
    *highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate,freqHF, resHF); 
}

// P R O C E S S   B L O C K 
void SpectralDistortionAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) //passed by & reference from function in header
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel) // outer loop - going through input channels
    {
        // Copy samples into temp low pass and high pass buffers ready for filtering
        LPBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());
        HPBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());

        dsp::AudioBlock<float> LFblock(LPBuffer);
        dsp::AudioBlock<float> HFblock(HPBuffer);
        updateFilter();
        lowPassFilter.process(dsp::ProcessContextReplacing <float>(LFblock)); 
        highPassFilter.process(dsp::ProcessContextReplacing <float>(HFblock));

        LPFilter1[channel].reset(); //trying resets here... it still crackles, no avail
        HPFilter1[channel].reset();
        LPFilter2[channel].reset();
        HPFilter2[channel].reset();


        ///// The Problem ///////////////////

        // it crackles!!
        // possible reason - last sample of channels (i.e. channel 0) gets processed in inner for loop, while outer for loop proceeds to next channel (i.e. channel 1), so the last sample ends up in teh wrong channel
        // Juce forum: https://forum.juce.com/t/1-most-common-programming-mistake-that-we-see-on-the-forum/26013
        // solution is difficult to implement now (this should have been done initially), following Josh H, AP tutorial 27 https://www.youtube.com/watch?v=sn9DoNwKK7M
        // after creating external dsp function, i have a problem with instatialting the SpectralDistortionAudioProcessor class itself...
        // currently the error message referring to line 519 reads 'object of abstract class type 'SpectralDistortionAudioProcessor ' is not allowed


        // now,  D I S T O R T I O N

        auto* LFchannelData = LPBuffer.getWritePointer(channel); 
        auto* HFchannelData = HPBuffer.getWritePointer(channel);

        auto* outputLF = LPBuffer.getWritePointer(channel);
        

        // inner for-loop for LOW FREQUENCIES
        for (int i = 0; i < LPBuffer.getNumSamples(); i++) 
        {
            auto LFin = LFchannelData[i]; 
            //float LFout;
            auto LFchoice = *treeState.getRawParameterValue("distortionTypeLF"); // actually an int..
            auto LFdrive = *treeState.getRawParameterValue("driveLF");           // i.e. auto LFdrive = *treeState.getRawParameterValue("driveLF");
            auto LFthres = *treeState.getRawParameterValue("thresLF");           // threshold.. auto or float  (to do:  use in soft clipping for 'range'?)
            auto LFdw = *treeState.getRawParameterValue("drywetLF");
            auto LFvol = *treeState.getRawParameterValue("volLF");               // volume or gain .. auto or float  (causes a lot of arithmetic overflow)


            // possible solution for not processing any sample in teh wrong channel, but faulty for now..
            outputLF[i] = LFdspDistortion.process(LFin, LFout[channel], LFchoice, LFdrive, LFthres, LFdw, LFvol); 

            /* 
            //////// this is now in the function above..
            if (LFchoice == 0) // "Hard Clipping LPF" 
            {
                if (LFin * (LFdrive * 0.25f) > LFthres) 
                {         
                    LFout = LFthres * (LFdrive * 0.25f);
                } 

                else if (LFin * (LFdrive * 0.25f) < -LFthres) 
                {
                    LFout = -LFthres * (LFdrive * 0.25f);
                }

                else 
                {
                    LFout = LFin * (LFdrive * 0.25f);
                }
            }
            
            else if (LFchoice == 1)  //"Soft Clipping LPF"             
            {
                LFout = ((2.0f + (LFthres * 2.0f)) / juce::MathConstants<float>::pi) * std::atan(LFin * LFdrive); 
            }                    

            else if (LFchoice == 2)  //"Half Wave" (using absolute functions (not fabs but std::abs))
            {
                LFout = (0.5f * (std::abs(LFin) + LFin)) * ((LFdrive * 0.5f) + (LFthres * LFdrive)); 
            }
            

            else if (LFchoice == 3) // "Overdrive"
            { 
                if (LFin < LFthres * 0.1f) // if in < 0.0 -- now experimentally using threshold
                {
                    LFout = (((-std::sqrt(-LFin) - LFin) * (LFdrive * 0.25f)) + LFin) * (1.0f - (LFdrive * 0.03125f)) * 0.2f; // reduce vol applied  f causing overfows ((*LFdrive / 4.0f) * 0.5f)) * (*LFvol * 0.2f);
                } //                                       lowered drive                         lowered drive
                else // above threshold path
                {
                    LFout = (((std::sqrt(LFin) - LFin) * (LFdrive * 0.25f)) + LFin) * (1.0f - (LFdrive * 0.03125f)) * 0.2f; //((LFdrive * 0.25f) / 4.0f) now 0.0625 * 0.5
                }
            }
            
            else // (LFchoice == 4) 'none' - do nothing
            {
                LFout = LFin; // just to call in out; gain control now done below
            }
           
            LFchannelData[i] = ((LFout * LFdw) + (LFin * (1.0f - LFdw))) * LFvol;

            */
        }


        // inner for-loop for HIGH FREQUENCIES (can we live with 2 inner loops?)
        for (int j = 0; j < HPBuffer.getNumSamples(); j++)
        {
            auto HFin = HFchannelData[j]; // it's only local but calling this int for samples j and not i .. just in case it'll cause any confusion
            float HFout;

            // which distortion is selected?
            auto HFchoice = *treeState.getRawParameterValue("distortionTypeHF");

            // get knob values
            auto HFdrive = *treeState.getRawParameterValue("driveHF");
            auto HFthres = *treeState.getRawParameterValue("thresHF");
            auto HFdw = *treeState.getRawParameterValue("drywetHF");
            auto HFvol = *treeState.getRawParameterValue("volHF");


            if (HFchoice == 0) // "Hard Clipping LPF" 
            {
                if (HFin * (HFdrive * 0.25f) > HFthres)
                {
                    HFout = HFthres * (HFdrive * 0.25f); 
                } 

                else if (HFin * (HFdrive * 0.25f) < -HFthres)
                {
                    HFout = -HFthres * (HFdrive * 0.25f);
                }

                else 
                {
                    HFout = HFin * (HFdrive * 0.25f);
                }
            }

            else if (HFchoice == 1) // "Soft"
            {
                HFout = ((2.0f + (HFthres * 2.0f)) / juce::MathConstants<float>::pi) * std::atan(HFin * HFdrive);
            }

            else if (HFchoice == 2)  // "Half Wave"
            {
                HFout = (0.5f * (std::abs(HFin) + HFin)) * ((HFdrive * 0.5f) + (HFthres * HFdrive)); 
            }
            

            else if (HFchoice == 3) // "Overdrive"
            {
                if (HFin < HFthres * 0.2f)
                {
                    HFout = (((-std::sqrt(-HFin) - HFin) * (HFdrive * 0.25f)) + HFin) * (1.0f - (HFdrive * 0.03125f)) * 0.2f;
                }

                else 
                {
                    HFout = (((std::sqrt(HFin) - HFin) * (HFdrive * 0.25f)) + HFin) * (1.0f - (HFdrive * 0.03125f)) * 0.2f; 
                }
            }


            else 
            {
                HFout = HFin;
            }

            // apply dry/wet and volume, and let it be channel data again
            HFchannelData[j] = ((HFout * HFdw) + (HFin * (1.0f - HFdw))) * HFvol;

        }

            // flip phase of the high pass (muliply by –1) so it's in-phase with the low pass
            HPBuffer.applyGain(channel, 0, HPBuffer.getNumSamples(), -1.0f); 

            // add low pass and high pass branches together and copy them into output buffer
            HPBuffer.addFrom(channel, 0, LPBuffer, channel, 0, LPBuffer.getNumSamples());
            buffer.copyFrom(channel, 0, HPBuffer, channel, 0, HPBuffer.getNumSamples()); // output buffer
    }

       
}

//==============================================================================
bool SpectralDistortionAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SpectralDistortionAudioProcessor::createEditor()
{
    return new SpectralDistortionAudioProcessorEditor(*this, treeState);
}

//==============================================================================
void SpectralDistortionAudioProcessor::getStateInformation (MemoryBlock& destData) //override?! //JUCE tutorial does so... https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
{
    // creates xlm to save treeState (xml) into a memoryblock object (it'll have the tag name "PARAMETERS" eventually)
    auto state = treeState.copyState(); 
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SpectralDistortionAudioProcessor::setStateInformation(const void* data, int sizeInBytes) 
{
    // recall from what's been saved
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)        // errorchecking - does the xml have the right tag?
        if (xmlState->hasTagName(treeState.state.getType()))       //if it passes the test...
            treeState.replaceState(ValueTree::fromXml(*xmlState)); //...replace values in vts
}

//==============================================================================

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralDistortionAudioProcessor();
}


/*
//  R E S E T   F U N C T I O N
void SpectralDistortionAudioProcessor::reset()  //argh! 'inherited member is not allowed. can't find a solution...
{
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        LPFilter1[channel].reset();
        HPFilter1[channel].reset();
        LPFilter2[channel].reset();
        HPFilter2[channel].reset();
    }
// reset filter called in prepare to play...?
}
*/




