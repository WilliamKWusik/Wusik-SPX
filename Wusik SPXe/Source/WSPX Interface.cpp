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
	/*for (int ss = 0; ss < processor.collection->soundGroups[processor.collection->editingSound]->sounds.size(); ss++)
	{
		addAndMakeVisible(soundZones.add(new WSPXSoundZone(processor.collection->soundGroups[processor.collection->editingSound]->sounds[ss])));
	}*/

	if (editObject.type == WusikEditObject::kCollection)
	{
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Name", &processor.collection->name)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Author", &processor.collection->author)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Version", &processor.collection->version)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Company", &processor.collection->company)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Tags", &processor.collection->tags)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Description", &processor.collection->description)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kImage, "Image Icon", &processor.collection->imageIcon)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kImage, "Image About", &processor.collection->imageAbout)));
		addAndMakeVisible(editOptions.add(new WusikEditOption(WusikEditOption::kString, "Protection Key", &processor.collection->protectionKey)));
	}
	else if (editObject.type == WusikEditObject::kPreset)
	{
		WSPX_Collection_Preset* preset = (WSPX_Collection_Preset*) editObject.object;
		//
		for (int ll = 0; ll < preset->layers.size(); ll++)
		{
			
		}
	}
}