/*
LFdspDistortion
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class LFdspDistortion
{
public:
	float process(float LFin, float LFout, float LFchoice, float LFdrive, float LFthres, float LFdw, float LFvol);
private:
	//float out;
	//float LFout;
};

