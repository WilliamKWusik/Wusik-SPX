/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#include "PluginProcessor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::saveCompilation(OutputStream& stream)
{
	// This saves the Compilation file in WSPXe format (editor format) not the final WSPX format (player format) //
	//
	stream.writeString("WSPXe Source Compilation File");
	collection->streamWSPXe(&stream, WS::kWrite);
	stream.flush();
	collection->hasUnsavedChanges = false;
}