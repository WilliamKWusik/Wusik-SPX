/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
#if WSPXEDITOR
	class WS // Small Static Class to handle streams of data (read and write) //
	{
	public:
		static void stream(void* _stream, float& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeFloat(_value); else _value = ((InputStream*)_stream)->readFloat(); }
		static void stream(void* _stream, bool& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeBool(_value); else _value = ((InputStream*)_stream)->readBool(); }
		static void stream(void* _stream, char& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeByte(_value); else _value = ((InputStream*)_stream)->readByte(); }
		static void stream(void* _stream, uint8_t& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeByte(_value); else _value = ((InputStream*)_stream)->readByte(); }
		static void stream(void* _stream, String& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeString(_value); else _value = ((InputStream*)_stream)->readString(); }
		static void stream(void* _stream, File& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeString(_value.getFullPathName()); else _value = ((InputStream*)_stream)->readString(); }
		static void stream(void* _stream, int& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeInt(_value); else _value = ((InputStream*)_stream)->readInt(); }
		//
		static void stream(void* _stream, MemoryBlock& _value, int _type) 
		{ 
			if (_type == kWrite)
			{
				int64 _size = _value.getSize();
				((OutputStream*)_stream)->writeInt64(_size);
				((OutputStream*)_stream)->write(_value.getData(), _value.getSize());
			}
			else
			{
				int64 _size = ((InputStream*)_stream)->readInt64();
				_value.setSize(_size);
				if (_size > 0) ((InputStream*)_stream)->read(_value.getData(), (size_t) _size);
			}
		}
		//
		enum { kRead, kWrite };
	};
#endif
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
		void streamWSPXe(void* stream, int type)
		{
			String fileName = imageFilename.getFullPathName();
			WS::stream(stream, fileName, type);
			if (type == WS::kRead) imageFilename = fileName;
			//if (imageFilename.existsAsFile()) image = ImageFileFormat::loadFrom(imageFilename);
		};
		//
		File imageFilename;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_LFO
{
public:
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
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int _type)
		{
			WS::stream(stream, waveform, _type);
			WS::stream(stream, sync, _type);
			WS::stream(stream, inverted, _type);
			WS::stream(stream, noteOnReset, _type);
			WS::stream(stream, speed1, _type);
			WS::stream(stream, speed2, _type);
			WS::stream(stream, phase, _type);
			WS::stream(stream, smooth, _type);
			WS::stream(stream, toVolume, _type);
			WS::stream(stream, toFilterFreq, _type);
		}
	#endif
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
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int _type)
		{
			WS::stream(stream, type, _type);
			WS::stream(stream, attack, _type);
			WS::stream(stream, decay, _type);
			WS::stream(stream, sustain, _type);
			WS::stream(stream, release, _type);
			WS::stream(stream, velocity, _type);
			WS::stream(stream, maxSeconds, _type);
			WS::stream(stream, keyTrack, _type);
			WS::stream(stream, velTrack, _type);
		}
	#endif
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
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int _type)
		{
			WS::stream(stream, type, _type);
			WS::stream(stream, frequency, _type);
			WS::stream(stream, rezonance, _type);
			WS::stream(stream, smooth, _type);
			WS::stream(stream, drive, _type);
			WS::stream(stream, sampleAndHold, _type);
			WS::stream(stream, feedback, _type);
			WS::stream(stream, delay, _type);
			WS::stream(stream, toEnvelope, _type);
			//
			envelope.streamWSPXe(stream, _type);
		}
	#endif
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
	//
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int type)
		{
			WS::stream(stream, volume, type);
			WS::stream(stream, pan, type);
			WS::stream(stream, fine, type);
			WS::stream(stream, tune, type);
			WS::stream(stream, filterFreq, type);
			WS::stream(stream, time, type);
		}
	#endif
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
	int loopStart = 0;
	float smoothOutput = 0.0f;
	char type = 0;
	//
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int type);
	#endif
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
	WSPX_Sequencer sequencer;
	OwnedArray<WSPX_Channel> channels;
	Array<int> soundGroupIDs;
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
	//
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int type);
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Preset
{
public:
	OwnedArray<WSPX_Collection_Preset_Layer> layers;
	OwnedArray<WSPX_Collection_Effect> effects;
	WSPX_Image imagePresetIcon;
	String name, tags, author, description;
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	char coarseTune = 0;
	//
	#if WSPXEDITOR
		void streamWSPXe(void* stream, int type);
	#endif
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
		void streamWSPXe(void* stream, int type);
		//
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