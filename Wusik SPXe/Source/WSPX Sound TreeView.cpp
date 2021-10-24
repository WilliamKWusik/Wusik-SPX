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
	int _specialItem, WSPX_Collection_Sound* _sound, WSPX_Collection_Sound_File* _soundFile)
	: WSPXTreeItem(_processor, _level, _specialItem, _ui_ratio, _name), sound(_sound), soundFile(_soundFile)
{
	if (level == kLevel_Add_Sound)
	{
		for (int gg = 0; gg < processor.collection->sounds.size(); gg++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "", kRegular_Item, processor.collection->sounds[gg]));
		}
	}
	else if (level == kLevel_Sounds)
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "Remove", kSound_Remove, sound));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "Load File", kSound_File_Add, sound));
		//
		for (int ss = 0; ss < sound->soundFiles.size(); ss++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "", kRegular_Item, sound, sound->soundFiles[ss]));
		}
	}
	else if (level == kLevel_Sound_Files && name.isEmpty())
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_File_Options, "Remove", kSound_File_Remove, sound, soundFile));
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
	else if (level == kLevel_Sounds) g.drawText(sound->name, 0, 0, width, height, Justification::left);
	else if (level == kLevel_Sound_Files)
	{
		if (File(soundFile->soundFile).existsAsFile())
			g.drawFittedText(File(soundFile->soundFile).getFileNameWithoutExtension(), 0, 0, width, height, Justification::left, 1);
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
		if (WConfirmBox("Add New Sound", "Are you sure?"))
		{
			processor.collection->sounds.add(new WSPX_Collection_Sound);
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "", kRegular_Item, processor.collection->sounds.getLast()));
			openOnlyLast(this);
			//
			editor->editObject.set(WusikEditObject::kSound, 0, (void*)processor.collection->sounds.getLast());
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
		else reselectParent();
	}
	else if (level == kLevel_Sounds)
	{
		openOnly(this);
		editor->editObject.set(WusikEditObject::kSound, 0, (void*)sound);
		editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
	}
	else if (level == kLevel_Sound_Files)
	{
		if (specialItem == kRegular_Item)
		{
			openOnly(this);
			editor->editObject.set(WusikEditObject::kSoundFile, 0, (void*)soundFile);
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
		else if (specialItem == kSound_File_Add)
		{
			if (WConfirmBox("Add New Sound File", "Are you sure?"))
			{
				sound->soundFiles.add(new WSPX_Collection_Sound_File);
				getParentItem()->addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "", kRegular_Item, sound, sound->soundFiles.getLast()));
				openOnlyLast(getParentItem());
				//
				editor->editObject.set(WusikEditObject::kSoundFile, 0, (void*)sound->soundFiles.getLast());
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
			}
			else reselectParent();
		}
		else if (specialItem == kSound_File_Remove)
		{
			if (WConfirmBox("Remove Sound", "Are you sure?"))
			{
				StringArray presetsUsingThisSound;
				//
				for (int pp = 0; pp < processor.collection->presets.size(); pp++)
				{
					for (int ll = 0; ll < processor.collection->presets[pp]->layers.size(); ll++)
					{
						for (int ss = 0; ss < processor.collection->presets[pp]->layers[ll]->soundLinks.size(); ss++)
						{
							if (processor.collection->presets[pp]->layers[ll]->soundLinks[ss] == sound)
							{
								presetsUsingThisSound.add(processor.collection->presets[pp]->name);
							}
						}
					}
				}
				//
				if (presetsUsingThisSound.size() > 0)
				{
					WMessageBox("The following presets are using this sound. You must remove those presets before you can continue...", 
						presetsUsingThisSound.joinIntoString("\n"));
				}
				else
				{
					editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sound,
						processor.collection->sounds.indexOf(sound), getParentItem()->getIndexInParent(), 0, 0, (void*)getParentItem());
					return; // we exit quickly as this is about to go down //
				}
			}
			else reselectParent();
		}
	}
	else if (level == kLevel_Sound_File_Options)
	{
		if (specialItem == kSound_File_Remove)
		{
			if (WConfirmBox("Remove Sound File", "Are you sure?"))
			{
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sound_File,
					processor.collection->sounds.indexOf(sound), sound->soundFiles.indexOf(soundFile), 
					getParentItem()->getIndexInParent(), 0, (void*)getParentItem());
				return; // we exit quickly as this is about to go down //
			}
			else reselectParent();
		}
	}
}