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

	//
	if (editObject.type == WusikEditObject::kCollection)
	{
		editOptionsComponent = new Component;
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
			String sFile = "File #" + String(ss + 1) + " ";
			//
			if (sound->soundFiles[ss]->soundFile.isEmpty())
			{
				sFile.append("No File", 9999);
			}
			else
			{
				sFile.append("\"" + File(sound->soundFiles[ss]->soundFile).getFileName() + "\"", 9999);
				//
				if (File(sound->soundFiles[ss]->soundFile).existsAsFile())
				{
					int64 xSize = File(sound->soundFiles[ss]->soundFile).getSize();
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
		//
		AddCompoLabelSM("Total Size " + WS::getSize(totalSize));

	}
	else if (editObject.type == WusikEditObject::kSoundFile)
	{
		WSPX_Collection_Sound_File* soundFile = (WSPX_Collection_Sound_File*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Sound File Details");
		//
		String xFile;
		//
		if (soundFile->soundFile.isEmpty()) xFile = "No File";
		else if (File(soundFile->soundFile).existsAsFile()) xFile = "\"" + soundFile->soundFile + "\"";
		else xFile = File(soundFile->soundFile).getFileName() + " ! MISSING FILE !";
		//
		AddCompoLabelSM(xFile);
		AddCompoLabelSM("Channels " + String(soundFile->channels) + " - Samples " + String(soundFile->totalSamples) + " - SampleRate " + String(soundFile->sampleRate));
		//
		xFile = "Time " + String(double(soundFile->totalSamples) / double(soundFile->sampleRate), 2) + " seconds - Size " + WS::getSize(File(soundFile->soundFile).getSize());
		AddCompoLabelSM(xFile);
		//
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
		if (soundFile->channelInformation.size() > 0) AddCompoLabel("Channel Information");
		for (int cc = 0; cc < soundFile->channelInformation.size(); cc++)
		{
			AddCompoLabelSM("Channel #" + String(cc + 1));
			AddCompo(kString, "Name", &soundFile->channelInformation[cc]->name);
			AddCompo(kSlider, "Volume", &soundFile->channelInformation[cc]->volume);
			AddCompo4(kSliderBipolar, "Pan", &soundFile->channelInformation[cc]->pan, "", -1.0f, 1.0f);
		}
		//
		midiKeyboard.selectedHigh = soundFile->keyZoneHigh;
		midiKeyboard.selectedLow = soundFile->keyZoneLow;
		midiKeyboard.rootKey = soundFile->keyRoot;
		midiKeyboard.repaint();
	}
	else if (editObject.type == WusikEditObject::kPreset)
	{
		WSPX_Collection_Preset* preset = (WSPX_Collection_Preset*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Preset Details");
		AddCompo(kString, "Name", &preset->name);
		AddCompo(kString, "Author", &preset->author);
		AddCompo(kString, "Description", &preset->description);
		AddCompo(kString, "Tags", &preset->tags);
		AddCompo(kImage, "Preset Image Icon", &preset->imagePresetIcon);
		AddCompo(kButtonCallback, "Scripting", &preset->scripting);
		AddCompo(kSlider, "Volume", &preset->volume);
		AddCompo4(kSliderBipolar, "Pan", &preset->pan, "", -1, 1);
		AddCompo4(kSliderBipolar, "Fine Tune", &preset->fineTune, "", -1, 1);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &preset->coarseTune, "", -48, 48);
	}
	else if (editObject.type == WusikEditObject::kPresetLayer)
	{
		WSPX_Collection_Preset_Layer* layer = (WSPX_Collection_Preset_Layer*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Layer Details");
		AddCompo(kString, "Name", &layer->name);
		AddCompo(kButtonCallback, "Scripting", &layer->scripting);
		AddCompo(kSlider, "Volume", &layer->volume);
		AddCompo4(kSliderBipolar, "Pan", &layer->pan, "", -1, 1);
		AddCompo4(kSliderBipolar, "Fine Tune", &layer->fineTune, "", -1, 1);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &layer->coarseTune, "", -48, 48);
		AddCompo4(kSliderInteger, "Voices", &layer->voices, "", 1, 128);
		AddCompo(kSlider, "Glide", &layer->glide);
		AddCompo(kOnOffButton, "Auto Glide", &layer->autoGlide);
		AddCompo(kOnOffButton, "Reverse", &layer->reverse);
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
			AddCompo(kTime, "Time", &layer->lfos[ll].time);
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
		AddCompoLabel("Sequencer");
		AddCompo(kTime, "Time", &layer->sequencer.time);
		AddCompo(kSlider, "Smooth", &layer->sequencer.smoothOutput);
		AddCompo6(kPopupList, "Mode", &layer->sequencer.mode, "", layer->sequencer.modes);
		AddCompo(kButtonCallback, "Steps", &layer->sequencer);
		//
		midiKeyboard.selectedHigh = layer->keyZoneHigh;
		midiKeyboard.selectedLow = layer->keyZoneLow;
		midiKeyboard.repaint();
	}
	else if (editObject.type == WusikEditObject::kSoundZones)
	{
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
void WusikEditOptionCallback_UpdateCollectionName::process(WusikSpxAudioProcessor* processor)
{
	((WusikSpxAudioProcessorEditor*)processor->getActiveEditor())->
		collectionNameLabel->setText(processor->collection->name, NotificationType::dontSendNotification);
}