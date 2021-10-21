/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
#if WSPXEDITOR
	extern bool isWSPXEditor = true;
#else
	extern bool isWSPXEditor = false;
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikSpxAudioProcessor::WusikSpxAudioProcessor()
{
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
	//
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
	//MemoryInputStream stream(data, sizeInBytes, false);
	//loadCompilation(stream);
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
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();
	//
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
	//
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
	}
}