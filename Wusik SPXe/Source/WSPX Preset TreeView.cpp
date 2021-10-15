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
WSPXPresetTreeItem::WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, uint8_t _level, int16 _preset, double _ui_ratio, int16 _sound, int16 _subSound)
	: processor(_processor), preset(_preset), level(_level), ui_ratio(_ui_ratio), sound(_sound), subSound(_subSound)
{
	if (level == kLevel_AddPreset)
	{
		for (int pp = 0; pp < processor.collection->presets.size(); pp++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, kLevel_PresetName, pp, ui_ratio));
		}
	}
	else if (level == kLevel_PresetName)
	{
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_PresetName_Options, preset, ui_ratio, kPreset_Options));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_PresetName_Options, preset, ui_ratio, kPreset_Remove));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_PresetName_Options, preset, ui_ratio, kPreset_Duplicate));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_PresetName_Options, preset, ui_ratio, kPreset_Add_Sound));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_PresetName_Options, preset, ui_ratio, kPreset_Sounds));
	}
	else if (level == kLevel_PresetName_Options && sound == kPreset_Sounds)
	{
		for (int ss = 0; ss < processor.collection->presets[preset]->sounds.size(); ss++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, kLevel_Sounds, preset, ui_ratio, ss));
		}
	}
	else if (level == kLevel_Sounds)
	{
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_Sounds_Options, preset, ui_ratio, kSound_Options));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_Sounds_Options, preset, ui_ratio, kSound_Remove));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_Sounds_Options, preset, ui_ratio, kSound_Add_SubSound));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_Sounds_Options, preset, ui_ratio, sound));
	}
	else if (level == kLevel_Sounds_Options && sound >= 0)
	{
		for (int ss = 0; ss < processor.collection->presets[preset]->sounds[sound]->soundsID.size(); ss++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, kLevel_SubSounds, preset, ui_ratio, sound, ss));
		}
	}
	else if (level == kLevel_SubSounds)
	{
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_SubSounds_Options, preset, ui_ratio, kSubSound_Options));
		addSubItem(new WSPXPresetTreeItem(processor, kLevel_SubSounds_Options, preset, ui_ratio, kSubSound_Remove));
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXPresetTreeItem::paintItem(Graphics& g, int width, int height)
{
	if (isSelected()) g.fillAll(Colours::red.withAlpha(0.32f));  else g.fillAll(Colours::black.withAlpha(0.16f));
	//
	g.setColour(Colours::white.withAlpha(0.82f));
	g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(height) * 0.72f));
	//
	if (level == kLevel_AddPreset)
	{
		g.setColour(Colours::lightblue.withAlpha(0.82f));
		g.drawText("Add Preset", 0, 0, width, height, Justification::left);
	}
	else if (level == kLevel_PresetName) g.drawText(processor.collection->presets[preset]->name, 0, 0, width, height, Justification::left);
	else if (level == kLevel_PresetName_Options)
	{
		if (sound == kPreset_Options)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Options", 0, 0, width, height, Justification::left);
		}
		else if (sound == kPreset_Remove)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Remove", 0, 0, width, height, Justification::left);
		}
		else if (sound == kPreset_Duplicate)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Duplicate", 0, 0, width, height, Justification::left);
		}
		else if (sound == kPreset_Add_Sound)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Add Sound", 0, 0, width, height, Justification::left);
		}
		else if (sound == kPreset_Sounds)
		{
			g.drawText("Sounds", 0, 0, width, height, Justification::left);
		}
	}
	else if (level == kLevel_Sounds)
	{
		g.drawText("Sound " + String(sound + 1), 0, 0, width, height, Justification::left);
	}
	else if (level == kLevel_Sounds_Options)
	{
		if (sound == kSound_Options)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Options", 0, 0, width, height, Justification::left);
		}
		else if (sound == kSound_Add_SubSound)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Add Sub", 0, 0, width, height, Justification::left);
		}
		else if (sound == kSound_Remove)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Remove", 0, 0, width, height, Justification::left);
		}
		else if (sound >= 0)
		{
			g.drawText("SubSounds", 0, 0, width, height, Justification::left);
		}
	}
	else if (level == kLevel_SubSounds)
	{
		g.drawText(processor.collection->sounds[processor.collection->presets[preset]->sounds[sound]->soundsID[subSound]]->name,
			0, 0, width, height, Justification::left);
	}
	else if (level == kLevel_SubSounds_Options)
	{
		if (sound == kSubSound_Options)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Options", 0, 0, width, height, Justification::left);
		}
		else if (sound == kSubSound_Remove)
		{
			g.setColour(Colours::lightblue.withAlpha(0.82f));
			g.drawText("Remove", 0, 0, width, height, Justification::left);
		}
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXPresetTreeItem::itemClicked(const MouseEvent& e)
{
	if (level > kLevel_AddPreset) setOpen(!isOpen());
}