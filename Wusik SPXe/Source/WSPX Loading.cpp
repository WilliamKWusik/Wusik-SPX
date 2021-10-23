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
	{
		collection->sounds.add(new WSPX_Collection_Sound);
		collection->sounds.getFirst()->soundFiles.add(new WSPX_Collection_Sound_File);
		collection->sounds.getFirst()->soundFiles.getLast()->keyZoneHigh = 59;
		collection->sounds.getFirst()->soundFiles.getLast()->keyRoot = 42;
		collection->sounds.getFirst()->soundFiles.add(new WSPX_Collection_Sound_File);
		collection->sounds.getFirst()->soundFiles.getLast()->keyZoneLow = 60;
		collection->sounds.getFirst()->soundFiles.getLast()->keyZoneHigh = 88;
		collection->sounds.getFirst()->soundFiles.getLast()->keyRoot = 78;
		collection->sounds.getFirst()->soundFiles.add(new WSPX_Collection_Sound_File);
		collection->sounds.getFirst()->soundFiles.getLast()->keyZoneLow = 89;
		collection->sounds.getFirst()->soundFiles.getLast()->keyRoot = 102;
		collection->sounds.getFirst()->name = "Test Sound 1";
		//
		collection->sounds.add(new WSPX_Collection_Sound);
		collection->sounds.getLast()->soundFiles.add(new WSPX_Collection_Sound_File);
		collection->sounds.getLast()->name = "Another Sound 2";
	}
	//
	collection->presets.getFirst()->name = "My Preset XYZ";
	collection->presets.getFirst()->layers.add(new WSPX_Collection_Preset_Layer);
	collection->presets.getFirst()->layers.getFirst()->soundLinks.add(collection->sounds.getFirst());
	collection->presets.getFirst()->layers.getFirst()->soundLinks.add(collection->sounds.getLast());
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
	//
	if (header.containsIgnoreCase("WSPXe"))
	{
		collection->streamData((void*)&stream, WS::kRead);
		return true;
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error loading file!", "Wrong header information.");
		return false;
	}
}