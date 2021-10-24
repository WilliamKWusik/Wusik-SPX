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
WSPXSoundTreeItem::WSPXSoundTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name, 
	int _specialItem, WSPX_Collection_Sound* _soundGroup, WSPX_Collection_Sound_File* _sound)
	: processor(_processor), level(_level), ui_ratio(_ui_ratio), soundGroup(_soundGroup), specialItem(_specialItem),  sound(_sound), name(_name)
{
	if (level == kLevel_Add_Sound)
	{
		for (int gg = 0; gg < processor.collection->sounds.size(); gg++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "", kRegular_Item, processor.collection->sounds[gg]));
		}
	}
	else if (level == kLevel_Sound_Groups)
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "Remove", kSound_Group_Remove, soundGroup));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "Load File", kSound_Group_Add_Sound, soundGroup));
		//
		for (int ss = 0; ss < soundGroup->soundFiles.size(); ss++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "", kRegular_Item, soundGroup, soundGroup->soundFiles[ss]));
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
	else if (level == kLevel_Sound_Groups) g.drawText(soundGroup->name, 0, 0, width, height, Justification::left);
	else if (level == kLevel_Sounds)
	{
		if (File(sound->soundFile).existsAsFile())
			g.drawFittedText(File(sound->soundFile).getFileNameWithoutExtension(), 0, 0, width, height, Justification::left, 1);
		else 
			g.drawText("No File", 0, 0, width, height, Justification::left);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXSoundTreeItem::itemClicked(const MouseEvent& e)
{
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor.getActiveEditor();
	//
	if (level == kLevel_Add_Sound)
	{
		processor.collection->sounds.add(new WSPX_Collection_Sound);
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Groups, "", kRegular_Item, processor.collection->sounds.getLast()));
		openOnlyLast(this);
		//
		editor->editObject.set(WusikEditObject::kSoundGroup, 0, (void*)processor.collection->sounds.getLast());
		editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
	}
	else if (level == kLevel_Sound_Groups)
	{
		openOnly(this);
		editor->editObject.set(WusikEditObject::kSoundGroup, 0, (void*)soundGroup);
		editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
	}

}