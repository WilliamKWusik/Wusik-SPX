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
void WusikSpxAudioProcessorEditor::timerCallback()
{
	stopTimer();
	//
	if (timerAction.get() == kTimerAction_Update_Interface || timerAction.get() == kTimerAction_Update_Interface_Show_Collection)
	{
		if (timerAction.get() == kTimerAction_Update_Interface_Show_Collection) editObject.set(WusikEditObject::kCollection, 0);
		//
		cleanInterface();
		updateInterface();
	}
	else if (timerAction.get() == kTimerAction_Update_Interface_Not_TreeViews)
	{
		redoTreeViewsOnResize = false;
		cleanInterface();
		updateInterface();
	}
	else if (timerAction.get() == kTimerAction_Remove_Preset)
	{
		editObject.set(WusikEditObject::kCollection, 0);
		WSPXPresetTreeItem* treeViewItem = (WSPXPresetTreeItem*)timerActionValueObject;
		treeViewItem->getParentItem()->removeSubItem(timerActionValue2);
		processor.collection->presets.remove(timerActionValue1);
		//
		presetChanged();
		redoTreeViewsOnResize = false;
		cleanInterface();
		updateInterface();
	}
	else if (timerAction.get() == kTimerAction_Remove_Layer)
	{
		editObject.set(WusikEditObject::kCollection, 0);
		WSPXPresetTreeItem* treeViewItem = (WSPXPresetTreeItem*) timerActionValueObject;
		treeViewItem->getParentItem()->removeSubItem(timerActionValue3);
		processor.collection->presets[timerActionValue1]->layers.remove(timerActionValue2);
		//
		presetChanged();
		redoTreeViewsOnResize = false;
		cleanInterface();
		updateInterface();
	}
	else if (timerAction.get() == kTimerAction_Remove_Sound_Link)
	{
		editObject.set(WusikEditObject::kCollection, 0);
		WSPXPresetTreeItem* treeViewItem = (WSPXPresetTreeItem*)timerActionValueObject;
		treeViewItem->getParentItem()->removeSubItem(timerActionValue4);
		processor.collection->presets[timerActionValue1]->layers[timerActionValue2]->soundLinks.remove(timerActionValue3);
		//
		presetChanged();
		redoTreeViewsOnResize = false;
		cleanInterface();
		updateInterface();
	}
	else if (timerAction.get() == kTimerAction_Remove_Sound)
	{
		editObject.set(WusikEditObject::kCollection, 0);
		WSPXPresetTreeItem* treeViewItem = (WSPXPresetTreeItem*)timerActionValueObject;
		treeViewItem->getParentItem()->removeSubItem(timerActionValue2);
		processor.collection->sounds.remove(timerActionValue1);
		//
		presetChanged();
		redoTreeViewsOnResize = false;
		cleanInterface();
		updateInterface();
	}
	else if (timerAction.get() == kTimerAction_Remove_Sound_File)
	{
		processor.stopAllAudio(); // We are about to mess with the audio data //
		//
		editObject.set(WusikEditObject::kCollection, 0);
		WSPXPresetTreeItem* treeViewItem = (WSPXPresetTreeItem*)timerActionValueObject;
		treeViewItem->getParentItem()->removeSubItem(timerActionValue3);
		processor.collection->sounds[timerActionValue1]->soundFiles.remove(timerActionValue2);
		//
		presetChanged();
		redoTreeViewsOnResize = false;
		cleanInterface();
		updateInterface();
		//
		processor.resumeAudio();
	}
	//
	timerAction.set(kTimerAction_None);
}