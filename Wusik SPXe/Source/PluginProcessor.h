/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "WSPX Definitions.h"
#if WSPXPLAYERPREVIEW
	#include "WSPX Player/WSX Player.h"
	#pragma pack(32)
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXThread: public ThreadWithProgressWindow
{
public:
	WSPXThread(void* __processor, Component* editor, int _type, String title) :
		ThreadWithProgressWindow(title, true, false, 0, String(), editor), _processor(__processor), type(_type) { }
	//
	void run() override;
	void* _processor;
	int type;
	//
	enum
	{
		kLoadPreset = 0,
		kSaveBundle
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikSpxAudioProcessor : public AudioProcessor
{
public:
	DECLARE_ALIGNED(32);
	//
    WusikSpxAudioProcessor();
    ~WusikSpxAudioProcessor();
	//
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override { }
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override { return true; }
	const String getName() const override { return JucePlugin_Name; }
	bool acceptsMidi() const override { return true; }
	bool producesMidi() const override { return false; }
	bool isMidiEffect() const override { return false; }
	double getTailLengthSeconds() const override { return 0.0; }
	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int index) override { }
	const String getProgramName(int index) override { return "Default"; }
	void changeProgramName(int index, const String& newName) override { }
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	void newCompilation();
	bool loadCompilation(InputStream& stream);
	void saveCompilation(OutputStream& stream);
	void loadSoundFileDetails(WSPX_Collection_Sound_File* soundFile);
	void loadPreset(bool forceLoad = false);
	void addSoundFiles(Array<File>& files, WSPX_Collection_Sound& sound, void* treeItem);
	int pickSound();
	//
	String getLastSoundFilePath(String inputFile = String())
	{
		if (inputFile.isNotEmpty() && File(inputFile).existsAsFile()) lastSoundFilePath = File(inputFile).getParentDirectory().getFullPathName();
		return lastSoundFilePath;
	}
	//
	void stopAllAudio()
	{
		suspendProcessing(true);
		// Reset any Synth information Here //
		#if WSPXPLAYERPREVIEW
			if (playerPreset != nullptr && playerPreset->isReady.get() == 1) playerPreset->voices.clear();
		#endif	
	}
	//
	void resumeAudio()
	{
		suspendProcessing(false);
		// Restart the Synth engine //
		#if WSPXPLAYERPREVIEW
			if (playerPreset != nullptr && playerPreset->isReady.get() == 1) playerPreset->voices.clear();
		#endif	
	}
	//
	String readGlobalSettings(String key, String defaultVal)
	{
		String sKey(XRGKEY + key);
		if (WindowsRegistry::keyExists(sKey) &&
			WindowsRegistry::getValue(XRGKEY + key).isNotEmpty())
		{
			return WindowsRegistry::getValue(XRGKEY + key);
		}
		//
		return defaultVal;
	}
	//
	void saveGlobalSettings(String key, String value)
	{
		WindowsRegistry::setValue(XRGKEY + key, value);
	}
	//
	void deleteGlobalSettings(String key)
	{
		saveGlobalSettings(key, "");
		if (WindowsRegistry::keyExists(XRGKEY + key)) WindowsRegistry::deleteKey(XRGKEY + key);
	}
	//
	void unloadSounds(WSPX_Collection_Preset* preset)
	{
		for (int ll = 0; ll < preset->layers.size(); ll++)
		{
			for (int ss = 0; ss < preset->layers[ll]->soundLinks.size(); ss++)
			{
				for (int ff = 0; ff < preset->layers[ll]->soundLinks[ss]->soundFiles.size(); ff++)
				{
					preset->layers[ll]->soundLinks[ss]->soundFiles[ff]->soundData.reset();
				}
			}
		}
	}
	//
	ScopedPointer<WSPX_Collection> collection;
	ScopedPointer<WSPXThread> processThread;
	AudioFormatManager audioFormatManager;
	String lastSoundFilePath;
	MidiKeyboardState midiKeyboardState;
	float lastSamplerate = 44100.0f;
	int lastSamplesPerBlock = 128;
	bool allNotesOff = false;
	//
	#if WSPXPLAYERPREVIEW
		alignas(32) ScopedPointer<WSPX_Player_Preset> playerPreset; // We align the data so variables can be aligned to SSE/AVX requirements //
	#endif
	//
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessor)
};