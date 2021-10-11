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
#if WSPXEDITOR
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

	};
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_LFO
{
public:
	char waveform = 0;
	bool sync = true;
	bool inverted = false;
	bool noteOnReset = false;
	float speed1 = 1.0f;
	float speed2 = 4.0f;
	float phase = 0.0f;
	float smooth = 0.0f;
	float toVolume = 0.0f;
	float toFilterFreq = 0.0f;
	//
	enum
	{
		kWaveform_Sine = 0,
		kWaveform_Sawtooth,
		kWaveform_Pulse,
		kWaveform_Random,
		kWaveform_Noise,
		kWaveform_Triangle
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Envelope
{
public:
	int16 type = 0;
	float attack = 0.0f;
	float decay = 0.0f;
	float sustain = 1.0f;
	float release = 0.28f;
	float velocity = 1.0f;
	float maxSeconds = 6.0f;
	float keyTrack = 0.0f;
	float velTrack = 0.0f;
	//
	enum
	{
		kType_Linear = 0,
		kType_Exp,
		kType_Log
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Preset
{
public:
	WSPX_Collection_LFO lfos[2];
	ScopedPointer<WSPX_Image> imageIcon;
	WSPX_Collection_Envelope ampEnvelope;
	String name, tags, author, description;
	uint8_t keyZoneLow = 0;
	uint8_t keyZoneHigh = 127;
	uint8_t velZoneLow = 0;
	uint8_t velZoneHigh = 127;
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	char coarseTune = 0;
	uint8_t voices = 16;
	uint8_t output = 0; // Main Out //
	MemoryBlock scripting;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Position
{
public:
	String name;
	Array<int64> positions;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Image
{
public:
	Image image;
	Rectangle<int> position;
	//
	#if WSPXEDITOR
		File imageFilename;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection
{
public:
	ScopedPointer<WSPX_Position> presetTags;
	ScopedPointer<WSPX_Position> soundTags;
	ScopedPointer<WSPX_Image> imageAbout;
	ScopedPointer<WSPX_Image> imageIcon;
	//
	String name, description, author, company, tags, version;
	bool isProtected = false;
	//
	#if WSPXEDITOR
		OwnedArray<WSPX_Collection_Presets> presets;
		OwnedArray<WSPX_Collection_Sounds> sounds;
		//
		int16 editingPreset = 0;
		int16 editingSound = 0;
		File file;
	#else
		Array<int64> presetPositions;
		Array<int64> soundPositions;
	#endif
};