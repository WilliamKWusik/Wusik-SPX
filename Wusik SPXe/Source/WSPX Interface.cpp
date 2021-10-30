/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::cleanInterface()
{
	if (redoTreeViewsOnResize)
	{
		presetsTreeView = nullptr;
		soundsTreeView = nullptr;
	}
	//
	editOptionsViewport = nullptr;
	editOptionsComponent = nullptr;
	//
	keyVelZones.clear();
	editOptions.clear();
	//
	statusLabel->setVisible(false);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::updateInterface()
{
	editOptionsComponent = nullptr;
	midiKeyboard.selectedHigh = midiKeyboard.selectedLow = midiKeyboard.rootKey = -1;
	//
	#define AddCompoLabel(label) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kLabel, label)))
	#define AddCompoLabelSM(label) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kLabelSmall, label)))
	#define AddCompoCallback(type, name, variable, label, callback) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, callback)))
	#define AddCompo(type, name, variable) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable)))
	#define AddCompo2(type, name, variable, label, showEditInsteead, callback) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, showEditInsteead, callback)))
	#define AddCompo4(type, name, variable, label, min, max) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, false, nullptr, min, max)))
	#define AddCompo6(type, name, variable, label, PopupList) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, false, nullptr, 0, 1, PopupList)))
	#define AddMIDIKey(value) editOptions.getLast()->slider->midiKeyboard = &midiKeyboard; editOptions.getLast()->slider->midiKeyboardValue = &midiKeyboard.value
	#define AddSequencerButton(name, type) AddCompo2(kButtonCallback, name, nullptr, "", false, new WusikEditOptionCallback_Sequencer_Step(sequencer, nullptr, WusikEditOptionCallback_Sequencer_Step::type))
	#define AddTime(variable) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Time", variable, "Enter time in 1 / 1 format for host sync or 10hz for free run(0.0001hz up to 99999hz")))
	//
	if (editObject.type == WusikEditObject::kCollection)
	{
		editOptionsComponent = new Component;
		processor.collection->lastSelectedPreset = nullptr;
		//
		AddCompoLabel("Collection Details");
		AddCompo2(kString, "Name", &processor.collection->name, "", false, new WusikEditOptionCallback_UpdateCollectionName);
		AddCompo(kString, "Author", &processor.collection->author);
		AddCompo(kString, "Version", &processor.collection->version);
		AddCompo(kString, "Company", &processor.collection->company);
		AddCompo2(kString, "Tags", &processor.collection->tags, "Separated by ;", false, nullptr);
		AddCompo2(kString, "Description", &processor.collection->description, "", true, nullptr);
		AddCompo(kImage, "Image Icon", &processor.collection->imageIcon);
		AddCompo(kImage, "Image About", &processor.collection->imageAbout);
		AddCompo(kSkinFolder, "Player Skin Folder", &processor.collection->playerSkinFolder);
		AddCompo2(kString, "Protection Key", &processor.collection->protectionKey, "", true, nullptr);
	}
	else if (editObject.type == WusikEditObject::kSound || editObject.type == WusikEditObject::kSoundLink)
	{
		WSPX_Collection_Sound* sound = (WSPX_Collection_Sound*)editObject.object;
		processor.collection->lastSelectedPreset = nullptr;
		editOptionsComponent = new Component;
		//
		if (editObject.type == WusikEditObject::kSound)
		{
			AddCompoLabel("Sound Details");
			AddCompo(kString, "Name", &sound->name);
			AddCompo(kString, "Tags", &sound->tags);
			AddCompo4(kSliderInteger, "Choke Group", &sound->chokeGroup, "", 0, 128);
		}
		else
		{
			AddCompoLabel("Sound Link Details");
			AddCompoLabelSM("Name: " + sound->name);
		}
		//
		AddCompoLabelSM("Total Of Sounds " + String(sound->soundFiles.size()));
		//
		int64 totalSize;
		for (int ss = 0; ss < sound->soundFiles.size(); ss++)
		{
			for (int ff = 0; ff < sound->soundFiles[ss]->files.size(); ff++)
			{
				String sFile = "File #" + String(ss + 1) + " ";
				if (sound->soundFiles[ss]->files.size() > 1) sFile.append("(channel set " + String(ff + 1) + ") ", 9999);
				//
				if (sound->soundFiles[ss]->files[ff]->filename.isEmpty())
				{
					sFile.append("No File", 9999);
				}
				else
				{
					sFile.append("\"" + File(sound->soundFiles[ss]->files[ff]->filename).getFileName() + "\"", 9999);
					//
					if (File(sound->soundFiles[ss]->files[ff]->filename).existsAsFile())
					{
						int64 xSize = File(sound->soundFiles[ss]->files[ff]->filename).getSize();
						totalSize += xSize;
						sFile.append(" " + WS::getSize(xSize), 9999);
					}
					else
					{
						sFile.append(" ! MISSING FILE !", 9999);
					}
				}
				//
				AddCompoLabelSM(sFile);
			}
		}
		//
		AddCompoLabelSM("Total Size " + WS::getSize(totalSize));

	}
	else if (editObject.type == WusikEditObject::kSoundFile)
	{
		WSPX_Collection_Sound_File* soundFile = (WSPX_Collection_Sound_File*)editObject.object;
		editOptionsComponent = new Component;
		processor.collection->lastSelectedPreset = nullptr;
		//
		for (int ff = 0; ff < soundFile->files.size(); ff++)
		{
			AddCompoLabel("Sound File #" + String(ff + 1));
			//
			String xFile;
			//
			if (soundFile->files[ff]->filename.isEmpty()) xFile = "No File";
			else if (File(soundFile->files[ff]->filename).existsAsFile()) xFile = "\"" + File(soundFile->files[ff]->filename).getFileName() + "\"";
			else xFile = File(soundFile->files[ff]->filename).getFileName() + " ! MISSING FILE !";
			//
			AddCompoLabelSM(xFile);
			AddCompoLabelSM("Channels " + String(soundFile->files[ff]->channels) + " - Samples " + String(soundFile->totalSamples) + " - SampleRate " + String(soundFile->sampleRate));
			//
			xFile = "Time " + String(double(soundFile->totalSamples) / double(soundFile->sampleRate), 2) + " seconds - Size " + WS::getSize(File(soundFile->files[ff]->filename).getSize());
			AddCompoLabelSM(xFile);
			//
			for (int cc = 0; cc < soundFile->files[ff]->channelsInfo.size(); cc++)
			{
				AddCompo(kString, "Channel Name", &soundFile->files[ff]->channelsInfo[cc]->name);
				AddCompo4(kSliderBipolar, "Pan", &soundFile->files[ff]->channelsInfo[cc]->pan, "", -1, 1);
				AddCompo4(kSliderBipolar, "Volume", &soundFile->files[ff]->channelsInfo[cc]->volume, "", -1, 1);
			}
		}
		//
		AddCompoLabelSM("Global Settings");
		AddCompo(kSlider, "Volume", &soundFile->volume);
		AddCompo4(kSliderBipolar, "Pan", &soundFile->pan, "", -1, 1);
		AddCompo4(kSliderBipolar, "Fine Tune", &soundFile->fineTune, "", -1, 1);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &soundFile->coarseTune, "", -48, 48);
		AddCompo(kOnOffButton, "Reverse", &soundFile->reverse);
		AddCompo(kOnOffButton, "Release", &soundFile->release);
		//
		AddCompoLabel("Loop");
		AddCompo6(kPopupList, "Type", &soundFile->loopType, "", soundFile->loopTypes);
		AddCompo4(kStringInt64, "Start", &soundFile->loopStart, "", 0, soundFile->totalSamples - 1);
		AddCompo4(kStringInt64, "End", &soundFile->loopEnd, "", 0, soundFile->totalSamples - 1);
		//
		AddCompoLabel("Zones");
		AddCompo4(kSliderInteger, "Key Zone Low", &soundFile->keyZoneLow, "", 0, 127); AddMIDIKey(selectedLow);
		AddCompo4(kSliderInteger, "Key Zone High", &soundFile->keyZoneHigh, "", 0, 127); AddMIDIKey(selectedHigh);
		AddCompo4(kSliderInteger, "Vel Zone Low", &soundFile->velZoneLow, "", 0, 127);
		AddCompo4(kSliderInteger, "Vel Zone High", &soundFile->velZoneHigh, "", 0, 127);
		AddCompo4(kSliderInteger, "Key Root", &soundFile->keyRoot, "", 0, 127); AddMIDIKey(rootKey);
		//
		midiKeyboard.selectedHigh = soundFile->keyZoneHigh;
		midiKeyboard.selectedLow = soundFile->keyZoneLow;
		midiKeyboard.rootKey = soundFile->keyRoot;
		midiKeyboard.repaint();
	}
	else if (editObject.type == WusikEditObject::kPreset)
	{
		WSPX_Collection_Preset* preset = (WSPX_Collection_Preset*)editObject.object;
		editOptionsComponent = new Component;
		processor.collection->lastSelectedPreset = preset;
		//
		AddCompoLabel("Preset Details");
		AddCompo(kString, "Name", &preset->name);
		AddCompo(kString, "Author", &preset->author);
		AddCompo(kString, "Description", &preset->description);
		AddCompo(kString, "Tags", &preset->tags);
		AddCompo(kImage, "Preset Image Icon", &preset->imagePresetIcon);
		AddCompo(kSlider, "Volume", &preset->volume);
		AddCompo4(kSliderBipolar, "Pan", &preset->pan, "", -1, 1);
		AddCompo4(kSliderBipolar, "Fine Tune", &preset->fineTune, "", -1, 1);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &preset->coarseTune, "", -48, 48);
		AddCompo(kButtonCallback, "Edit Scripting", &preset->scripting);
	}
	else if (editObject.type == WusikEditObject::kPresetLayer)
	{
		WSPX_Collection_Preset_Layer* layer = (WSPX_Collection_Preset_Layer*)editObject.object;
		editOptionsComponent = new Component;
		//
		for (int xs = 0; xs < processor.collection->presets.size(); xs++)
		{
			if (processor.collection->presets[xs]->layers.contains(layer))
			{
				processor.collection->lastSelectedPreset = processor.collection->presets[xs];
				break;
			}
		}
		//
		AddCompoLabel("Layer Details");
		AddCompo(kString, "Name", &layer->name);
		AddCompo(kSlider, "Volume", &layer->volume);
		AddCompo4(kSliderBipolar, "Pan", &layer->pan, "", -1, 1);
		AddCompo4(kSliderBipolar, "Fine Tune", &layer->fineTune, "", -1, 1);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &layer->coarseTune, "", -48, 48);
		AddCompo4(kSliderInteger, "Voices", &layer->voices, "", 1, 128);
		AddCompo(kSlider, "Glide", &layer->glide);
		AddCompo(kOnOffButton, "Auto Glide", &layer->autoGlide);
		AddCompo(kOnOffButton, "Reverse", &layer->reverse);
		AddCompo(kButtonCallback, "Edit Scripting", &layer->scripting);
		//
		AddCompoLabel("Zones");
		AddCompo4(kSliderInteger, "Key Zone Low", &layer->keyZoneLow, "", 0, 127); AddMIDIKey(selectedLow);
		AddCompo4(kSliderInteger, "Key Zone High", &layer->keyZoneHigh, "", 0, 127); AddMIDIKey(selectedHigh);
		AddCompo4(kSliderInteger, "Vel Zone Low", &layer->velZoneLow, "", 0, 127);
		AddCompo4(kSliderInteger, "Vel Zone High", &layer->velZoneHigh, "", 0, 127);
		//
		AddCompoLabel("AMP Envelope");
		AddCompo(kSlider, "Attack", &layer->ampEnvelope.attack);
		AddCompo(kSlider, "Decay", &layer->ampEnvelope.decay);
		AddCompo(kSlider, "Sustain", &layer->ampEnvelope.sustain);
		AddCompo(kSlider, "Release", &layer->ampEnvelope.release);
		AddCompo4(kSliderInteger, "Max Seconds", &layer->ampEnvelope.maxSeconds, "", 1, 99);
		AddCompo(kSlider, "Velocity %", &layer->ampEnvelope.velocity);
		AddCompo4(kSliderBipolar, "Key Track", &layer->ampEnvelope.keyTrack, "", -1, 1);
		AddCompo4(kSliderBipolar, "Velocity Track", &layer->ampEnvelope.velTrack, "", -1, 1);
		AddCompo6(kPopupList, "Type", &layer->ampEnvelope.type, "", layer->ampEnvelope.types);
		//
		AddCompoLabel("Filter");
		AddCompo6(kPopupList, "Type", &layer->filter.type, "", layer->filter.types);
		AddCompo(kSlider, "Frequency", &layer->filter.frequency);
		AddCompo(kSlider, "Rezonance", &layer->filter.rezonance);
		AddCompo(kSlider, "Smooth", &layer->filter.smooth);
		AddCompo(kSlider, "Feedback", &layer->filter.feedback);
		AddCompo(kSlider, "Feedback Delay", &layer->filter.delay);
		AddCompo(kSlider, "Sample and Hold", &layer->filter.sampleAndHold);
		AddCompo4(kSliderBipolar, "To Envelope", &layer->filter.toEnvelope, "", -1, 1);
		//
		AddCompoLabel("Filter Envelope");
		AddCompo(kSlider, "Attack", &layer->filter.envelope.attack);
		AddCompo(kSlider, "Decay", &layer->filter.envelope.decay);
		AddCompo(kSlider, "Sustain", &layer->filter.envelope.sustain);
		AddCompo(kSlider, "Release", &layer->filter.envelope.release);
		AddCompo4(kSliderInteger, "Max Seconds", &layer->filter.envelope.maxSeconds, "", 1, 99);
		AddCompo(kSlider, "Velocity %", &layer->filter.envelope.velocity);
		AddCompo4(kSliderBipolar, "Key Track", &layer->filter.envelope.keyTrack, "", -1, 1);
		AddCompo4(kSliderBipolar, "Velocity Track", &layer->filter.envelope.velTrack, "", -1, 1);
		AddCompo6(kPopupList, "Type", &layer->filter.envelope.type, "", layer->filter.envelope.types);
		//
		for (int ll = 0; ll < 2; ll++)
		{
			AddCompoLabel("LFO " + String(ll + 1));
			AddCompo6(kPopupList, "Waveform", &layer->lfos[ll].waveform, "", layer->lfos[ll].waveforms);
			AddTime(&layer->lfos[ll].time);
			AddCompo(kSlider, "Phase", &layer->lfos[ll].phase);
			AddCompo(kSlider, "Smooth", &layer->lfos[ll].smooth);
			AddCompo4(kSliderBipolar, "To Filter Frequency", &layer->lfos[ll].toFilterFreq, "", -1, 1);
			AddCompo4(kSliderBipolar, "To Volume", &layer->lfos[ll].toVolume, "", -1, 1);
			AddCompo4(kSliderBipolar, "To Pan", &layer->lfos[ll].toPan, "", -1, 1);
			AddCompo4(kSliderBipolar, "To Effect Send 1", &layer->lfos[ll].toEffect1, "", -1, 1);
			AddCompo4(kSliderBipolar, "To Effect Send 2", &layer->lfos[ll].toEffect2, "", -1, 1);
			AddCompo4(kSliderBipolar, "To Effect Send 3", &layer->lfos[ll].toEffect3, "", -1, 1);
			AddCompo4(kSliderBipolar, "To Effect Send 4", &layer->lfos[ll].toEffect4, "", -1, 1);
			AddCompo(kOnOffButton, "Note On Reset", &layer->lfos[ll].noteOnReset);
			AddCompo(kOnOffButton, "Inverted", &layer->lfos[ll].inverted);
		}
		//
		AddCompoLabel("ARP");
		AddTime(&layer->ARP.time);
		AddCompo6(kPopupList, "Mode", &layer->ARP.mode, "", layer->ARP.modes);
		AddCompo4(kSliderInteger, "Octaves", &layer->ARP.octaves, "", 1, 24);
		AddCompo(kOnOffButton, "Glissando", &layer->ARP.glissando);
		//
		AddCompoLabel("Sequencer");
		AddTime(&layer->sequencer.time);
		AddCompo(kSlider, "Smooth", &layer->sequencer.smoothOutput);
		AddCompo6(kPopupList, "Mode", &layer->sequencer.mode, "", layer->sequencer.modes);
		AddCompo2(kButtonCallback, "Edit Sequencer", nullptr, "", false, new WusikEditOptionCallback_OpenEditSequencer(layer));
		//
		midiKeyboard.selectedHigh = layer->keyZoneHigh;
		midiKeyboard.selectedLow = layer->keyZoneLow;
		midiKeyboard.repaint();
	}
	else if (editObject.type == WusikEditObject::kSequencer)
	{
		WSPX_Sequencer* sequencer = (WSPX_Sequencer*)editObject.object;
		editOptionsComponent = new Component;
		processor.collection->lastSelectedPreset = nullptr;
		//
		AddCompoLabel("Sequencer");
		AddTime(&sequencer->time);
		AddCompo(kSlider, "Smooth", &sequencer->smoothOutput);
		AddCompo6(kPopupList, "Mode", &sequencer->mode, "", sequencer->modes);
		//
		if (sequencer->steps.size() > 0)
		{
			AddCompo4(kSliderInteger, "Loop Start", &sequencer->loopStart, "", 1, sequencer->steps.size());
			AddCompoLabelSM("Total Steps: " + String(sequencer->steps.size() + 1));
		}
		//
		if (sequencer->steps.size() == 0)
		{
			AddSequencerButton("Add Step", kAddToStart);
			AddSequencerButton("Add 16 Steps", kAppend16Steps);
		}
		else
		{
			AddSequencerButton("Append 16 Steps", kAppend16Steps);
			AddSequencerButton("Add Step To Start", kAddToStart);
			AddSequencerButton("Add Step To End", kAddToEnd);
			AddSequencerButton("Remove All Steps", kRemoveAll);
		}
		//
		if (sequencer->steps.size() > 0)
		{
			for (int tt = 0; tt < sequencer->steps.size(); tt++)
			{
				AddCompoLabel("Step #" + String(tt + 1));
				AddCompo4(kSliderInteger, "Time", &sequencer->steps[tt]->time, "", 1, 128);
				AddCompo(kSlider, "Volume", &sequencer->steps[tt]->volume);
				AddCompo4(kSliderBipolar, "Pan", &sequencer->steps[tt]->pan, "", -1, 1);
				AddCompo4(kSliderBipolar, "Fine Tune", &sequencer->steps[tt]->fine, "", -1, 1);
				AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &sequencer->steps[tt]->tune, "", -48, 48);
				AddCompo4(kSliderBipolar, "To Filter Frequency", &sequencer->steps[tt]->filterFreq, "", -1, 1);
				//
				AddCompo2(kButtonCallback, "Remove Step", nullptr, "", false, new WusikEditOptionCallback_Sequencer_Step(sequencer, sequencer->steps[tt], WusikEditOptionCallback_Sequencer_Step::kRemove));
			}
		}
	}
	else if (editObject.type == WusikEditObject::kSoundZones)
	{
		processor.collection->lastSelectedPreset = nullptr;
		WSPX_Collection_Sound* sound = (WSPX_Collection_Sound*)editObject.object;
		//
		for (int ss = 0; ss < sound->soundFiles.size(); ss++)
		{
			addAndMakeVisible(keyVelZones.add(new WSPXKeyVelZone(this, sound->soundFiles[ss])));
		}
	}
	//
	if (editOptionsComponent != nullptr)
	{
		addAndMakeVisible(editOptionsViewport = new Viewport);
		editOptionsViewport->setViewedComponent(editOptionsComponent);
		editOptionsViewport->setScrollBarsShown(true, false);
		editOptionsViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colours::grey);
		editOptionsViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::trackColourId, Colours::grey);
	}
	//
	resized();
	repaint();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikEditOption::WusikEditOption(WusikSpxAudioProcessor* _processor, Component* _editor, int _type, String _label,
	void* _object, String _extraLabel, bool _showEditInstead, WusikEditOptionCallback* _callback, int _min, int _max, String _popupList)
	: label(_label), object(_object), type(_type), showEditInstead(_showEditInstead), extraLabel(_extraLabel),
	callback(_callback), processor(_processor), editor(_editor), min(_min), max(_max)
{
	if (type == kSlider || type == kSliderBipolar)
	{
		WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor->getActiveEditor();
		slider = new WSlider(editor->sliderBackground, editor->sliderFilled, editor->sliderThumb, object, type == kSliderBipolar, min, max);
		addAndMakeVisible(slider);
	}
	else if (type == kSliderInteger || type == kSliderIntegerBipolar)
	{
		WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor->getActiveEditor();
		slider = new WSlider(editor->sliderBackground, editor->sliderFilled, editor->sliderThumb, object, (type == kSliderIntegerBipolar), min, max, WSlider::kInt);
		addAndMakeVisible(slider);
	}
	else if (type == kPopupList)
	{
		popupList = StringArray::fromLines(_popupList);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
bool WusikEditOptionCallback_UpdateCollectionName::process(WusikSpxAudioProcessor* processor)
{
	((WusikSpxAudioProcessorEditor*)processor->getActiveEditor())->
		collectionNameLabel->setText(processor->collection->name, NotificationType::dontSendNotification);
	return false;
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
bool WusikEditOptionCallback_OpenEditSequencer::process(WusikSpxAudioProcessor* processor)
{
	((WusikSpxAudioProcessorEditor*)processor->getActiveEditor())->
		createAction(WusikSpxAudioProcessorEditor::kTimerAction_Open_Edit_Sequencer, 0, 0, 0, 0, (void*) &layer->sequencer);
	return true;
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
bool WusikEditOptionCallback_Sequencer_Step::process(WusikSpxAudioProcessor* processor)
{
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*) processor->getActiveEditor();
	//
	if (type == kAddToStart) sequencer->steps.insert(0, new WSPX_Sequencer_Step);
	else if (type == kAddToEnd) sequencer->steps.add(new WSPX_Sequencer_Step);
	else if (type == kRemoveAll)
	{
		if (WConfirmBox("Remove All Steps", "Are you sure?")) 
		{ 
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sequencer_Step, -1, 0, 0, 0, sequencer);
			return true; // we exit quickly as this is about to go down //
		}
	}
	else if (type == kAppend16Steps)
	{
		if (WConfirmBox("Append 16 Steps (to the end of the sequence)", "Are you sure?"))
		{
			for (int ss = 0; ss < 16; ss++) { sequencer->steps.add(new WSPX_Sequencer_Step); }
		}
	}
	else if (type == kRemove)
	{
		editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sequencer_Step, sequencer->steps.indexOf(step), 0, 0, 0, sequencer);
		return true; // we exit quickly as this is about to go down //
	}
	//
	editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
	return true; // we exit quickly as this is about to go down //
}