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
	if (!keepTreeViews)
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
	/*for (int ss = 0; ss < processor.collection->soundGroups[processor.collection->editingSound]->sounds.size(); ss++)
	{
		addAndMakeVisible(soundZones.add(new WSPXSoundZone(processor.collection->soundGroups[processor.collection->editingSound]->sounds[ss])));
	}*/

	#define AddCompoLabel(label) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kLabel, label)))
	#define AddCompoCallback(type, name, variable, label, callback) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, callback)))
	#define AddCompo(type, name, variable) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable)))
	#define AddCompo2(type, name, variable, label, showEditInsteead, callback) editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::type, name, variable, label, showEditInsteead, callback)))
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
		//
		addAndMakeVisible(editOptionsViewport = new Viewport);
		editOptionsViewport->setViewedComponent(editOptionsComponent);
		editOptionsViewport->setScrollBarsShown(true, false);
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
		//
		addAndMakeVisible(editOptionsViewport = new Viewport);
		editOptionsViewport->setViewedComponent(editOptionsComponent);
		editOptionsViewport->setScrollBarsShown(true, false);
	}
	else if (editObject.type == WusikEditObject::kPresetLayer)
	{
		WSPX_Collection_Preset_Layer* layer = (WSPX_Collection_Preset_Layer*)editObject.object;
		//
		editOptionsComponent = new Component;
		AddCompoLabel("Layer #" + String(editObject.index + 1) + " Details");
		//
		addAndMakeVisible(editOptionsViewport = new Viewport);
		editOptionsViewport->setViewedComponent(editOptionsComponent);
		editOptionsViewport->setScrollBarsShown(true, false);
	}
	//
	resized();
	repaint();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikEditOptionCallback_UpdateCollectionName::process(WusikSpxAudioProcessor* processor)
{
	((WusikSpxAudioProcessorEditor*)processor->getActiveEditor())->
		collectionNameLabel->setText(processor->collection->name, NotificationType::dontSendNotification);
}