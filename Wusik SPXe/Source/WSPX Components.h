/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WSPX Look And Feel.h"
#include "WSPX Knob.h"
//
// -------------------------------------------------------------------------------------------------------------------------------
#define WAlert AlertWindow::showMessageBox(AlertWindow::NoIcon, "!", "!")
#define WConfirmBox(a,b) AlertWindow::showOkCancelBox(AlertWindow::NoIcon, a, b)
#define WMessageBox(dd,ds) AlertWindow::showMessageBox(AlertWindow::NoIcon, dd,ds)
#define WShowValue(value) AlertWindow::showMessageBox(AlertWindow::NoIcon, "","" + String::formatted("%d",value))
#define AskValue(Label1, Label2, Value, ValueLabel, OK, CANCEL, FinalValue) AlertWindow w(Label1, Label2, AlertWindow::NoIcon);\
	w.addTextEditor("Value", Value, ValueLabel); \
	w.addButton(OK, 1, KeyPress(KeyPress::returnKey, 0, 0)); \
	w.addButton(CANCEL, 0, KeyPress(KeyPress::escapeKey, 0, 0)); \
	if (w.runModalLoop() == 1) FinalValue = w.getTextEditorContents("Value"); else FinalValue = String();
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditObject
{
public:
	void set(int _type, int _index, void* _object = nullptr) { type = _type; object = _object; index = _index; }
	int type = kCollection;
	void* object = nullptr;
	int index = 0;
	//
	enum
	{
		kNone,
		kCollection,
		kPreset,
		kPresetLayer,
		kSoundGroup,
		kSound
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditOptionCallback
{
public:
	virtual void process(WusikSpxAudioProcessor* processor) { }
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditOptionCallback_UpdateCollectionName : public WusikEditOptionCallback
{
public:
	virtual void process(WusikSpxAudioProcessor* processor);
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditOption : public Component
{
public:
	WusikEditOption(WusikSpxAudioProcessor* _processor, Component* _editor, int _type, String _label, void* _object = nullptr, 
		String _extraLabel = String(), bool _showEditInstead = false, WusikEditOptionCallback* _callback = nullptr, int _min = 0, 
		int _max = 1, String _popupList = String());
	//
	void mouseMove(const MouseEvent& e) override { repaint(); }
	void mouseExit(const MouseEvent& e) override { repaint(); }
	void mouseEnter(const MouseEvent& e) override { repaint(); }
	//
	void mouseUp(const MouseEvent& e) override
	{
		if (type == kPopupList)
		{
			int index = jlimit(0, popupList.size() - 1, int(((float*)object)[0] * float(popupList.size() - 1)));
			PopupMenu mm;
			//
			for (int tt = 0; tt < popupList.size(); tt++)
			{
				mm.addItem(tt + 1, popupList[tt], true, index == tt);
			}
			//
			int result = mm.show();
			if (result > 0)
			{
				((float*)object)[0] = float(result - 1) / float(popupList.size() - 1);
			}
		}
		else if (type == kOnOffButton)
		{
			if (((float*)object)[0] > 0.0f) ((float*)object)[0] = 0.0f; else ((float*)object)[0] = 1.0f;
		}
		else if (type == kString)
		{
			String sValue;
			AskValue(label, extraLabel, ((String*)object)[0], "", "OK", "Cancel", sValue);
			if (sValue.isNotEmpty())
			{
				((String*)object)[0] = sValue;
				processor->collection->hasUnsavedChanges = true;
			}
		}
		else if (type == kImage)
		{
			bool loadImage = true;
			WSPX_Image* theImage = (WSPX_Image*)object;
			//
			if (File(theImage->imageFilename).existsAsFile())
			{
				loadImage = false;
				PopupMenu mm;
				mm.addItem(1, "View Image");
				mm.addItem(2, "Remove Image");
				mm.addItem(4, "Replace Image");
				//
				int result = mm.show();
				if (result > 0)
				{
					if (result == 2)
					{
						theImage->imageFilename = String();
						theImage->image = Image();
						processor->collection->hasUnsavedChanges = true;
					}
					else if (result == 4) loadImage = true;
					else if (result == 1) File(theImage->imageFilename).startAsProcess();
				}
			}
			//
			if (loadImage)
			{
				FileChooser browseFile("Load Image", File(theImage->imageFilename).getParentDirectory().getFullPathName(), "*.png;*.jpg");
				//
				if (browseFile.browseForFileToOpen())
				{
					theImage->imageFilename = browseFile.getResult().getFullPathName();
					theImage->image = ImageFileFormat::loadFrom(theImage->imageFilename);
					processor->collection->hasUnsavedChanges = true;
				}
			}
		}
		//
		if (callback != nullptr) callback->process(processor);
		editor->repaint();
	}
	//
	void paint(Graphics& g) override
	{
		if (type == kLabel)
		{
			g.fillAll(Colours::darkblue.withAlpha(0.22f));
			g.setColour(Colours::white.withAlpha(0.94f));
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.52f));
			g.drawFittedText(label, 0, 4, getWidth(), getHeight(), Justification::centred, 1);
		}
		else
		{
			if (isMouseOver(true)) g.fillAll(Colours::darkblue.withAlpha(0.42f)); else g.fillAll(Colours::darkblue.withAlpha(0.12f));
			//
			g.setColour(Colours::white.withAlpha(0.26f));
			g.drawRect(0, 0, getWidth(), getHeight(), 1);
			g.setColour(Colours::white.withAlpha(0.86f));
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.32f));
			g.drawFittedText(label, 8, 0, (double(getWidth()) * 0.26) - 16, getHeight(), Justification::centredLeft, 1);
			//
			if (type == kPopupList)
			{
				int index = ((float*)object)[0] * float(popupList.size() - 1);
				String xText = popupList[jlimit(0, popupList.size() - 1, int(index))];
				g.drawFittedText(xText, 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
			}
			else if (type == kOnOffButton)
			{
				if (((float*)object)[0] > 0.0f)
				{
					g.drawFittedText("ENABLED", 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
				}
				else
				{
					g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.28f));
					g.setColour(Colours::white.withAlpha(0.46f));
					g.drawFittedText("DISABLED", 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
				}
			}
			else if (showEditInstead)
			{
				g.drawFittedText("EDIT", 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
			}
			else
			{
				if (type == kString)
				{
					g.drawFittedText(((String*)object)[0], (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
				else if (type == kImage)
				{
					WSPX_Image* theImage = (WSPX_Image*)object;
					if (File(theImage->imageFilename).existsAsFile())
						g.drawFittedText(File(theImage->imageFilename).getFileNameWithoutExtension(), (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
					else
						g.drawFittedText("No File Selected", (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
				else if (type >= kSlider && type <= kSliderIntegerBipolar)
				{
					slider->setBounds(getWidth() - (double(getWidth()) * 0.42), 12, double(getWidth()) * 0.40, getHeight() - 16);
				}
			}
		}
	}
	//
	void* object;
	String label;
	WSlider* slider = nullptr;
	int type = 0;
	int min = 0;
	int max = 1;
	bool showEditInstead = false;
	ScopedPointer<WusikEditOptionCallback> callback = nullptr;
	WusikSpxAudioProcessor* processor;
	String extraLabel;
	Component* editor;
	StringArray popupList;
	//
	enum
	{
		kString,
		kImage,
		kLabel,
		kPopupList,
		kSlider,
		kSliderBipolar,
		kSliderInteger,
		kSliderIntegerBipolar,
		kOnOffButton
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXSoundZone : public Component
{
public:
	WSPXSoundZone(WSPX_Collection_Sound* _sound) : sound(_sound) { }
	//
	void paint(Graphics& g) override
	{
		g.fillAll(Colours::yellow.withAlpha(0.22f));
		g.setColour(Colours::white.withAlpha(0.66f));
		g.drawRect(0, 0, getWidth(), getHeight(), 2);
	}
	//
	WSPX_Collection_Sound* sound;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXPresetTreeItem : public TreeViewItem
{
public:
	WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name = String(), 
		WSPX_Collection_Preset* _preset = nullptr, int _specialItem = 0, WSPX_Collection_Preset_Layer* _layer = nullptr, 
		WSPX_Collection_Sound_Group* _soundGroup = nullptr, WSPX_Collection_Sound* _sound = nullptr);
	//
	bool mightContainSubItems() override { return getNumSubItems() != 0; }
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	int getItemHeight() const override { return 24.0 * ui_ratio; }
	void reselectParent() { getParentItem()->setSelected(true, true, NotificationType::dontSendNotification); }
	//
	WusikSpxAudioProcessor& processor;
	WSPX_Collection_Preset* preset = nullptr;
	WSPX_Collection_Preset_Layer* layer = nullptr;
	WSPX_Collection_Sound_Group* soundGroup = nullptr;
	WSPX_Collection_Sound* sound = nullptr;
	int level = 0;
	int specialItem = 0;
	double ui_ratio = 1.0;
	String name;
	//
	enum
	{
		kLevel_AddPreset = 0,
		kLevel_Presets,
		kLevel_Preset_Layers,
		kLevel_Sound_Groups,
		kLevel_Sound_Groups_Options,
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
		kPreset_Layer_Add_Sound_Group,
		//
		kSound_Group_Remove = 1
	};
	//
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WSPXPresetTreeItem)
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXSoundTreeItem : public TreeViewItem
{
public:
	WSPXSoundTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name = String(), 
		int _specialItem = 0, WSPX_Collection_Sound_Group* _soundGroup = nullptr, WSPX_Collection_Sound* _sound = nullptr);
	//
	bool mightContainSubItems() override { return getNumSubItems() != 0; }
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	int getItemHeight() const override { return 24.0 * ui_ratio; }
	//
	WusikSpxAudioProcessor& processor;
	WSPX_Collection_Sound_Group* soundGroup = nullptr;
	WSPX_Collection_Sound* sound = nullptr;
	int level = 0;
	int specialItem = 0;
	double ui_ratio = 1.0;
	String name;
	//
	enum
	{
		kLevel_AddSound = 0,
		kLevel_Sound_Groups,
		kLevel_Sounds,
		kLevel_Sounds_Options,
		//
		kRegular_Item = 0,
		//
		kSound_Group_Remove = 1,
		kSound_Group_Add_Sound,
		//
		kSound_Remove = 1
	};
	//
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WSPXSoundTreeItem)
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
			tree.setRootItem(new WSPXPresetTreeItem(processor, _ui_ratio, WSPXPresetTreeItem::kLevel_AddPreset, "Add Preset"));
		else 
			tree.setRootItem(new WSPXSoundTreeItem(processor, _ui_ratio, WSPXSoundTreeItem::kLevel_AddSound, "Add Sound"));
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
//
// -------------------------------------------------------------------------------------------------------------------------------
class WTransparentButton : public TextButton
{
public:
	WTransparentButton(Button::Listener* _owner) : TextButton(""), owner(_owner) { addListener(owner); }
	~WTransparentButton() { removeListener(owner); }
	void paint(Graphics& g) override { }
	Button::Listener* owner;
};