
/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
#if WSPXPLAYERPREVIEW
	#include "WSPX Player/WSX Player.h"
	#include "WSPX Player/WSX Player UI.h"
#endif
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXThread::run()
{
	WusikSpxAudioProcessor* processor = (WusikSpxAudioProcessor*) _processor;
	processor->stopAllAudio();
	double progress = 0.0;
	setProgress(progress);
	sleep(50);
	//
	if (type == kSaveBundle)
	{
		setProgress(2.0f);
		FileOutputStream stream(processor->collection->file);
		processor->saveCompilation(stream);
	}
	else if (type == kLoadPreset)
	{
		#if WSPXPLAYERPREVIEW
			if (!SystemStats::hasSSE2())
			{
				WMessageBox("Can't start preset engine!", "SSE2 is required!");
				processor->resumeAudio();
				return;
			}
			//
			// Unload previous Sound //
			if (processor->playerPreset != nullptr) processor->unloadSounds(&processor->playerPreset->preset);
			//
			// Count how many sounds we are about to load //
			int totalSounds = 0;
			//
			for (int ll = 0; ll < processor->collection->lastSelectedPreset->layers.size(); ll++)
			{
				for (int ss = 0; ss < processor->collection->lastSelectedPreset->layers[ll]->soundLinks.size(); ss++)
				{
					for (int xs = 0; xs < processor->collection->lastSelectedPreset->layers[ll]->soundLinks[ss]->soundFiles.size(); xs++)
					{
						totalSounds += processor->collection->lastSelectedPreset->layers[ll]->soundLinks[ss]->soundFiles[xs]->files.size();
					}
				}
			}
			//
			// Load the actual sounds //
			bool allOK = true;
			double progressRatio = 1.0 / double(totalSounds);
			//
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
						if (sound->soundData.getSize() == 0 && (sound->totalSamples * sound->channelsInfo.size()) > 0)
						{
							int samplePosition = 0;
							sound->soundData.setSize(sound->totalSamples * sound->channelsInfo.size() * sizeof(float));
							sound->bits = 32;
							//
							for (int cc = 0; cc < sound->files.size(); cc++)
							{
								ScopedPointer<AudioFormatReader> reader = processor->audioFormatManager.createReaderFor(sound->files[cc]);
								//
								if (reader == nullptr)
								{
									WMessageBox("Error loading sound file!", sound->files[cc]);
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
										float* samplesDestinaton = (float*) sound->soundData.getData();
										const float* samplesSource = samples.getReadPointer(ch);
										//
										for (int ss = 0; ss < sound->totalSamples; ss++)
										{
											samplesDestinaton[samplePosition] = samplesSource[ss];
											samplePosition++;
										}
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
				processor->playerPreset->isReady.set(1);
				openPlayerUI();
			}
			//
		#else
			WMessageBox("NOT AVAILABLE", "The open source code does not include the Player code.");
		#endif
	}
	//
	sleep(50);
	processor->resumeAudio();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXThread::openPlayerUI()
{
	WusikSpxAudioProcessor* processor = (WusikSpxAudioProcessor*)_processor;
	//
	if (processor->openUIWhenLoadingPreset)
	{
		processor->playerUI = new WSXPlayerUIWindow();
		WSXPlayerUI* playerUI = new WSXPlayerUI(processor->collection, processor->playerPreset);
		playerUI->setBounds(0, 0, playerUI->getWidth(), playerUI->getHeight());
		processor->playerUI->setContentOwned(playerUI, true);
		processor->playerUI->centreAroundComponent(processor->getActiveEditor(), playerUI->getWidth(), playerUI->getHeight());
		processor->playerUI->setAlwaysOnTop(true);
		processor->playerUI->setVisible(true);
		processor->openUIWhenLoadingPreset = false;
	}
}