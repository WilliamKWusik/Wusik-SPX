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
	if (soundFile->soundFile.isNotEmpty())
	{
		ScopedPointer<AudioFormatReader> audioReader = audioFormatManager.createReaderFor(File(soundFile->soundFile));
		//
		if (audioReader != nullptr)
		{
			soundFile->channels = audioReader->numChannels;
			soundFile->totalSamples = audioReader->lengthInSamples;
			soundFile->sampleRate = audioReader->sampleRate;
			//
			if (!soundFile->sampleDataMetaValuesRead)
			{
				soundFile->loopStart = jlimit(0, (int) soundFile->totalSamples - 1, audioReader->metadataValues.getValue("Loop0Start", "0").getIntValue());
				soundFile->loopEnd = jlimit(0, (int)soundFile->totalSamples - 1, audioReader->metadataValues.getValue("Loop0End", "0").getIntValue());
				soundFile->keyRoot = jlimit(0, 127, audioReader->metadataValues.getValue("MidiUnityNote", "60").getIntValue());
				//
				soundFile->sampleDataMetaValuesRead = true;
			}
			//
			soundFile->channelInformation.clear();
			//
			for (int cc = 0; cc < soundFile->channels; cc++)
			{
				soundFile->channelInformation.add(new WSPX_Channel_Info);
				soundFile->channelInformation.getLast()->name = "Channel #" + String(cc + 1);
			}
			//
			if (soundFile->channels == 1) soundFile->channelInformation.getFirst()->name = "Mono";
			else if (soundFile->channels == 2)
			{
				soundFile->channelInformation.getFirst()->name = "Left";
				soundFile->channelInformation.getFirst()->pan = -1.0f;
				soundFile->channelInformation.getLast()->name = "Right";
				soundFile->channelInformation.getLast()->pan = 1.0f;
			}
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error Loading Sound File! Couldn't create a reader for it...", soundFile->soundFile);
		}
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error Loading Sound File!", "Sound File is Empty");
	}
}