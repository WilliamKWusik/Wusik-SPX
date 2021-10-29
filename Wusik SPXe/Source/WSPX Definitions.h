/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#define WIGNORE_SET_STATE 1
#define WVERSION "1.0.0 BETA 00"
#define XRGKEY "HKEY_CURRENT_USER\\Software\\Wusik\\Wusik SPXe\\"
extern bool isWSPXEditor;
extern bool WSPXeBundle;
extern String collectionFile;
//
// ------------------------------------------------------------------------------------------------------------------------- //
#if WSPXEDITOR
	class WS // Small Static Class to handle streams of data (read and write) //
	{
	public:
		static void stream(void* _stream, float& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeFloat(_value); else _value = ((InputStream*)_stream)->readFloat(); }
		static void stream(void* _stream, bool& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeBool(_value); else _value = ((InputStream*)_stream)->readBool(); }
		static void stream(void* _stream, String& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeString(_value); else _value = ((InputStream*)_stream)->readString(); }
		static void stream(void* _stream, File& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeString(_value.getFullPathName()); else _value = ((InputStream*)_stream)->readString(); }
		static void stream(void* _stream, int& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeInt(_value); else _value = ((InputStream*)_stream)->readInt(); }
		static void stream(void* _stream, int64& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeInt64(_value); else _value = ((InputStream*)_stream)->readInt64(); }
		//
		static void stream(void* _stream, MemoryBlock& _value, int _type) 
		{ 
			if (_type == kWrite)
			{
				int64 _size = _value.getSize();
				((OutputStream*)_stream)->writeInt64(_size);
				if (_size > 0) ((OutputStream*)_stream)->write(_value.getData(), _value.getSize());
			}
			else
			{
				int64 _size = ((InputStream*)_stream)->readInt64();
				_value.setSize(_size);
				if (_size > 0) ((InputStream*)_stream)->read(_value.getData(), (size_t) _size);
			}
		}
		//
		static String pathRelativeToCollection(File path) { return path.getRelativePathFrom(File(collectionFile).getParentDirectory()); }
		static String pathFromCollectionLocation(String path) { return File(collectionFile).getParentDirectory().getChildFile(path).getFullPathName(); }
		//
		static void streamRelativePath(void* _stream, String& _value, int _type)
		{ 
			if (_type == kWrite) ((OutputStream*)_stream)->writeString(pathRelativeToCollection(_value));
			else _value = pathFromCollectionLocation(((InputStream*)_stream)->readString());
		}
		//
		static String getSize(int64 size)
		{
			if (size < 1024) return String(size) + " Bytes";
			else if (size < (1024 * 1024)) return String(double(size) / 1024.0, 2) + " KB";
			else if (size < (1024 * 1024 * 1024)) return String(double(size) / 1024.0 / 1024, 2) + " MB";
			else return String(double(size) / 1024.0 / 1024.0 / 1024, 2) + " GB";
		}
		//
		enum { kRead, kWrite };
	};
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Channel_Info
{
public:
	void streamData(void* stream, int type)
	{
		WS::stream(stream, volume, type);
		WS::stream(stream, pan, type);
		WS::stream(stream, name, type);
	}
	//
	float volume = 1.0f;
	float pan = 0.0f;
	String name;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sound_File_Filename
{
public:
	void streamData(void* stream, int type)
	{
		WS::stream(stream, channels, type);
		WS::streamRelativePath(stream, filename, type);
		//
		int totalChannelsInfo = channelsInfo.size();
		WS::stream(stream, totalChannelsInfo, type);
		//
		for (int cc = 0; cc < totalChannelsInfo; cc++)
		{
			if (type == WS::kRead) channelsInfo.add(new WSPX_Channel_Info);
			channelsInfo[cc]->streamData(stream, type);
		}
	}
	//
	int channels = 1;
	String filename;
	OwnedArray<WSPX_Channel_Info> channelsInfo;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Samples
{
public:
	inline virtual void* getSamples(int _channel) { return nullptr; }
	virtual void setSize(int _channels, int _samples) { }
	int type = kEmpty;
	//
	enum
	{
		kEmpty = 0,
		kFloat_32bits,
		kInt_24bits,
		kInt_16bits
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Samples_Float_32bits : public WSPX_Samples
{
public:
	WSPX_Samples_Float_32bits() { type = kFloat_32bits; }
	inline void* getSamples(int _channel) override { return samples.getWritePointer(_channel); }
	void setSize(int _channels, int _samples) override { samples.setSize(_channels, _samples); }
	AudioSampleBuffer samples;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sound_File
{
public:
	void streamData(void* stream, int type);
	//
	ScopedPointer<WSPX_Samples> soundData; // The type of storage will depend on the format: float, int24, int32 //
	float volume = 1.0f;
	float pan = 0.0f;
	bool roundRobin = false;
	bool random = false;
	bool release = false;
	bool reverse = false;
	int keySwitch = 0;
	float randomProbability = 1.0f;
	float fineTune = 0.0f;
	int coarseTune = 0;
	int keyZoneLow = 0;
	int keyZoneHigh = 127;
	int velZoneLow = 0;
	int velZoneHigh = 127;
	int keyRoot = 60;
	//
	int keySwitchType = 0;
	int64 loopStart = 0;
	int64 loopEnd = 0;
	int loopType = 0;
	int bits = 24; // 16 (int16), 24 (int16 + int8) or 32 bits (float) //
	int format = 0;
	int totalChannels = 1;
	//
	bool sampleDataMetaValuesRead = false;
	int64 totalSamples = 0;
	int sampleRate = 44100;
	//
	const String formats = "Binary\nFlac\nGZIP";
	const String keySwitchTypes = "Normal\nMomentary\nLatch";
	const String loopTypes = "Normal\nPingPong";
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
		int exportBits = 24;
		int exportFormat = 0;
		OwnedArray<WSPX_Collection_Sound_File_Filename> files;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sound
{
public:
	void streamData(void* stream, int type)
	{
		int totalSounds = soundFiles.size();
		WS::stream(stream, totalSounds, type);
		WS::stream(stream, name, type);
		WS::stream(stream, tags, type);
		WS::stream(stream, chokeGroup, type);
		//
		for (int ss = 0; ss < totalSounds; ss++)
		{
			if (type == WS::kRead) soundFiles.add(new WSPX_Collection_Sound_File);
			soundFiles[ss]->streamData(stream, type);
		}
	}
	//
	OwnedArray<WSPX_Collection_Sound_File> soundFiles;
	float chokeGroup = 0.0f;
	String name = "Init Sound";
	String tags;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Parameter
{
public:
	WSPX_Parameter(int _type, void* _value, float _min, float _max) : type(_type), value(_value), min(_min), max(_max) { }
	//
	void setValue(float newValue) { ((float*)value)[0] = newValue; }
	void setValue(int newValue) { ((int*)value)[0] = newValue; }
	void setValue(bool newValue) { ((bool*)value)[0] = newValue; }
	//
	float getFloat() { return ((float*)value)[0]; }
	int getInt() { return ((int*)value)[0]; }
	int getBool() { return ((bool*)value)[0]; }
	//
	void* value = nullptr;
	String name = "None";
	float min = 0.0f;
	float max = 1.0f;
	int type;
	//
	enum
	{
		kFloat = 1,
		kInteger,
		kBool
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Effect
{
public:
	virtual void streamData(void* stream, int _type) { }
	OwnedArray<WSPX_Parameter> parameters;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Effect
{
public:
	void streamData(void* stream, int _type);
	//
	ScopedPointer<WSPX_Effect> effect[4]; // Up to 4 effects running in series or parallel //
	const String types = "None\nReverb\nChorus\nDelay\nDistortion\nPhaser\nFilter\nConvolution";
	int type = kNone;
	float dry = 0.0f;
	float wet = 1.0f;
	float parallel = 0.0f;
	//
	enum
	{
		kNone = 0,
		kReverb,
		kChorus,
		kDelay,
		kDistortion,
		kPhaser,
		kFilter,
		kConvolution
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Image
{
public:
	void streamData(void* stream, int type)
	{
		WS::streamRelativePath(stream, imageFilename, type);
		if (File(imageFilename).existsAsFile()) image = ImageFileFormat::loadFrom(imageFilename);
	}
	//
	Image image;
	String imageFilename;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Time
{
public:
	void streamData(void* stream, int _type) { WS::stream(stream, speed, _type); }
	String speed = "1/4";
};
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_LFO
{
public:
	void streamData(void* stream, int _type)
	{
		time.streamData(stream, _type);
		WS::stream(stream, waveform, _type);
		WS::stream(stream, inverted, _type);
		WS::stream(stream, noteOnReset, _type);
		WS::stream(stream, phase, _type);
		WS::stream(stream, smooth, _type);
		WS::stream(stream, toVolume, _type);
		WS::stream(stream, toFilterFreq, _type);
		WS::stream(stream, toPan, _type);
		WS::stream(stream, toEffect1, _type);
		WS::stream(stream, toEffect2, _type);
		WS::stream(stream, toEffect3, _type);
		WS::stream(stream, toEffect4, _type);
	}
	//
	const String waveforms = "Sine\nSawtooth\nPulse\nRandom\nNoise\nTriangle";
	WSPX_Time time;
	int waveform = 0;
	bool inverted = false;
	bool noteOnReset = false;
	float phase = 0.0f;
	float smooth = 0.0f;
	float toVolume = 0.0f;
	float toFilterFreq = 0.0f;
	float toPan = 0.0f;
	float toEffect1 = 0.0;
	float toEffect2 = 0.0;
	float toEffect3 = 0.0;
	float toEffect4 = 0.0;
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
	void streamData(void* stream, int _type)
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
	//
	const String types = "Linear\nExp\nLog";
	int type = 0;
	float attack = 0.0f;
	float decay = 0.0f;
	float sustain = 1.0f;
	float release = 0.28f;
	float velocity = 1.0f;
	int maxSeconds = 4;
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
	void streamData(void* stream, int _type)
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
		envelope.streamData(stream, _type);
	}
	//
	WSPX_Collection_Envelope envelope;
	const String types = "LowPass\nBandPass\nHighPass\nNotch";
	int type = 0;
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
	void streamData(void* stream, int type)
	{
		WS::stream(stream, volume, type);
		WS::stream(stream, pan, type);
		WS::stream(stream, fine, type);
		WS::stream(stream, tune, type);
		WS::stream(stream, filterFreq, type);
		WS::stream(stream, time, type);
	}
	//
	float volume = 1.0f;
	float pan = 0.0f;
	float fine = 0.0f;
	int tune = 0;
	float filterFreq = 0.0f;
	int time = 0;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Sequencer
{
public:
	void streamData(void* stream, int type);
	//
	const String modes = "Forward\nBackwards\nPingPong\nRandom";
	OwnedArray<WSPX_Sequencer_Step> steps;
	WSPX_Time time;
	int loopStart = 0;
	float smoothOutput = 0.0f;
	int mode = 0;
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
class WSPX_Collection_Preset_Layer
{
public:
	void streamData(void* stream, int type, OwnedArray<WSPX_Collection_Sound>& soundsList);
	//
	WSPX_Sequencer sequencer;
	Array<WSPX_Collection_Sound*> soundLinks;
	WSPX_Collection_LFO lfos[2];
	WSPX_Collection_Envelope ampEnvelope;
	WSPX_Collection_Filter filter;
	WSPX_Collection_Effect effects;
	String name = "Layer";
	bool reverse = false;
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	int coarseTune = 0;
	int voices = 16;
	int output = 0; // Main Out //
	float glide = 0.0f;
	bool autoGlide = true;
	int keyZoneLow = 0;
	int keyZoneHigh = 127;
	int velZoneLow = 0;
	int velZoneHigh = 127;
	MemoryBlock scripting;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Preset
{
public:
	void streamData(void* stream, int type, OwnedArray<WSPX_Collection_Sound>& soundsList);
	//
	OwnedArray<WSPX_Collection_Preset_Layer> layers;
	WSPX_Collection_Effect effects[4]; // Up to 4 Sends with up to 4 internal effects - each Layer has up to 4 insert effects //
	WSPX_Image imagePresetIcon;
	String name = "Init Preset";
	String tags, author, description;
	MemoryBlock scripting;
	//
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	int coarseTune = 0;
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
	void streamData(void* stream, int type);
	//
	MemoryBlock playerSkin;
	WSPX_Image imageAbout;
	WSPX_Image imageIcon;
	String name = "Init Collection";
	String description, author, company, tags, version;
	String protectionKey;
	File file;
	//
	#if WSPXEDITOR
		OwnedArray<WSPX_Collection_Preset> presets;
		OwnedArray<WSPX_Collection_Sound> sounds;
		WSPX_Collection_Preset* lastSelectedPreset = nullptr;
		bool hasUnsavedChanges = false;
		File exportedFile;
		bool isWSPXeBundle = false;
		File playerSkinFolder;
	#else
		ScopedPointer<WSPX_Collection_Preset> preset;
		//
		Array<int64> presetPositions;
		Array<int64> soundPositions;
	#endif
};