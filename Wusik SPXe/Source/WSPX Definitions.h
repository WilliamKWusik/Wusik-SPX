/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
//
#define WSPXEDITOR 1
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_SubSounds
{
public:
	AudioSampleBuffer samples;
	//
	#if WSPXEDITOR
		File soundFile;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sounds
{
public:
	OwnedArray<WSPX_Collection_SubSounds> subSounds;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Presets
{
public:
	OwnedArray<WSPX_Collection_Sounds> sounds;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection
{
public:
	OwnedArray<WSPX_Collection_Presets> presets;
	String name = "Empty";
	//
	#if WSPXEDITOR
		int16 editingPreset = 0;
		int16 editingSound = 0;
		File file;
	#endif
};