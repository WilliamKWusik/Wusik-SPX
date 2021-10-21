/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#include "PluginProcessor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection::streamWSPXe(void* stream, int type)
{
	int xVersion = 1; // The WSPXe File Version //
	int totalPresets = presets.size();
	int totalSoundGroups = soundGroups.size();
	//
	WS::stream(stream, xVersion, type);
	WS::stream(stream, name, type);
	WS::stream(stream, author, type);
	WS::stream(stream, description, type);
	WS::stream(stream, company, type);
	WS::stream(stream, tags, type);
	WS::stream(stream, version, type); // The Version Reported by the User for this WSPX file //
	WS::stream(stream, file, type);
	WS::stream(stream, exportedFile, type);
	WS::stream(stream, protectionKey, type);
	//
	imageAbout.streamWSPXe(stream, type);
	imageIcon.streamWSPXe(stream, type);
	//
	WS::stream(stream, totalPresets, type);
	WS::stream(stream, totalSoundGroups, type);
	//
	for (int pp = 0; pp < totalPresets; pp++)
	{
		if (type == WS::kRead) presets.add(new WSPX_Collection_Preset);
		presets[pp]->streamWSPXe(stream, type);
	}
	//
	for (int ss = 0; ss < totalSoundGroups; ss++)
	{
		if (type == WS::kRead) soundGroups.add(new WSPX_Collection_Sound_Group);
	}
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Preset::streamWSPXe(void* stream, int type)
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
		layers[ll]->streamWSPXe(stream, type);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPX_Collection_Preset_Layer::streamWSPXe(void* stream, int type)
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
	sequencer.streamWSPXe(stream, type);
	filter.streamWSPXe(stream, type);
	ampEnvelope.streamWSPXe(stream, type);
	lfos[0].streamWSPXe(stream, type);
	lfos[1].streamWSPXe(stream, type);
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
void WSPX_Sequencer::streamWSPXe(void* stream, int type)
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
		steps[tt]->streamWSPXe(stream, type);
	}
}