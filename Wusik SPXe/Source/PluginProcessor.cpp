/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
extern String collectionFile = "";
extern bool WSPXeBundle = false;
#if WSPXEDITOR
	extern bool isWSPXEditor = true;
#else
	extern bool isWSPXEditor = false;
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikSpxAudioProcessor::WusikSpxAudioProcessor()
{
	audioFormatManager.registerBasicFormats();
	newCompilation();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikSpxAudioProcessor::~WusikSpxAudioProcessor()
{
	//
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSamplerate = sampleRate;
	lastSamplesPerBlock = samplesPerBlock;
	//
	#if WSPXPLAYERPREVIEW
		if (playerPreset != nullptr && playerPreset->isReady.get() == 1) playerPreset->prepareToPlay(sampleRate, samplesPerBlock);
	#endif
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
AudioProcessorEditor* WusikSpxAudioProcessor::createEditor()
{
    return new WusikSpxAudioProcessorEditor (*this);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	MemoryOutputStream stream(destData, false);
	saveCompilation(stream);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	#if WIGNORE_SET_STATE
		return;
	#endif
	//
	MemoryInputStream stream(data, sizeInBytes, false);
	loadCompilation(stream);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new WusikSpxAudioProcessor();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	lastSamplerate = getSampleRate();
	//
	midiKeyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
	//
	#if WSPXPLAYERPREVIEW
		if (playerPreset != nullptr && playerPreset->isReady.get() == 1) 
		{
			if (allNotesOff) 
			{ 
				playerPreset->reset(); 
				allNotesOff = false; 
			}
			//
			playerPreset->processBlock(buffer, midiMessages);
		}
	#endif
}