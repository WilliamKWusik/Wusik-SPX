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
	editOptionsViewport = nullptr;
	editOptionsComponent = nullptr;
	soundZones.clear();
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::updateInterface()
{
	/*for (int ss = 0; ss < processor.collection->soundGroups[processor.collection->editingSound]->sounds.size(); ss++)
	{
		addAndMakeVisible(soundZones.add(new WSPXSoundZone(processor.collection->soundGroups[processor.collection->editingSound]->sounds[ss])));
	}*/

	if (editObject.type == WusikEditObject::kCollection)
	{
		editOptionsComponent = new Component;
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kLabel, "Collection Details")));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Name", &processor.collection->name, "", false, new WusikEditOptionCallback_UpdateCollectionName)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Author", &processor.collection->author)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Version", &processor.collection->version)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Company", &processor.collection->company)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Tags", &processor.collection->tags, "Separated by ;")));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Description", &processor.collection->description, "", true)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kImage, "Image Icon", &processor.collection->imageIcon)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kImage, "Image About", &processor.collection->imageAbout)));
		editOptionsComponent->addAndMakeVisible(editOptions.add(new WusikEditOption(&processor, this, WusikEditOption::kString, "Protection Key", &processor.collection->protectionKey, "", true)));
		//
		addAndMakeVisible(editOptionsViewport = new Viewport);
		editOptionsViewport->setViewedComponent(editOptionsComponent);
		editOptionsViewport->setScrollBarsShown(true, false);

	}
	else if (editObject.type == WusikEditObject::kPreset)
	{
		WSPX_Collection_Preset* preset = (WSPX_Collection_Preset*) editObject.object;
		//
		for (int ll = 0; ll < preset->layers.size(); ll++)
		{
			
		}
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikEditOptionCallback_UpdateCollectionName::process(WusikSpxAudioProcessor* processor)
{
	((WusikSpxAudioProcessorEditor*)processor->getActiveEditor())->
		collectionNameLabel->setText(processor->collection->name, NotificationType::dontSendNotification);
}