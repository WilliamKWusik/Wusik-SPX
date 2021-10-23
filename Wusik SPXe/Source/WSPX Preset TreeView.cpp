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
WSPXPresetTreeItem::WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name, 
	WSPX_Collection_Preset* _preset, int _specialItem, WSPX_Collection_Preset_Layer* _layer, 
	WSPX_Collection_Sound_Group* _soundGroup, WSPX_Collection_Sound* _sound)
	: processor(_processor), preset(_preset), level(_level), ui_ratio(_ui_ratio), soundGroup(_soundGroup), 
	layer(_layer), specialItem(_specialItem), sound(_sound), name(_name)
{
	if (level == kLevel_AddPreset)
	{
		for (int pp = 0; pp < processor.collection->presets.size(); pp++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Presets, "", processor.collection->presets[pp]));
		}
	}
	else if (level == kLevel_Presets)
	{
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Remove", preset, kPreset_Remove));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Duplicate", preset, kPreset_Duplicate));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Move Up", preset, kPreset_Move_Up));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Move Down", preset, kPreset_Move_Down));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "Add Layer", preset, kPreset_Add_Layer));
		//
		for (int ll = 0; ll < preset->layers.size(); ll++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "", preset, kRegular_Item, preset->layers[ll]));
		}
	}
	else if (level == kLevel_Preset_Layers && specialItem == kRegular_Item)
	{
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "Remove", preset, kPreset_Layer_Remove, layer));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "Add Group", preset, kPreset_Layer_Add_Sound_Group, layer));
		//
		for (int ss = 0; ss < layer->soundGroupIDs.size(); ss++)
		{
			//addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "", preset, kRegular_Item, layer, layer->soundGroupIDs[ss]));
		}
	}
	else if (level == kLevel_Sound_Groups && specialItem == kRegular_Item)
	{
		//addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Groups_Options, "Remove", preset, kSound_Group_Remove, layer, sound));
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
	else if (level == kLevel_Presets) g.drawFittedText(preset->name, 0, 0, width, height, Justification::left, 1);
	else if (level == kLevel_Sound_Groups) g.drawFittedText("Snd Group", 0, 0, width, height, Justification::left, 1);
	else if (level == kLevel_Preset_Layers) g.drawFittedText("Layer", 0, 0, width, height, Justification::left, 1);
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
	if (level == kLevel_AddPreset)
	{
		if (WConfirmBox("Add New Preset", "Are you sure?"))
		{
			processor.collection->presets.add(new WSPX_Collection_Preset);
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface);
			return; // we exit quickly as this is about to go down //
		}
	}
	if (level == kLevel_Presets)
	{
		editor->editObject.set(WusikEditObject::kPreset, processor.collection->presets.indexOf(preset), (void*) preset);
		redoInterface = true;
	}
	else if (level == kLevel_Sound_Groups)
	{
		if (specialItem == kPreset_Layer_Remove)
		{
			if (WConfirmBox("Remove Layer", "Are you sure?"))
			{
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Layer, 
					processor.collection->presets.indexOf(preset), 
					preset->layers.indexOf(layer), getParentItem()->getIndexInParent(), 0, (void*) getParentItem());
				return; // we exit quickly as this is about to go down //
			}
		}
	}
	else if (level == kLevel_Sound_Groups_Options)
	{
		if (specialItem == kSound_Group_Remove)
		{
			if (WConfirmBox("Remove Sound Group", "Are you sure?"))
			{
				return;
			}
		}
	}
	else if (level == kLevel_Preset_Layers)
	{
		if (specialItem == kPreset_Remove)
		{
			if (WConfirmBox("Remove Preset", "Are you sure?"))
			{
				//editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Preset, preset);
				return; // we exit quickly as this is about to go down //
			}
		}
		else if (specialItem == kPreset_Duplicate)
		{
			if (WConfirmBox("Duplicate Preset", "Are you sure?"))
			{
				MemoryBlock xData;
				MemoryOutputStream streamOut(xData, false);
				preset->streamData((void *) &streamOut, WS::kWrite);
				//
				processor.collection->presets.add(new WSPX_Collection_Preset);
				//
				MemoryInputStream sendData(xData.getData(), xData.getSize(), false);
				processor.collection->presets.getLast()->streamData((void *) &sendData, WS::kRead);
				//
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Show_Collection);
				return; // we exit quickly as this is about to go down //
			}
		}
		else if (specialItem == kPreset_Move_Up)
		{
			if (preset > 0)
			{
				//processor.collection->presets.move(preset, preset - 1);
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Show_Collection);
				return; // we exit quickly as this is about to go down //
			}
		}
		else if (specialItem == kPreset_Move_Down)
		{
			/*if (preset < (processor.collection->presets.size() - 1))
			{
				//processor.collection->presets.move(preset, preset + 1);
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Show_Collection);
				return; // we exit quickly as this is about to go down //
			}*/
		}
		else if (specialItem == kPreset_Add_Layer)
		{
			//processor.collection->presets[preset]->layers.add(new WSPX_Collection_Preset_Layer);
			//getParentItem()->addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "", preset, kRegular_Item, processor.collection->presets[preset]->layers.size() - 1));
		}
		else
		{
			//editor->editObject.set(WusikEditObject::kPresetLayer, layer, (void*)processor.collection->presets[preset]->layers[layer]);
			redoInterface = true;
		}
	}
	//
	if (redoInterface)
	{
		editor->redoTreeViewsOnResize = false;
		editor->cleanInterface();
		editor->updateInterface();
	}
}