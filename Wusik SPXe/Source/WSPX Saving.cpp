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
	if (WSPXeBundle)
	{
		stream.writeString("WSPXeBundle");
		collection->isWSPXeBundle = true;
	}
	else if (isWSPXEditor) stream.writeString("WSPXe"); 
	else stream.writeString("WSPX");
	//
	collection->streamData(&stream, WS::kWrite);
	stream.flush();
	collection->hasUnsavedChanges = false;
}