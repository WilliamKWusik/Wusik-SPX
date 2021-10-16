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
WSPXSoundTreeItem::WSPXSoundTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, uint8_t _level, String _name, uint8_t _specialItem, int16 _soundGroup, int16 _sound)
	: processor(_processor), level(_level), ui_ratio(_ui_ratio), soundGroup(_soundGroup), specialItem(_specialItem), sound(_sound), name(_name)
{
	if (level == kLevel_AddSound)
	{
		for (int gg = 0; gg < processor.collection->soundGroups.size(); gg++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "", kRegular_Item, gg));
		}
	}
	else if (level == kLevel_Sound_Groups)
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "Remove", kSound_Group_Remove, soundGroup));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "Load File", kSound_Group_Add_Sound, soundGroup));
		//
		for (int ss = 0; ss < processor.collection->soundGroups[soundGroup]->sounds.size(); ss++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "", kRegular_Item, soundGroup, ss));
		}
	}
	else if (level == kLevel_Sounds && name.isEmpty())
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds_Options, "Remove", kSound_Remove, soundGroup, sound));
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXSoundTreeItem::paintItem(Graphics& g, int width, int height)
{
	if (isSelected()) g.fillAll(Colours::red.withAlpha(0.32f));  else g.fillAll(Colours::black.withAlpha(0.16f));
	//
	g.setColour(Colours::white.withAlpha(0.82f));
	g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(height) * 0.72f));
	//
	if (name.isNotEmpty())
	{
		g.setColour(Colours::lightblue.withAlpha(0.82f));
		g.drawText(name, 0, 0, width, height, Justification::left);
	}
	else if (level == kLevel_Sound_Groups) g.drawText(processor.collection->soundGroups[soundGroup]->name, 0, 0, width, height, Justification::left);
	else if (level == kLevel_Sounds)
	{
		if (processor.collection->soundGroups[soundGroup]->sounds[sound]->soundFile.existsAsFile())
			g.drawFittedText(processor.collection->soundGroups[soundGroup]->sounds[sound]->soundFile.getFileNameWithoutExtension(), 0, 0, width, height, Justification::left, 1);
		else 
			g.drawText("No File", 0, 0, width, height, Justification::left);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXSoundTreeItem::itemClicked(const MouseEvent& e)
{
	if (level > kLevel_AddSound) setOpen(!isOpen());
}