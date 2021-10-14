/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
#pragma once
#include "PluginProcessor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::newCompilation()
{
	if (collection != nullptr && collection->hasUnsavedChanges)
	{

	}
	//
	collection = new WSPX_Collection;
	collection->name = "Empty";
	collection->presets.add(new WSPX_Collection_Preset);

	/*collection->presets.getFirst()->sounds.add(new WSPX_Collection_Sounds);
	collection->presets.getFirst()->sounds.getFirst()->subSounds.add(new WSPX_Collection_SubSounds);
	//
	collection->presets.getFirst()->sounds.getFirst()->subSounds.getFirst()->samples.setSize(2, 100, false, true); */
}