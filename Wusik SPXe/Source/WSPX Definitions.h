/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#define WIGNORE_SET_STATE 0
#define WVERSION "1.0.0 BETA 00"
extern bool isWSPXEditor;
extern String collectionFile;
//
// ------------------------------------------------------------------------------------------------------------------------- //
#if WSPXEDITOR
	class WS // Small Static Class to handle streams of data (read and write) //
	{
	public:
		static void stream(void* _stream, float& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeFloat(_value); else _value = ((InputStream*)_stream)->readFloat(); }
		static void stream(void* _stream, bool& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeBool(_value); else _value = ((InputStream*)_stream)->readBool(); }
		static void stream(void* _stream, uint8_t& _value, int _type) { if (_type == kWrite) ((OutputStream*)_stream)->writeByte(_value); else _value = ((InputStream*)_stream)->readByte(); }
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
		static void streamRelativePath(void* _stream, String& _value, int _type) 
		{ 
			if (_type == kWrite) ((OutputStream*)_stream)->writeString(pathRelativeToCollection(_value));
			else _value = pathFromCollectionLocation(((InputStream*)_stream)->readString());
		}
		//
		static String pathRelativeToCollection(File path)
		{
			return path.getRelativePathFrom(File(collectionFile).getParentDirectory());
		}
		//
		static String pathFromCollectionLocation(File path)
		{
			AlertWindow::showMessageBox(AlertWindow::NoIcon, "", collectionFile);

			AlertWindow::showMessageBox(AlertWindow::NoIcon, path.getFullPathName(),
				File(collectionFile).getParentDirectory().getChildFile(path.getFullPathName()).getFullPathName());

			AlertWindow::showMessageBox(AlertWindow::NoIcon, path.getFullPathName(),
			path.getChildFile(File(collectionFile).getParentDirectory().getFullPathName()).getFullPathName());

			return path.getChildFile(File(collectionFile).getParentDirectory().getFullPathName()).getFullPathName();
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
	void streamData(void* stream, int type);
	//
	MemoryBlock soundData;
	int64 totalSamples = 0;
	bool roundRobin = false;
	bool random = false;
	bool isRelease = false;
	bool reverse = false;
	int keySwitch = 0;
	int keySwitchType = 0;
	int64 loopStart = 0;
	int64 loopEnd = 0;
	int loopType = 0;
	float randomProbability = 1.0f;
	int keyZoneLow = 0;
	int keyZoneHigh = 127;
	int velZoneLow = 0;
	int velZoneHigh = 127;
	int keyRoot = 60;
	float fineTune = 0.0f;
	float coarseTune = 0.0f;
	int bits = 24;
	int format = 0;
	int channels = 0;
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
		int exportBits = 24;
		int exportFormat = 0;
		File soundFile;
	#endif
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Sound_Group
{
public:
	void streamData(void* stream, int type)
	{
		int totalSounds = sounds.size();
		WS::stream(stream, totalSounds, type);
		WS::stream(stream, name, type);
		WS::stream(stream, tags, type);
		WS::stream(stream, chokeGroup, type);
		//
		for (int ss = 0; ss < totalSounds; ss++)
		{
			if (type == WS::kRead) sounds.add(new WSPX_Collection_Sound);
			sounds[ss]->streamData(stream, type);
		}
	};
	//
	OwnedArray<WSPX_Collection_Sound> sounds;
	int chokeGroup = 0;
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
	void streamData(void* stream, int type)
	{
		//String fileName = imageFilename.getFullPathName();
		//WS//::stream(stream, fileName, type);
		//if (type == WS::kRead) imageFilename = fileName;
		WS::streamRelativePath(stream, imageFilename, type);
		if (File(imageFilename).existsAsFile()) image = ImageFileFormat::loadFrom(imageFilename);
	};
	//
	Image image;
	String imageFilename;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_LFO
{
public:
	void streamData(void* stream, int _type)
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
	//
	int waveform = 0;
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
	int type = 0;
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
	int type = 0;
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
	int time = 1;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Sequencer
{
public:
	void streamData(void* stream, int type);
	//
	OwnedArray<WSPX_Sequencer_Step> steps;
	bool syncBPM = true;
	float time1 = 1.0f;
	float time2 = 8.0f;
	int loopStart = 0;
	float smoothOutput = 0.0f;
	int type = 0;
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
	void streamData(void* stream, int type);
	//
	WSPX_Sequencer sequencer;
	OwnedArray<WSPX_Channel> channels;
	Array<int> soundGroupIDs;
	WSPX_Collection_LFO lfos[2];
	WSPX_Collection_Envelope ampEnvelope;
	WSPX_Collection_Filter filter;
	bool reverse = false;
	int keyZoneLow = 0;
	int keyZoneHigh = 127;
	int velZoneLow = 0;
	int velZoneHigh = 127;
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	float coarseTune = 0.0f;
	int voices = 16;
	int output = 0; // Main Out //
	float glide = 0.0f;
	bool autoGlide = true;
	MemoryBlock scripting;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPX_Collection_Preset
{
public:
	void streamData(void* stream, int type);
	//
	OwnedArray<WSPX_Collection_Preset_Layer> layers;
	OwnedArray<WSPX_Collection_Effect> effects;
	WSPX_Image imagePresetIcon;
	String name, tags, author, description;
	float volume = 1.0f;
	float pan = 0.0f;
	float fineTune = 0.0f;
	float coarseTune = 0;
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
	WSPX_Image imageAbout;
	WSPX_Image imageIcon;
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