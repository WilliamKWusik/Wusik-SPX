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
WSPXPresetTreeItem::WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, uint8_t _level, String _name, int16 _preset, uint8_t _specialItem, int16 _layer, int16 _soundGroup, int16 _sound)
	: processor(_processor), preset(_preset), level(_level), ui_ratio(_ui_ratio), soundGroup(_soundGroup), layer(_layer), specialItem(_specialItem), sound(_sound), name(_name)
{
	if (level == kLevel_AddPreset)
	{
		for (int pp = 0; pp < processor.collection->presets.size(); pp++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Presets, "", pp));
		}
	}
	else if (level == kLevel_Presets)
	{
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Remove", preset, kPreset_Remove));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Duplicate", preset, kPreset_Duplicate));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Add Layer", preset, kPreset_Add_Layer));
		//
		for (int ll = 0; ll < processor.collection->presets[preset]->layers.size(); ll++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "", preset, kRegular_Item, ll));
		}
	}
	else if (level == kLevel_Preset_Layers && specialItem == kRegular_Item)
	{
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "Remove", preset, kPreset_Layer_Remove, layer));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "Add Group", preset, kPreset_Layer_Add_Sound_Group, layer));
		//
		for (int ss = 0; ss < processor.collection->presets[preset]->layers[soundGroup]->soundGroupIDs.size(); ss++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "", preset, kRegular_Item, layer, ss));
		}
	}
	else if (level == kLevel_Sound_Groups && specialItem == kRegular_Item)
	{
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups_Options, "Remove", preset, kSound_Group_Remove, layer, sound));
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
	if (name.isNotEmpty())
	{
		g.setColour(Colours::lightblue.withAlpha(0.82f));
		g.drawText(name, 0, 0, width, height, Justification::left);
	}
	else if (level == kLevel_Presets) g.drawFittedText(processor.collection->presets[preset]->name, 0, 0, width, height, Justification::left, 1);
	else if (level == kLevel_Sound_Groups) g.drawFittedText("Snd Group " + String(soundGroup + 1), 0, 0, width, height, Justification::left, 1);
	else if (level == kLevel_Preset_Layers) g.drawFittedText("Layer " + String(layer + 1), 0, 0, width, height, Justification::left, 1);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXPresetTreeItem::itemClicked(const MouseEvent& e)
{
	if (level > kLevel_AddPreset && !isOpen()) setOpen(true);
	//
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor.getActiveEditor();
	bool redoInterface = false;
	//
	if (level == kLevel_Presets)
	{
		editor->editObject.set(WusikEditObject::kPreset, preset, (void*)processor.collection->presets[preset]);
		redoInterface = true;
	}
	//
	if (level == kLevel_Preset_Layers)
	{
		editor->editObject.set(WusikEditObject::kPresetLayer, layer, (void*)processor.collection->presets[preset]->layers[layer]);
		redoInterface = true;
	}
	//
	if (redoInterface)
	{
		editor->redoTreeViewsOnResize = false;
		editor->cleanInterface();
		editor->updateInterface();
	}
}