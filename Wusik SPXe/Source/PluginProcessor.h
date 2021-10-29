/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#pragma once
#include "WSPX Definitions.h"
#if WSPXPLAYERPREVIEW
	#include "WSPX Player/WSX Player.h"
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXThread: public ThreadWithProgressWindow
{
public:
	WSPXThread(void* __processor, Component* editor, int _type) :
		ThreadWithProgressWindow("Sound Thumb Processing", true, false, 0, String(), editor), _processor(__processor), type(_type) { }
	//
	void run() override;
	void* _processor;
	int type;
	//
	enum
	{
		kLoadPreset = 0
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikSpxAudioProcessor : public AudioProcessor
{
public:
    WusikSpxAudioProcessor();
    ~WusikSpxAudioProcessor();
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
	}
	//
	void resumeAudio()
	{
		suspendProcessing(false);
		// Restart the Synth engine //
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
	ScopedPointer<WSPX_Collection> collection;
	ScopedPointer<WSPXThread> processThread;
	AudioFormatManager audioFormatManager;
	String lastSoundFilePath;
	MidiKeyboardState midiKeyboardState;
	MidiKeyboardState midiKeyboardStateBottom;
	float lastSamplerate = 44100.0f;
	int lastSamplesPerBlock = 128;
	//
	#if WSPXPLAYERPREVIEW
		ScopedPointer<WSPX_Player_Preset> playerPreset;
	#endif
	//
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessor)
};
