/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::cleanInterface()
{
	soundZones.clear();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::updateInterface()
{
	for (int ss = 0; ss < processor.collection->soundGroups[processor.collection->editingSound]->sounds.size(); ss++)
	{
		addAndMakeVisible(soundZones.add(new WSPXSoundZone(processor.collection->soundGroups[processor.collection->editingSound]->sounds[ss])));
	}
}