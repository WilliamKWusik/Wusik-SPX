/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#include "PluginProcessor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection::streamData(void* stream, int type)
{
	int xVersion = 1; // The file version //
	int totalPresets = presets.size();
	int totalsounds = sounds.size();
	//
	if (isWSPXEditor) WS::stream(stream, playerSkinFolder, type); else WS::stream(stream, playerSkin, type);
	//
	WS::stream(stream, xVersion, type);
	WS::stream(stream, name, type);
	WS::stream(stream, author, type);
	WS::stream(stream, description, type);
	WS::stream(stream, company, type);
	WS::stream(stream, tags, type);
	WS::stream(stream, version, type); // The version reported by the user  //
	WS::stream(stream, file, type);
	WS::stream(stream, exportedFile, type);
	WS::stream(stream, protectionKey, type);
	collectionFile = file.getFullPathName();
	//
	imageAbout.streamData(stream, type);
	imageIcon.streamData(stream, type);
	//
	WS::stream(stream, totalsounds, type);
	WS::stream(stream, totalPresets, type);
	//
	if (isWSPXEditor)
	{
		for (int ss = 0; ss < totalsounds; ss++)
		{
			if (type == WS::kRead) sounds.add(new WSPX_Collection_Sound);
			sounds[ss]->streamData(stream, type);
		}
		//
		for (int pp = 0; pp < totalPresets; pp++)
		{
			if (type == WS::kRead) presets.add(new WSPX_Collection_Preset);
			presets[pp]->streamData(stream, type, sounds);
		}
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Preset::streamData(void* stream, int type, OwnedArray<WSPX_Collection_Sound>& soundsList)
{
	int totalLayers = layers.size();
	//
	WS::stream(stream, name, type);
	WS::stream(stream, author, type);
	WS::stream(stream, description, type);
	WS::stream(stream, tags, type);
	WS::stream(stream, volume, type);
	WS::stream(stream, pan, type);
	WS::stream(stream, fineTune, type);
	WS::stream(stream, coarseTune, type);
	WS::stream(stream, totalLayers, type);
	WS::stream(stream, scripting, type);
	WS::stream(stream, imagePresetIcon.imageFilename, type);
	//
	for (int ee = 0; ee < 4; ee++)
	{
		effects[ee].streamData(stream, type);
	}
	//
	for (int ll = 0; ll < totalLayers; ll++)
	{	
		if (type == WS::kRead) layers.add(new WSPX_Collection_Preset_Layer);
		layers[ll]->streamData(stream, type, soundsList);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Preset_Layer::streamData(void* stream, int type, OwnedArray<WSPX_Collection_Sound>& soundsList)
{
	WS::stream(stream, name, type);
	WS::stream(stream, volume, type);
	WS::stream(stream, pan, type);
	WS::stream(stream, fineTune, type);
	WS::stream(stream, glide, type);
	WS::stream(stream, keyZoneHigh, type);
	WS::stream(stream, keyZoneLow, type);
	WS::stream(stream, velZoneHigh, type);
	WS::stream(stream, velZoneLow, type);
	WS::stream(stream, autoGlide, type);
	WS::stream(stream, reverse, type);
	WS::stream(stream, coarseTune, type);
	WS::stream(stream, output, type);
	WS::stream(stream, voices, type);
	WS::stream(stream, scripting, type);
	//
	sequencer.streamData(stream, type);
	filter.streamData(stream, type);
	ampEnvelope.streamData(stream, type);
	lfos[0].streamData(stream, type);
	lfos[1].streamData(stream, type);
	//
	int totalsounds = soundLinks.size();
	WS::stream(stream, totalsounds, type);
	for (int ss = 0; ss < totalsounds; ss++)
	{
		if (isWSPXEditor)
		{
			if (type == WS::kRead)
			{
				int soundID = 0;
				WS::stream(stream, soundID, type);
				soundLinks.add(soundsList[soundID]);
			}
			else
			{
				int soundID = soundsList.indexOf(soundLinks[ss]);
				WS::stream(stream, soundID, type);
			}
		}
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Sequencer::streamData(void* stream, int type)
{
	time.streamData(stream, type);
	int totalSteps = steps.size();
	WS::stream(stream, loopStart, type);
	WS::stream(stream, smoothOutput, type);
	WS::stream(stream, mode, type);
	//
	for (int tt = 0; tt < totalSteps; tt++)
	{
		if (type == WS::kRead) steps.add(new WSPX_Sequencer_Step);
		steps[tt]->streamData(stream, type);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Sound_File::streamData(void* stream, int type)
{
	WS::stream(stream, roundRobin, type);
	WS::stream(stream, random, type);
	WS::stream(stream, release, type);
	WS::stream(stream, reverse, type);
	WS::stream(stream, keySwitch, type);
	WS::stream(stream, keySwitchType, type);
	WS::stream(stream, loopStart, type);
	WS::stream(stream, loopEnd, type);
	WS::stream(stream, loopType, type);
	WS::stream(stream, randomProbability, type);
	WS::stream(stream, keyZoneLow, type);
	WS::stream(stream, keyZoneHigh, type);
	WS::stream(stream, velZoneLow, type);
	WS::stream(stream, velZoneHigh, type);
	WS::stream(stream, keyRoot, type);
	WS::stream(stream, fineTune, type);
	WS::stream(stream, coarseTune, type);
	WS::stream(stream, bits, type);
	WS::stream(stream, format, type);
	WS::stream(stream, channels, type);
	WS::stream(stream, sampleDataMetaValuesRead, type);
	WS::stream(stream, totalSamples, type);
	//
	int totalChannelsInfo = channelInformation.size();
	WS::stream(stream, totalChannelsInfo, type);
	for (int cc = 0; cc < totalChannelsInfo; cc++)
	{
		if (type == WS::kRead) channelInformation.add(new WSPX_Channel_Info);
		channelInformation[cc]->streamData(stream, type);
	}
	//
	if (isWSPXEditor)
	{
		WS::stream(stream, exportBits, type);
		WS::stream(stream, exportFormat, type);
		WS::streamRelativePath(stream, soundFile, type);
		//
		if (WSPXeBundle)
		{
			if (type == WS::kWrite) File(soundFile).loadFileAsData(soundData);
			WS::stream(stream, soundData, type);
		}
	}
	else
	{
		//
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Effect::streamData(void* stream, int _type)
{
	WS::stream(stream, type, _type);
	WS::stream(stream, dry, _type);
	WS::stream(stream, wet, _type);
	WS::stream(stream, parallel, _type);
	//
	for (int ee = 0; ee < 4; ee++)
	{
		bool hasEffect = false;
		WS::stream(stream, hasEffect, _type);
		//
		// if (type == WS::kRead)
		// if (effect[ee] != nullptr) effect[ee]->streamData(stream, type);
	}
}