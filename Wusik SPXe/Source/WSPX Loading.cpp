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
	//
	collection->presets.getFirst()->name = "Init";
	collection->presets.getFirst()->layers.add(new WSPX_Collection_Preset_Layer);
	collection->presets.getFirst()->layers.getFirst()->soundGroupIDs.add(0);
	//
	collection->soundGroups.add(new WSPX_Collection_Sound_Group);
	collection->soundGroups.getFirst()->sounds.add(new WSPX_Collection_Sound);
	collection->soundGroups.getFirst()->name = "Empty Sound";
	//
	collection->presets.add(new WSPX_Collection_Preset);
	collection->presets.getLast()->name = "Init 2";
}