/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_SubSounds
{
public:
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
	int16 currentPreset = 0;
	int16 currentSound = 0;
	String name = "Empty";
	File file;
};