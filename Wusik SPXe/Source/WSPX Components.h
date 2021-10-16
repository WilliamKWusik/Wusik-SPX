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
// -------------------------------------------------------------------------------------------------------------------------------
class LookAndFeelEx : public LookAndFeel_V4
{
public:
	static const Font getCustomFont()
	{
		static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::lucon_ttf, BinaryData::lucon_ttfSize);
		return Font(typeface);
	};
	//
	static const Font getCustomFontBold()
	{
		static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::lucon_ttf, BinaryData::lucon_ttfSize);
		return Font(typeface);
	};
	//
	Typeface::Ptr getTypefaceForFont(const Font& f) override
	{
		return getCustomFont().getTypeface();
	};
	//
	bool areLinesDrawnForTreeView(TreeView&) override
	{
		return true;
	}
	//
	void drawTreeviewPlusMinusBox(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isOpen, bool isMouseOver) override
	{
		Path p;
		p.addTriangle(0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);
		//
		g.setColour(Colours::red.withAlpha(0.66f));
		g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
	}
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditObject
{
public:
	void set(char _type, void* _object = nullptr) { type = _type; object = _object; };
	char type = kCollection;
	void* object = nullptr;
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
	virtual void process(WusikSpxAudioProcessor* processor) { };
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
	WusikEditOption(WusikSpxAudioProcessor* _processor, int _type, String _label, void* _object = nullptr, 
		String _extraLabel = String(), bool _showEditInstead = false, WusikEditOptionCallback* _callback = nullptr)
		: label(_label), object(_object), type(_type), showEditInstead(_showEditInstead), extraLabel(_extraLabel),
		callback(_callback), processor(_processor) { };
	void mouseMove(const MouseEvent& e) override { repaint(); };
	void mouseExit(const MouseEvent& e) override { repaint(); };
	void mouseEnter(const MouseEvent& e) override { repaint(); };
	//
	void mouseUp(const MouseEvent& e) override 
	{
		if (type == kString)
		{
			String sValue;
			AskValue(label, extraLabel, ((String*)object)[0], "", "OK", "Cancel", sValue);
			if (sValue.isNotEmpty()) ((String*)object)[0] = sValue;
		}
		//
		if (callback != nullptr) callback->process(processor);
		repaint(); 
	};
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
			if (isMouseOver()) g.fillAll(Colours::darkblue.withAlpha(0.42f)); else g.fillAll(Colours::darkblue.withAlpha(0.12f));
			//
			g.setColour(Colours::white.withAlpha(0.26f));
			g.drawRect(0, 0, getWidth(), getHeight(), 1);
			g.setColour(Colours::white.withAlpha(0.86f));
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.32f));
			g.drawFittedText(label, 8, 0, (double(getWidth()) * 0.26) - 16, getHeight(), Justification::centredLeft, 1);
			//
			if (showEditInstead)
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
					if (theImage->imageFilename.existsAsFile())
						g.drawFittedText(theImage->imageFilename.getFileNameWithoutExtension(), (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
					else
						g.drawFittedText("No File Selected", (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
			}
		}
	};
	//
	void* object;
	String label;
	int type = 0;
	bool showEditInstead = false;
	ScopedPointer<WusikEditOptionCallback> callback = nullptr;
	WusikSpxAudioProcessor* processor;
	String extraLabel;
	//
	enum
	{
		kString,
		kImage,
		kLabel
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXSoundZone : public Component
{
public:
	WSPXSoundZone(WSPX_Collection_Sound* _sound) : sound(_sound) { };
	//
	void paint(Graphics& g) override
	{
		g.fillAll(Colours::yellow.withAlpha(0.22f));
		g.setColour(Colours::white.withAlpha(0.66f));
		g.drawRect(0, 0, getWidth(), getHeight(), 2);
	};
	//
	WSPX_Collection_Sound* sound;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXPresetTreeItem : public TreeViewItem
{
public:
	WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, uint8_t _level, String _name = String(), int16 _preset = 0, uint8_t _specialItem = 0, int16 _layer = 0, int16 _soundGroup = 0, int16 _sound = 0);
	bool mightContainSubItems() override { return getNumSubItems() != 0; }
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	int getItemHeight() const override { return 24.0 * ui_ratio; };
	//
	WusikSpxAudioProcessor& processor;
	int16 preset = 0;
	int16 layer = 0;
	int16 soundGroup = 0;
	int16 sound = 0;
	uint8_t level = 0;
	uint8_t specialItem = 0;
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
	WSPXSoundTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, uint8_t _level, String _name = String(), uint8_t _specialItem = 0, int16 _soundGroup = 0, int16 _sound = 0);
	bool mightContainSubItems() override { return getNumSubItems() != 0; }
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	int getItemHeight() const override { return 24.0 * ui_ratio; };
	//
	WusikSpxAudioProcessor& processor;
	int16 soundGroup = 0;
	int16 sound = 0;
	uint8_t level = 0;
	uint8_t specialItem = 0;
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
			tree.setRootItem(new WSPXPresetTreeItem(processor, _ui_ratio, WSPXPresetTreeItem::kLevel_AddPreset, "Add Preset", WSPXPresetTreeItem::kAdd_Preset));
		else 
			tree.setRootItem(new WSPXSoundTreeItem(processor, _ui_ratio, WSPXSoundTreeItem::kLevel_AddSound, "Add Sound", WSPXSoundTreeItem::kSound_Group_Add_Sound));
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
	WTransparentButton(Button::Listener* _owner) : TextButton(""), owner(_owner) { addListener(owner); };
	~WTransparentButton() { removeListener(owner); };
	void paint(Graphics& g) override { };
	Button::Listener* owner;
};