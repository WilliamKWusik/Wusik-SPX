/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/

#pragma once
#include "WSPX Definitions.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikSpxAudioProcessor  : public AudioProcessor
{
public:
    WusikSpxAudioProcessor();
    ~WusikSpxAudioProcessor();
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override { };
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override { return true; };
	const String getName() const override { return JucePlugin_Name; };
	bool acceptsMidi() const override { return true; };
	bool producesMidi() const override { return false; };
	bool isMidiEffect() const override { return false; };
	double getTailLengthSeconds() const override { return 0.0; }
	int getNumPrograms() override { return 1; };
	int getCurrentProgram() override { return 0; };
	void setCurrentProgram(int index) override { };
	const String getProgramName(int index) override { return "Default"; };
	void changeProgramName(int index, const String& newName) override { };
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	void newCompilation();
	void saveCompilation(OutputStream& stream);
	//
	ScopedPointer<WSPX_Collection> collection;
	//
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessor)
};
