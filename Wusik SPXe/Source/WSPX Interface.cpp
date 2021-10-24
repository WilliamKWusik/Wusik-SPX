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
	soundZones.clear();
	editOptions.clear();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::updateInterface()
{
	/*for (int ss = 0; ss < processor.collection->sounds[processor.collection->editingSound]->sounds.size(); ss++)
	{
		addAndMakeVisible(soundZones.add(new WSPXSoundZone(processor.collection->sounds[processor.collection->editingSound]->sounds[ss])));
	}*/

	editOptionsComponent = nullptr;

	#define AddCompoLabel(label) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kLabel, label)))
	#define AddCompoCallback(type, name, variable, label, callback) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, callback)))
	#define AddCompo(type, name, variable) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable)))
	#define AddCompo2(type, name, variable, label, showEditInsteead, callback) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, showEditInsteead, callback)))
	#define AddCompo4(type, name, variable, label, min, max) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, false, nullptr, min, max)))
	#define AddCompo6(type, name, variable, label, PopupList) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, false, nullptr, 0, 1, PopupList)))
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
		AddCompo2(kString, "Protection Key", &processor.collection->protectionKey, "", true, nullptr);
	}
	else if (editObject.type == WusikEditObject::kSound)
	{
		WSPX_Collection_Sound* sound = (WSPX_Collection_Sound*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Sound Details");
		AddCompo(kString, "Name", &sound->name);
		AddCompo(kString, "Tags", &sound->tags);
		AddCompo4(kSliderInteger, "Choke Group", &sound->chokeGroup, "", 0, 128);

	}
	else if (editObject.type == WusikEditObject::kSoundFile)
	{
		WSPX_Collection_Sound_File* soundFile = (WSPX_Collection_Sound_File*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Sound File Details");
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
		AddCompo(kSlider, "Volume", &preset->volume);
		AddCompo(kSliderBipolar, "Pan", &preset->pan);
		AddCompo(kSliderBipolar, "Fine Tune", &preset->fineTune);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &preset->coarseTune, "", -48, 48);
	}
	else if (editObject.type == WusikEditObject::kPresetLayer)
	{
		WSPX_Collection_Preset_Layer* layer = (WSPX_Collection_Preset_Layer*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Layer Details");
		AddCompo(kSlider, "Volume", &layer->volume);
		AddCompo(kSliderBipolar, "Pan", &layer->pan);
		AddCompo(kSliderBipolar, "Fine Tune", &layer->fineTune);
		AddCompo4(kSliderIntegerBipolar, "Coarse Tune", &layer->coarseTune, "", -48, 48);
		AddCompo4(kSliderInteger, "Voices", &layer->voices, "", 1, 128);
		AddCompo(kSlider, "Glide", &layer->glide);
		AddCompo(kOnOffButton, "Auto Glide", &layer->autoGlide);
		AddCompo(kOnOffButton, "Reverse", &layer->reverse);
		//
		AddCompoLabel("Zones");
		AddCompo4(kSliderInteger, "Key Zone Low", &layer->keyZoneLow, "", 0, 127);
		AddCompo4(kSliderInteger, "Key Zone High", &layer->keyZoneHigh, "", 0, 127);
		AddCompo4(kSliderInteger, "Vel Zone Low", &layer->velZoneLow, "", 0, 127);
		AddCompo4(kSliderInteger, "Vel Zone High", &layer->velZoneHigh, "", 0, 127);
		//
		AddCompoLabel("AMP Envelope");
		AddCompo(kSlider, "Attack", &layer->ampEnvelope.attack);
		AddCompo(kSlider, "Decay", &layer->ampEnvelope.decay);
		AddCompo(kSlider, "Sustain", &layer->ampEnvelope.sustain);
		AddCompo(kSlider, "Release", &layer->ampEnvelope.release);
		AddCompo(kSlider, "Max Seconds", &layer->ampEnvelope.maxSeconds);
		AddCompo(kSlider, "Velocity %", &layer->ampEnvelope.velocity);
		AddCompo(kSliderBipolar, "Key Track", &layer->ampEnvelope.keyTrack);
		AddCompo(kSliderBipolar, "Velocity Track", &layer->ampEnvelope.velTrack);
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
		AddCompo(kSliderBipolar, "To Envelope", &layer->filter.toEnvelope);
		//
		AddCompoLabel("Filter Envelope");
		AddCompo(kSlider, "Attack", &layer->filter.envelope.attack);
		AddCompo(kSlider, "Decay", &layer->filter.envelope.decay);
		AddCompo(kSlider, "Sustain", &layer->filter.envelope.sustain);
		AddCompo(kSlider, "Release", &layer->filter.envelope.release);
		AddCompo(kSlider, "Max Seconds", &layer->filter.envelope.maxSeconds);
		AddCompo(kSlider, "Velocity %", &layer->filter.envelope.velocity);
		AddCompo(kSliderBipolar, "Key Track", &layer->filter.envelope.keyTrack);
		AddCompo(kSliderBipolar, "Velocity Track", &layer->filter.envelope.velTrack);
		AddCompo6(kPopupList, "Type", &layer->filter.envelope.type, "", layer->filter.envelope.types);
		//
		for (int ll = 0; ll < 2; ll++)
		{
			AddCompoLabel("LFO " + String(ll + 1));
			AddCompo6(kPopupList, "Waveform", &layer->lfos[ll].waveform, "", layer->lfos[ll].waveforms);
			AddCompo(kSlider, "Speed 1", &layer->lfos[ll].speed1);
			AddCompo(kSlider, "Speed 2", &layer->lfos[ll].speed2);
			AddCompo(kOnOffButton, "Sync to BPM", &layer->lfos[ll].sync);
			AddCompo(kSlider, "Phase", &layer->lfos[ll].phase);
			AddCompo(kSlider, "Smooth", &layer->lfos[ll].smooth);
			AddCompo(kSlider, "To Filter Frequency", &layer->lfos[ll].toFilterFreq);
			AddCompo(kSlider, "To Volume", &layer->lfos[ll].toVolume);
			AddCompo(kSlider, "To Pan", &layer->lfos[ll].toPan);
			AddCompo(kSlider, "To Effect Send 1", &layer->lfos[ll].toEffect1);
			AddCompo(kSlider, "To Effect Send 2", &layer->lfos[ll].toEffect2);
			AddCompo(kSlider, "To Effect Send 3", &layer->lfos[ll].toEffect3);
			AddCompo(kSlider, "To Effect Send 4", &layer->lfos[ll].toEffect4);
			AddCompo(kOnOffButton, "Note On Reset", &layer->lfos[ll].noteOnReset);
			AddCompo(kOnOffButton, "Inverted", &layer->lfos[ll].inverted);
		}
		//
		AddCompoLabel("Sequencer");
		AddCompo(kSlider, "Speed 1", &layer->sequencer.speed1);
		AddCompo(kSlider, "Speed 1", &layer->sequencer.speed2);
		AddCompo(kOnOffButton, "Sync to BPM", &layer->sequencer.sync);
		AddCompo(kSlider, "Smooth", &layer->sequencer.smoothOutput);
		AddCompo6(kPopupList, "Mode", &layer->sequencer.mode, "", layer->sequencer.modes);
	}
	//
	if (editOptionsComponent != nullptr)
	{
		addAndMakeVisible(editOptionsViewport = new Viewport);
		editOptionsViewport->setViewedComponent(editOptionsComponent);
		editOptionsViewport->setScrollBarsShown(true, false);
		editOptionsViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colours::darkblue);
		editOptionsViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::trackColourId, Colours::darkblue);
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
		slider = new WSlider(editor->sliderBackground, editor->sliderFilled, editor->sliderThumb, ((float*)object)[0], type == kSliderBipolar);
		addAndMakeVisible(slider);
	}
	else if (type == kSliderInteger || type == kSliderIntegerBipolar)
	{
		WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor->getActiveEditor();
		slider = new WSlider(editor->sliderBackground, editor->sliderFilled, editor->sliderThumb, ((float*)object)[0], (type == kSliderIntegerBipolar), true, min, max);
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