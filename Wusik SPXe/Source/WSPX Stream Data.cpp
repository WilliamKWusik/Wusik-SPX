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
	int totalSoundGroups = soundGroups.size();
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
	//
	imageAbout.streamData(stream, type);
	imageIcon.streamData(stream, type);
	//
	WS::stream(stream, totalPresets, type);
	WS::stream(stream, totalSoundGroups, type);
	//
	if (isWSPXEditor)
	{
		for (int pp = 0; pp < totalPresets; pp++)
		{
			if (type == WS::kRead) presets.add(new WSPX_Collection_Preset);
			presets[pp]->streamData(stream, type);
		}
		//
		for (int ss = 0; ss < totalSoundGroups; ss++)
		{
			if (type == WS::kRead) soundGroups.add(new WSPX_Collection_Sound_Group);
			soundGroups[ss]->streamData(stream, type);
		}
	}
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Preset::streamData(void* stream, int type)
{
	int totalEffects = effects.size();
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
	WS::stream(stream, totalEffects, type);
	WS::stream(stream, totalLayers, type);
	WS::stream(stream, imagePresetIcon.imageFilename, type);
	//
	for (int ll = 0; ll < totalLayers; ll++)
	{	
		if (type == WS::kRead) layers.add(new WSPX_Collection_Preset_Layer);
		layers[ll]->streamData(stream, type);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Preset_Layer::streamData(void* stream, int type)
{
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
	int totalChannels = channels.size();
	WS::stream(stream, totalChannels, type);
	for (int ch = 0; ch < totalChannels; ch++)
	{
		if (type == WS::kRead) channels.add(new WSPX_Channel);
		WS::stream(stream, channels[ch]->name, type);
		WS::stream(stream, channels[ch]->volume, type);
	}
	//
	int totalSoundGroupIDs = soundGroupIDs.size();
	WS::stream(stream, totalSoundGroupIDs, type);
	for (int ss = 0; ss < totalSoundGroupIDs; ss++)
	{
		if (type == WS::kRead) soundGroupIDs.add(0);
		WS::stream(stream, soundGroupIDs.getRawDataPointer()[ss], type);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Sequencer::streamData(void* stream, int type)
{
	int totalSteps = steps.size();
	WS::stream(stream, syncBPM, type);
	WS::stream(stream, time1, type);
	WS::stream(stream, time2, type);
	WS::stream(stream, loopStart, type);
	WS::stream(stream, smoothOutput, type);
	WS::stream(stream, type, type);
	//
	for (int tt = 0; tt < totalSteps; tt++)
	{
		if (type == WS::kRead) steps.add(new WSPX_Sequencer_Step);
		steps[tt]->streamData(stream, type);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Sound::streamData(void* stream, int type)
{
	WS::stream(stream, roundRobin, type);
	WS::stream(stream, random, type);
	WS::stream(stream, isRelease, type);
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
	WS::stream(stream, totalSamples, type);
	//
	int totalChannelsPan = channelPan.size();
	for (int cc = 0; cc < totalChannelsPan; cc++)
	{
		if (type == WS::kRead) channelPan.add(0.0f);
		WS::stream(stream, channelPan.getRawDataPointer()[cc], type);
	}
	//
	if (isWSPXEditor)
	{
		WS::stream(stream, exportBits, type);
		WS::stream(stream, exportFormat, type);
		WS::stream(stream, soundFile, type);
	}
	else
	{
		//
	}
}