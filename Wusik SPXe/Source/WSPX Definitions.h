/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sound
{
public:
	MemoryBlock samples;
	bool roundRobin = false;
	bool random = false;
	bool isRelease = false;
	bool reverse = false;
	uint8_t keySwitch = 0;
	uint8_t keySwitchType = 0;
	int64 loopStart = 0;
	int64 loopEnd = 0;
	uint8_t loopType = 0;
	float randomProbability = 1.0f;
	uint8_t keyZoneLow = 0;
	uint8_t keyZoneHigh = 127;
	uint8_t velZoneLow = 0;
	uint8_t velZoneHigh = 127;
	uint8_t keyRoot = 60;
	float fineTune = 0.0f;
	char coarseTune = 0;
	uint8_t bits = 24;
	uint8_t format = 0;
	uint8_t channels = 0;
	Array<float> channelPan;
	//
	enum
	{
		kKeySwitch_Normal = 0,
		kKeySwitch_Momentary,
		kKeySwitch_Latch,
		//
		kLoopType_Normal = 0,
		kLoopType_PingPong,
		//
		kFormat_Binary = 0,
		kFormat_Flac,
		kFormat_GZIP
	};
	//
	#if WSPXEDITOR
		uint8_t exportBits = 24;
		uint8_t exportFormat = 0;
		File soundFile;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sound_Group
{
public:
	OwnedArray<WSPX_Collection_Sound> sounds;
	char chokeGroup = 0;
	String name, tags;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Effect
{
public:
	float dry = 0.0f;
	float wet = 1.0f;
	bool parallel = false;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Image
{
public:
	Image image;
	//
	#if WSPXEDITOR
		File imageFilename;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_LFO
{
public:
	void save(OutputStream& stream)
	{
		stream.writeByte(waveform);
		stream.writeBool(sync);
		stream.writeBool(inverted);
		stream.writeBool(noteOnReset);
		stream.writeFloat(speed1);
		stream.writeFloat(speed2);
		stream.writeFloat(phase);
		stream.writeFloat(smooth);
		stream.writeFloat(toVolume);
		stream.writeFloat(toFilterFreq);
	};
	//
	void load(InputStream& stream)
	{
		waveform = stream.readByte();
		sync = stream.readBool();
		inverted = stream.readBool();
		noteOnReset = stream.readBool();
		speed1 = stream.readFloat();
		speed2 = stream.readFloat();
		phase = stream.readFloat();
		smooth = stream.readFloat();
		toVolume = stream.readFloat();
		toFilterFreq = stream.readFloat();
	};
	//
	uint8_t waveform = 0;
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
	void save(OutputStream& stream)
	{
		stream.writeByte(type);
		stream.writeFloat(attack);
		stream.writeFloat(decay);
		stream.writeFloat(sustain);
		stream.writeFloat(release);
		stream.writeFloat(velocity);
		stream.writeFloat(maxSeconds);
		stream.writeFloat(keyTrack);
		stream.writeFloat(velTrack);
	};
	//
	void load(InputStream& stream)
	{
		type = stream.readByte();
		attack = stream.readFloat();
		decay = stream.readFloat();
		sustain = stream.readFloat();
		release = stream.readFloat();
		velocity = stream.readFloat();
		maxSeconds = stream.readFloat();
		keyTrack = stream.readFloat();
		velTrack = stream.readFloat();
	};
	//
	uint8_t type = 0;
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
class WSPX_Collection_Filter
{
public:
	void save(OutputStream& stream)
	{
		stream.writeByte(type);
		envelope.save(stream);
		stream.writeFloat(frequency);
		stream.writeFloat(rezonance);
		stream.writeFloat(smooth);
		stream.writeFloat(drive);
		stream.writeFloat(sampleAndHold);
		stream.writeFloat(feedback);
		stream.writeFloat(delay);
		stream.writeFloat(toEnvelope);
	};
	//
	void load(InputStream& stream)
	{
		type = stream.readByte();
		envelope.load(stream);
		frequency = stream.readFloat();
		rezonance = stream.readFloat();
		smooth = stream.readFloat();
		drive = stream.readFloat();
		sampleAndHold = stream.readFloat();
		feedback = stream.readFloat();
		delay = stream.readFloat();
		toEnvelope = stream.readFloat();
	};
	//
	uint8_t type = 0;
	WSPX_Collection_Envelope envelope;
	float frequency = 1.0f;
	float rezonance = 0.2f;
	float smooth = 0.4f;
	float drive = 0.0f;
	float sampleAndHold = 0.0f;
	float feedback = 0.0f;
	float delay = 0.0f;
	float toEnvelope = 0.0f;
	//
	enum
	{
		kType_LowPass = 0,
		kType_BandPass,
		kType_HighPass,
		kType_Notch
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Sequencer_Step
{
public:
	float volume = 1.0f;
	float pan = 0.0f;
	float fine = 0.0f;
	char tune = 0;
	float filterFreq = 0.0f;
	char time = 1;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Sequencer
{
public:
	OwnedArray<WSPX_Sequencer_Step> steps;
	bool syncBPM = true;
	float time1 = 1.0f;
	float time2 = 8.0f;
	int16 loopStart = 0;
	float smoothOutput = 0.0f;
	char type = 0;
	//
	enum
	{
		kForward = 0,
		kBackwards,
		kPingPong,
		kRandom
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Channel
{
public:
	float volume = 1.0f;
	String name;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Preset_Layer
{
public:
	ScopedPointer<WSPX_Sequencer> sequencer;
	OwnedArray<WSPX_Channel> channels;
	Array<int16> soundGroupIDs;
	WSPX_Collection_LFO lfos[2];
	WSPX_Collection_Envelope ampEnvelope;
	WSPX_Collection_Filter filter;
	bool reverse = false;
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
	float glide = 0.0f;
	bool autoGlide = true;
	MemoryBlock scripting;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Preset
{
public:
	OwnedArray<WSPX_Collection_Preset_Layer> layers;
	OwnedArray<WSPX_Collection_Effect> effects;
	ScopedPointer<WSPX_Image> imagePresetIcon;
	String name, tags, author, description;
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	char coarseTune = 0;
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
class WSPX_Collection
{
public:
	WSPX_Image imageAbout;
	WSPX_Image imageIcon;
	//
	String name, description, author, company, tags, version;
	String protectionKey;
	File file;
	//
	#if WSPXEDITOR
		OwnedArray<WSPX_Collection_Preset> presets;
		OwnedArray<WSPX_Collection_Sound_Group> soundGroups;
		bool hasUnsavedChanges = false;
		File exportedFile;
	#else
		ScopedPointer<WSPX_Collection_Preset> preset;
		//
		Array<int64> presetPositions;
		Array<int64> soundPositions;
	#endif
};