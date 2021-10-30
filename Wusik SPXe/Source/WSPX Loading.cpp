/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
#pragma once
#include "PluginEditor.h"
#include "WSPX Definitions.h"
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
		collection->sounds.getFirst()->soundFiles.add(new WSPX_Collection_Sound_File);
		collection->sounds.getFirst()->soundFiles.add(new WSPX_Collection_Sound_File);
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
	WSPXeBundle = false;
	//
	if (header.containsIgnoreCase("WSPXe"))
	{
		if (header.containsIgnoreCase("WSPXeBundle"))
		{
			collection->isWSPXeBundle = true;
			WSPXeBundle = true;
		}
		//
		collection->streamData((void*)&stream, WS::kRead);
		return true;
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error loading file!", "Wrong header information.");
		return false;
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::loadSoundFileDetails(WSPX_Collection_Sound_File* soundFile)
{
	int channels = 0;
	float panning = -1.0f;
	//
	for (int ff = 0; ff < soundFile->files.size(); ff++)
	{
		if (soundFile->files[ff]->filename.isNotEmpty())
		{
			ScopedPointer<AudioFormatReader> audioReader = audioFormatManager.createReaderFor(File(soundFile->files[ff]->filename));
			//
			if (audioReader != nullptr)
			{
				soundFile->totalSamples = audioReader->lengthInSamples;
				soundFile->sampleRate = audioReader->sampleRate;
				soundFile->files[ff]->channels = audioReader->numChannels;
				//
				if (!soundFile->sampleDataMetaValuesRead)
				{
					soundFile->loopStart = jlimit(0, (int)soundFile->totalSamples - 1, audioReader->metadataValues.getValue("Loop0Start", "0").getIntValue());
					soundFile->loopEnd = jlimit(0, (int)soundFile->totalSamples - 1, audioReader->metadataValues.getValue("Loop0End", "0").getIntValue());
					soundFile->keyRoot = jlimit(0, 127, audioReader->metadataValues.getValue("MidiUnityNote", "60").getIntValue());
					//
					if (soundFile->files[ff]->channelsInfo.size() > 0) soundFile->files[ff]->channelsInfo.clear();
					//
					for (int cc = 0; cc < audioReader->numChannels; cc++)
					{
						channels++;
						soundFile->files[ff]->channelsInfo.add(new WSPX_Channel_Info);
						soundFile->files[ff]->channelsInfo.getLast()->name = "Channel " + String(channels);
						soundFile->files[ff]->channelsInfo.getLast()->pan = panning;
						if (panning == -1.0f) panning = 1.0f; else panning = -1.0f;
					}
				}
			}
			else
			{
				AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error Loading Sound File! Couldn't create a reader for it...", soundFile->files[ff]->filename);
			}
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error Loading Sound File!", "Sound File is Empty");
		}
	}
	//
	if (channels == 1) // Mono, one file //
	{
		soundFile->files.getFirst()->channelsInfo.getFirst()->pan = 0.0f;
		soundFile->files.getFirst()->channelsInfo.getFirst()->name = "Mono";
	}
	else if (channels == 2 && soundFile->files.size() == 1) // Stereo, one file //
	{
		soundFile->files.getFirst()->channelsInfo.getFirst()->name = "Left";
		soundFile->files.getFirst()->channelsInfo.getLast()->name = "Right";
	}
	//
	soundFile->sampleDataMetaValuesRead = true;
	soundFile->totalChannels = channels;
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::loadPreset(bool forceLoad)
{
	if (forceLoad)
	{
		if (playerPreset != nullptr) unloadSounds(&playerPreset->preset);
		while (processThread != nullptr && processThread->isThreadRunning()) { Thread::sleep(100); }
		processThread = new WSPXThread((void*)this, getActiveEditor(), WSPXThread::kLoadPreset, "Sound Loading");
		processThread->runThread(4);
		processThread = nullptr;
	}
	else
	{
		if (playerPreset != nullptr)
		{
			unloadSounds(&playerPreset->preset);
			playerPreset = nullptr;
		}
		else
		{
			if (collection->lastSelectedPreset == nullptr) AlertWindow::showMessageBox(AlertWindow::NoIcon, "Preset Preview", "Select a preset first!");
			else
			{
				while (processThread != nullptr && processThread->isThreadRunning()) { Thread::sleep(100); }
				processThread = new WSPXThread((void*)this, getActiveEditor(), WSPXThread::kLoadPreset, "Sound Loading");
				processThread->runThread(4);
				processThread = nullptr;
			}
		}
	}
	//
	getActiveEditor()->repaint();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::addSoundFiles(Array<File>& files, WSPX_Collection_Sound& sound, void* treeItem)
{
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)getActiveEditor();
	//
	for (int ff = 0; ff < files.size(); ff++)
	{
		sound.soundFiles.add(new WSPX_Collection_Sound_File);
		sound.soundFiles.getLast()->files.add(new WSPX_Collection_Sound_File_Filename);
		sound.soundFiles.getLast()->files.getFirst()->filename = files[ff].getFullPathName();
		loadSoundFileDetails(sound.soundFiles.getLast());
		editor->loadSoundFileThumb(sound.soundFiles.getLast());
		//
		String filename = files[ff].getFileNameWithoutExtension();
		//
		if (filename.contains("_") && filename.dropLastCharacters(3).getLastCharacter() == '_' && filename.dropLastCharacters(7).getLastCharacter() == '_')
		{
			sound.soundFiles.getLast()->keyRoot = filename.fromLastOccurrenceOf("_", false, false).getIntValue();
			filename = filename.upToLastOccurrenceOf("_", false, false);
			//
			sound.soundFiles.getLast()->velZoneHigh = filename.fromLastOccurrenceOf("_", false, false).getIntValue();
			filename = filename.upToLastOccurrenceOf("_", false, false);
			//
			sound.soundFiles.getLast()->velZoneLow = filename.fromLastOccurrenceOf("_", false, false).getIntValue();
			filename = filename.upToLastOccurrenceOf("_", false, false);
			//
			sound.soundFiles.getLast()->keyZoneHigh = filename.fromLastOccurrenceOf("_", false, false).getIntValue();
			filename = filename.upToLastOccurrenceOf("_", false, false);
			//
			sound.soundFiles.getLast()->keyZoneLow = filename.fromLastOccurrenceOf("_", false, false).getIntValue();
			filename = filename.upToLastOccurrenceOf("_", false, false);
		}
		//
		((WSPXSoundTreeItem*)treeItem)->addSubItem(new WSPXSoundTreeItem(*this, editor->multRatio, WSPXSoundTreeItem::kLevel_Sound_Files, "", WSPXSoundTreeItem::kRegular_Item, &sound, sound.soundFiles.getLast()));
	}
	//
	editor->presetChanged();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
int WusikSpxAudioProcessor::pickSound()
{
	StringArray soundItems;
	for (int gg = 0; gg < collection->sounds.size(); gg++)
	{
		soundItems.add("Sound " + String(gg + 1) + ": " + collection->sounds[gg]->name);
	}
	//
	AlertWindow w("Select a Sound", "", AlertWindow::NoIcon);
	w.addComboBox("Sound", soundItems);
	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
	w.setAlwaysOnTop(true);
	//
	if (w.runModalLoop() == 1) return w.getComboBoxComponent("Sound")->getSelectedItemIndex();
	return -1;
}