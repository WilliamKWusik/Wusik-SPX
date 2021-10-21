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
	stream.writeByte(1); // WSPXe File Version //
	//
	stream.writeString(collection->name);
	stream.writeString(collection->author);
	stream.writeString(collection->description);
	stream.writeString(collection->company);
	stream.writeString(collection->tags);
	stream.writeString(collection->version);
	stream.writeString(collection->file.getFullPathName());
	stream.writeString(collection->protectionKey);
	stream.writeString(collection->imageAbout.imageFilename.getFullPathName());
	stream.writeString(collection->imageIcon.imageFilename.getFullPathName());
	//
	stream.writeInt(collection->presets.size());
	stream.writeInt(collection->soundGroups.size());
	//
	stream.flush();
	collection->hasUnsavedChanges = false;
}