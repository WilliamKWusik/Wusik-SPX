/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "WSPX Components.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXTreeItem : public TreeViewItem
{
public:
	WSPXTreeItem(WusikSpxAudioProcessor& _processor, int _level, int _specialItem, double _ui_ratio, String _name)
		: processor(_processor), level(_level), specialItem(_specialItem), ui_ratio(_ui_ratio), name(_name) { };
	//
	bool mightContainSubItems() override { return getNumSubItems() != 0; }
	void reselectParent() { getParentItem()->setSelected(true, true, NotificationType::dontSendNotification); }
	int getItemHeight() const override { return 24.0 * ui_ratio; }
	//
	void openOnlyLast(TreeViewItem* item)
	{
		for (int ss = 0; ss < item->getNumSubItems(); ss++) { item->getSubItem(ss)->setOpen(false); }
		item->getSubItem(item->getNumSubItems() - 1)->setOpen(true);
		item->getSubItem(item->getNumSubItems() - 1)->setSelected(true, true, NotificationType::dontSendNotification);
	}
	void openOnly(TreeViewItem* item)
	{
		for (int ss = 0; ss < item->getParentItem()->getNumSubItems(); ss++) { item->getParentItem()->getSubItem(ss)->setOpen(false); }
		setOpen(true);
	}
	//
	WusikSpxAudioProcessor& processor;
	int level = 0;
	int specialItem = 0;
	double ui_ratio = 1.0;
	String name;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXPresetTreeItem : public WSPXTreeItem
{
public:
	WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name = String(), WSPX_Collection_Preset* _preset = nullptr,
		int _specialItem = 0, WSPX_Collection_Preset_Layer* _layer = nullptr, WSPX_Collection_Sound* _soundLink = nullptr);
	//
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	//
	WSPX_Collection_Preset* preset = nullptr;
	WSPX_Collection_Preset_Layer* layer = nullptr;
	WSPX_Collection_Sound* soundLink = nullptr;
	//
	enum
	{
		kLevel_Add_Preset = 0,
		kLevel_Presets,
		kLevel_Preset_Layers,
		kLevel_Sound_Links,
		kLevel_Sound_Links_Options,
		//
		kRegular_Item = 0,
		//
		kAdd_Preset = 1,
		//
		kPreset_Remove = 1,
		kPreset_Duplicate,
		kPreset_Move_Up,
		kPreset_Move_Down,
		kPreset_Add_Layer,
		kPreset_Layers,
		//
		kPreset_Layer_Remove = 1,
		kPreset_Layer_Add_Sound_Link,
		//
		kSound_Link_Remove = 1
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXSoundTreeItem : public WSPXTreeItem
{
public:
	WSPXSoundTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name = String(), 
		int _specialItem = 0, WSPX_Collection_Sound* _sound = nullptr, WSPX_Collection_Sound_File* _soundFile = nullptr);
	//
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	//
	WSPX_Collection_Sound* sound = nullptr;
	WSPX_Collection_Sound_File* soundFile = nullptr;
	//
	enum
	{
		kLevel_Add_Sound = 0,
		kLevel_Sounds,
		kLevel_Sound_Files,
		kLevel_Sound_File_Options,
		//
		kRegular_Item = 0,
		//
		kSound_Zones = 1,
		kSound_Remove,
		kSound_Add,
		//
		kSound_File_Remove = 1,
		kSound_File_Load
	};
};
//
// -------------------------------------------------------------------------------------------------------------------------------
class WusikTreeHolder : public Component
{
public:
	WusikTreeHolder(WusikSpxAudioProcessor& _processor, bool isPresetTreeView, double _ui_ratio) : processor(_processor)
	{
		addAndMakeVisible(tree);
		//
		if (isPresetTreeView)
			tree.setRootItem(new WSPXPresetTreeItem(processor, _ui_ratio, WSPXPresetTreeItem::kLevel_Add_Preset, "Add Preset"));
		else 
			tree.setRootItem(new WSPXSoundTreeItem(processor, _ui_ratio, WSPXSoundTreeItem::kLevel_Add_Sound, "Add Sound"));
		//
		tree.getRootItem()->setOpen(true);
		tree.setColour(TreeView::linesColourId, Colours::white);
	}
	//
	~WusikTreeHolder()
	{
		tree.deleteRootItem();
	}
	//
	void resized() override { tree.setBounds(getLocalBounds()); }
	TreeView tree;
	WusikSpxAudioProcessor& processor;
};