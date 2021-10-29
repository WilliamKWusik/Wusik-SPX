/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include  "WSPX Player/WSX Player.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXThread::run()
{
	WusikSpxAudioProcessor* processor = (WusikSpxAudioProcessor*) _processor;
	processor->stopAllAudio();
	double progress = 0.0;
	setProgress(progress);
	//
	if (type == kLoadPreset)
	{
		#if WSPXPLAYERPREVIEW
			//
			// Unload previous Sound //
			if (processor->playerPreset != nullptr)
			{
				for (int ll = 0; ll < processor->playerPreset->preset.layers.size(); ll++)
				{
					for (int ss = 0; ss < processor->playerPreset->preset.layers[ll]->soundLinks.size(); ss++)
					{
						for (int ff = 0; ff < processor->playerPreset->preset.layers[ll]->soundLinks[ss]->soundFiles.size(); ff++)
						{
							processor->playerPreset->preset.layers[ll]->soundLinks[ss]->soundFiles[ff]->soundData = nullptr;
						}
					}
				}
			}
			//
			// First count how many sounds we are about to load //
			//
			int totalSounds = 0;
			//
			for (int ll = 0; ll < processor->collection->lastSelectedPreset->layers.size(); ll++)
			{
				for (int ss = 0; ss < processor->collection->lastSelectedPreset->layers[ll]->soundLinks.size(); ss++)
				{
					totalSounds += processor->collection->lastSelectedPreset->layers[ll]->soundLinks[ss]->soundFiles.size();
				}
			}
			//
			// Now we load the actual sounds //
			//
			bool allOK = true;
			double progressRatio = 1.0 / double(totalSounds);
			for (int ll = 0; ll < processor->collection->lastSelectedPreset->layers.size(); ll++)
			{
				if (!allOK) break;
				//
				for (int ss = 0; ss < processor->collection->lastSelectedPreset->layers[ll]->soundLinks.size(); ss++)
				{
					if (!allOK) break;
					//
					for (int ff = 0; ff < processor->collection->lastSelectedPreset->layers[ll]->soundLinks[ss]->soundFiles.size(); ff++)
					{
						if (!allOK) break;
						WSPX_Collection_Sound_File* sound = processor->collection->lastSelectedPreset->layers[ll]->soundLinks[ss]->soundFiles[ff];
						//
						if (sound->soundData == nullptr)
						{
							int channelPosition = 0;
							WSPX_Samples_Float_32bits* soundData = new WSPX_Samples_Float_32bits;
							sound->soundData = soundData;
							soundData->setSize(sound->totalChannels, sound->totalSamples);
							//
							for (int cc = 0; cc < sound->files.size(); cc++)
							{
								ScopedPointer<AudioFormatReader> reader = processor->audioFormatManager.createReaderFor(sound->files[cc]->filename);
								//
								if (reader == nullptr)
								{
									WMessageBox("Error loading sound file!", sound->files[cc]->filename);
									allOK = false;
									break;
								}
								else
								{
									AudioSampleBuffer samples(reader->numChannels, sound->totalSamples);
									reader->read(&samples, 0, sound->totalSamples, 0, true, true);
									//
									for (int ch = 0; ch < reader->numChannels; ch++)
									{
										soundData->samples.copyFrom(channelPosition, 0, samples, ch, 0, sound->totalSamples);
										channelPosition++;
									}
								}
								//
								progress += progressRatio;
								setProgress(progress);
							}
						}
					}
				}
			}
			//
			if (allOK)
			{
				processor->playerPreset = new WSPX_Player_Preset(*processor->collection->lastSelectedPreset, _processor);
				processor->playerPreset->prepareToPlay(processor->lastSamplerate, processor->lastSamplesPerBlock);
			}
			//
		#else
			WMessageBox("NOT AVAILABLE", "The open source code does not include the Player code.");
		#endif
	}
	//
	processor->resumeAudio();
	WAlert;
	processor->processThread = nullptr;
}