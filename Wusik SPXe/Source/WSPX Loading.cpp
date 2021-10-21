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
		if (!AlertWindow::showOkCancelBox(AlertWindow::NoIcon, "There are unsaved changes!", "Are you sure you want to continue?"))
		{
			return;
		}
	}
	//
	collection = new WSPX_Collection;
	collection->name = "Testing WSPX Collection";
	collection->presets.add(new WSPX_Collection_Preset);
	//
	collection->presets.getFirst()->name = "My Preset XYZ";
	collection->presets.getFirst()->layers.add(new WSPX_Collection_Preset_Layer);
	collection->presets.getFirst()->layers.getFirst()->soundGroupIDs.add(0);
	collection->presets.getFirst()->layers.getFirst()->soundGroupIDs.add(1);
	//
	collection->soundGroups.add(new WSPX_Collection_Sound_Group);
	collection->soundGroups.getFirst()->sounds.add(new WSPX_Collection_Sound);
	collection->soundGroups.getFirst()->sounds.getLast()->keyZoneHigh = 59;
	collection->soundGroups.getFirst()->sounds.getLast()->keyRoot = 42;
	collection->soundGroups.getFirst()->sounds.add(new WSPX_Collection_Sound);
	collection->soundGroups.getFirst()->sounds.getLast()->keyZoneLow = 60;
	collection->soundGroups.getFirst()->sounds.getLast()->keyZoneHigh = 88;
	collection->soundGroups.getFirst()->sounds.getLast()->keyRoot = 78;
	collection->soundGroups.getFirst()->sounds.add(new WSPX_Collection_Sound);
	collection->soundGroups.getFirst()->sounds.getLast()->keyZoneLow = 89;
	collection->soundGroups.getFirst()->sounds.getLast()->keyRoot = 102;
	collection->soundGroups.getFirst()->name = "Test Sound 1";
	//
	collection->soundGroups.add(new WSPX_Collection_Sound_Group);
	collection->soundGroups.getLast()->sounds.add(new WSPX_Collection_Sound);
	collection->soundGroups.getLast()->name = "Another Sound 2";
	//
	collection->presets.add(new WSPX_Collection_Preset);
	collection->presets.getLast()->name = "Init Preset";
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
bool WusikSpxAudioProcessor::loadCompilation(InputStream& stream)
{
	collection = new WSPX_Collection;
	//
	String header = stream.readString();
	int version = stream.readByte();
	//
	if (header.containsIgnoreCase("WSPXe"))
	{
		collection->name = stream.readString();
		collection->author = stream.readString();
		collection->description = stream.readString();
		collection->company = stream.readString();
		collection->tags = stream.readString();
		collection->version = stream.readString();
		collection->file = stream.readString();
		collection->exportedFile = stream.readString();
		collection->protectionKey = stream.readString();
		collection->imageAbout.imageFilename = stream.readString();
		collection->imageIcon.imageFilename = stream.readString();
		//
		int totalPresets = stream.readInt();
		int totalSoundGroups = stream.readInt();
		//
		return true;
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error loading file!", "Wrong header information.");
		return false;
	}
}