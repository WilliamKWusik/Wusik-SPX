/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXThread::run()
{
	WusikSpxAudioProcessor* processor = (WusikSpxAudioProcessor*) _processor;
	processor->stopAllAudio();
	//
	if (type == kLoadPreset)
	{
		#if WSPXPLAYERPREVIEW
			processor->playerPreset = new WSPX_Player_Preset;
			processor->collection->lastSelectedPreset;
		#else
			WMessageBox("NOT AVAILABLE", "The open source code does not include the Player code.");
		#endif
	}
	//
	processor->resumeAudio();
	processor->processThread = nullptr;
}