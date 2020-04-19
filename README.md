# aap-coursework-19-20-Di50-update-filter_FAIL
 non-working version, was trying to remediate crackling sound suspected originating in nested for loops in process block
 
 processing in stereo is really tricky, and -- as in my case -- when approached naively leads to crackling audio. 
 the suspected reason is that in the process block the outer loop iterates through number of channels (0 and 1)
 and the inner loops goes through samples from a buffer. when the outer loop proceeds to the next channel (i.e. ch 1), the last sample in
 the inner loop (which then is still from channel 0) gets pushed to the next channel -- hence some ploppps.
 
 the problem is descibed here: https://forum.juce.com/t/1-most-common-programming-mistake-that-we-see-on-the-forum/26013/28
 solutions involve seperate dsp process classes -- i attempted this, but ran into a problem (more comments on this in the .cpp above th
 process block), so this version as it stands now does not compile.
 
