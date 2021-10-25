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
	WSPX_Collection_Preset* _preset, int _specialItem, WSPX_Collection_Preset_Layer* _layer, WSPX_Collection_Sound* _soundLink)
	: WSPXTreeItem(_processor, _level, _specialItem, _ui_ratio, _name), preset(_preset), soundLink(_soundLink), layer(_layer)
{
	if (level == kLevel_Add_Preset)
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
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Links, "Remove", preset, kPreset_Layer_Remove, layer));
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Links, "Link Sound", preset, kPreset_Layer_Add_Sound_Link, layer));
		//
		for (int ss = 0; ss < layer->soundLinks.size(); ss++)
		{
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Links, "", preset, kRegular_Item, layer, processor.collection->sounds[ss]));
		}
	}
	else if (level == kLevel_Sound_Links && specialItem == kRegular_Item)
	{
		addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Links_Options, "Remove", preset, kSound_Link_Remove, layer, soundLink));
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
	else if (level == kLevel_Sound_Links) g.drawFittedText("Snd Link", 0, 0, width, height, Justification::left, 1);
	else if (level == kLevel_Preset_Layers) g.drawFittedText("Layer", 0, 0, width, height, Justification::left, 1);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXPresetTreeItem::itemClicked(const MouseEvent& e)
{
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor.getActiveEditor();
	//
	if (level == kLevel_Add_Preset)
	{
		if (WConfirmBox("Add New Preset", "Are you sure?"))
		{
			processor.collection->presets.add(new WSPX_Collection_Preset);
			addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Presets, "", processor.collection->presets.getLast()));
			openOnlyLast(this);
			//
			editor->presetChanged();
			editor->editObject.set(WusikEditObject::kPreset, processor.collection->presets.size() - 1, (void*)processor.collection->presets.getLast());
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
	}
	else if (level == kLevel_Presets)
	{
		openOnly(this);
		editor->editObject.set(WusikEditObject::kPreset, processor.collection->presets.indexOf(preset), (void*) preset);
		editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
	}
	else if (level == kLevel_Sound_Links)
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
			else reselectParent();
		}
		else if (specialItem == kPreset_Layer_Add_Sound_Link)
		{
			if (WConfirmBox("Add New Sound Link", "Are you sure?"))
			{
				StringArray soundItems;
				for (int gg = 0; gg < processor.collection->sounds.size(); gg++)
				{
					soundItems.add("Sound " + String(gg + 1) + ": " + processor.collection->sounds[gg]->name);
				}
				//
				AlertWindow w("Link Sound", "", AlertWindow::NoIcon);
				w.addComboBox("Sound", soundItems);
				w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
				w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
				w.setAlwaysOnTop(true);
				//
				int result = w.runModalLoop();
				if (result == 1)
				{
					int selectedSound = w.getComboBoxComponent("Sound")->getSelectedItemIndex();
					layer->soundLinks.add(processor.collection->sounds[selectedSound]);
					getParentItem()->addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Sound_Links, "", preset, kRegular_Item, layer, layer->soundLinks.getLast()));
					openOnlyLast(getParentItem());
					//
					editor->presetChanged();
					editor->editObject.set(WusikEditObject::kSound, 0, (void*)layer->soundLinks.getLast());
					editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				}
				else reselectParent();
			}
			else reselectParent();
		}
		else
		{
			openOnly(this);
			editor->editObject.set(WusikEditObject::kSound, 0, (void*)soundLink);
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
	}
	else if (level == kLevel_Sound_Links_Options)
	{
		if (specialItem == kSound_Link_Remove)
		{
			if (WConfirmBox("Remove Sound Link", "Are you sure?"))
			{
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sound_Link,
					processor.collection->presets.indexOf(preset), preset->layers.indexOf(layer), 
					layer->soundLinks.indexOf(soundLink), getParentItem()->getIndexInParent(), (void*)getParentItem());
				return; // we exit quickly as this is about to go down //
			}
			else reselectParent();
		}
	}
	else if (level == kLevel_Preset_Layers)
	{
		if (specialItem == kPreset_Remove)
		{
			if (WConfirmBox("Remove Preset", "Are you sure?"))
			{
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Preset, 
					processor.collection->presets.indexOf(preset), getParentItem()->getIndexInParent(), 0, 0, (void*)getParentItem());
				return; // we exit quickly as this is about to go down //
			}
			else reselectParent();
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
				getParentItem()->getParentItem()->addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Presets, "", processor.collection->presets.getLast()));
				openOnlyLast(getParentItem()->getParentItem());
				//
				editor->presetChanged();
				editor->editObject.set(WusikEditObject::kPreset, processor.collection->presets.size() - 1, (void*)processor.collection->presets.getLast());
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
			}
			else reselectParent();
		}
		else if (specialItem == kPreset_Move_Up)
		{
			if (processor.collection->presets.indexOf(preset) > 0)
			{
				editor->presetChanged();
				processor.collection->presets.move(processor.collection->presets.indexOf(preset), processor.collection->presets.indexOf(preset) - 1);
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Show_Collection);
				return; // we exit quickly as this is about to go down //
			}
			else reselectParent();
		}
		else if (specialItem == kPreset_Move_Down)
		{
			if (processor.collection->presets.indexOf(preset) < (processor.collection->presets.size() - 1))
			{
				editor->presetChanged();
				processor.collection->presets.move(processor.collection->presets.indexOf(preset), processor.collection->presets.indexOf(preset) + 1);
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Show_Collection);
				return; // we exit quickly as this is about to go down //
			}
			else reselectParent();
		}
		else if (specialItem == kPreset_Add_Layer)
		{
			if (WConfirmBox("Add New Layer", "Are you sure?"))
			{
				preset->layers.add(new WSPX_Collection_Preset_Layer);
				getParentItem()->addSubItem(new WSPXPresetTreeItem(processor, ui_ratio, kLevel_Preset_Layers, "", preset, kRegular_Item, preset->layers.getLast()));
				openOnlyLast(getParentItem());
				//
				editor->presetChanged();
				editor->editObject.set(WusikEditObject::kPresetLayer, 0, (void*)preset->layers.getLast());
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
			}
			else reselectParent();
		}
		else
		{
			openOnly(this);
			editor->editObject.set(WusikEditObject::kPresetLayer, preset->layers.indexOf(layer), (void*) layer);
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
	}
}