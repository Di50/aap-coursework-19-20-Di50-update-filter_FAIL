/*
LFdspDistortion
*/

#include "LFdspDistortion.h"

float LFdspDistortion::process(float LFin, float &LFout, float LFchoice, float LFdrive, float LFthres, float LFdw, float LFvol)
{
    float out = LFout; // previous / delayed sample...

   if (LFchoice == 0) // "Hard Clipping LPF" (sound rough in lows, betting highs only?)
    {
    if (LFin * (LFdrive * 0.25f) > LFthres) // if sample is above threshold, set it to theshold, and apply volume (high threshold isn't doing much, so /2)
    {          // added multipl by drive
        out = LFthres * (LFdrive * 0.25f); // threshold is a bit weak in vol, so idea to add control via drive input / 4 (resp *0.25 -- computational easier?)
    } // these brackets might be redundant, however, they're used in any other if statement and don't seem to do harm

    else if (LFin * (LFdrive * 0.25f) < -LFthres) // .. same for negative part
    {
        out = -LFthres * (LFdrive * 0.25f);
    }

    else // if smaple is neither exceeding lower or upper threshold keep it as it is , just apply drive (linear)
    {
        out = LFin * (LFdrive * 0.25f);
    }
    }

   else if (LFchoice == 1)  //"Soft Clipping LPF"              // using arct tan
   {
    out = ((2.0f + (LFthres * 2.0f)) / juce::MathConstants<float>::pi) * std::atan(LFin * LFdrive); //((2.0f * LFthres) / juce::MathConstants<float>::pi) * std::atan(LFin * LFdrive); // changeing atanf now using std::atan (Julien of Roli recommends)
   }                    // adding twice the thresh

   else if (LFchoice == 2)  //"Half Wave" (using absolute functions (not fabs but std::abs))
   {
    out = (0.5f * (std::abs(LFin) + LFin)) * ((LFdrive * 0.5f) + (LFthres * LFdrive)); //0.25f);/////////
    }


   else if (LFchoice == 3) // "Overdrive"
   {
    if (LFin < LFthres * 0.1f) // if in < 0.0 -- now experimentally using threshold
        out = (((-std::sqrt(-LFin) - LFin) * (LFdrive * 0.25f)) + LFin) * (1.0f - (LFdrive * 0.03125f)) * 0.2f; // reduce vol applied  f causing overfows ((*LFdrive / 4.0f) * 0.5f)) * (*LFvol * 0.2f);
                        
    else // above threshold path
    
        out = (((std::sqrt(LFin) - LFin) * (LFdrive * 0.25f)) + LFin) * (1.0f - (LFdrive * 0.03125f)) * 0.2f; //((LFdrive * 0.25f) / 4.0f) now 0.0625 * 0.5
    }

   else
   {// (LFchoice == 4) 'none' - do nothing
    out = LFin; // just to call in out; gain control now done below
   }

  // apply dry/wet and volume, and let it be channel data again
  float LFprocessed = ((out * LFdw) + (LFin * (1.0f - LFdw))) * LFvol;

  return LFprocessed;
}
