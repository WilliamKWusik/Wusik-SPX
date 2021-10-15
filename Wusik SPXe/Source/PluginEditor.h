/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXPresetTreeItem : public TreeViewItem
{
public:
	WSPXPresetTreeItem(WusikSpxAudioProcessor& _processor, uint8_t _level, int16 _preset, double _ui_ratio, int16 _sound = 0, int16 _subSound = 0);
	bool mightContainSubItems() override { return getNumSubItems() != 0; }
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	int getItemHeight() const override { return 24.0 * ui_ratio; };
	//
	WusikSpxAudioProcessor& processor;
	int16 preset = 0;
	int16 sound = 0;
	int16 subSound = 0;
	uint8_t level = 0;
	double ui_ratio = 1.0;
	//
	enum
	{
		kLevel_AddPreset = 0,
		kLevel_PresetName,
		kLevel_PresetName_Options,
		kLevel_Sounds,
		kLevel_Sounds_Options,
		kLevel_SubSounds,
		kLevel_SubSounds_Options,
		//
		kPreset_Options = -1,
		kPreset_Remove = -2,
		kPreset_Duplicate = -3,
		kPreset_Add_Sound = -4,
		kPreset_Sounds = -5,
		//
		kSound_Options = -1,
		kSound_Remove = -2,
		kSound_Add_SubSound = -3,
		kSound_SubSounds = -4,
		//
		kSubSound_Options = -1,
		kSubSound_Remove = -2
	};
	//
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WSPXPresetTreeItem)
};
//
// -------------------------------------------------------------------------------------------------------------------------------
class WusikTreeHolder : public Component
{
public:
	WusikTreeHolder(WusikSpxAudioProcessor& _processor, bool isPresetTreeView, double _ui_ratio) : processor(_processor)
	{
		addAndMakeVisible(tree);
		if (isPresetTreeView) tree.setRootItem(new WSPXPresetTreeItem(processor, WSPXPresetTreeItem::kLevel_AddPreset, 0, _ui_ratio));
		tree.getRootItem()->setOpen(true);
		tree.setColour(TreeView::linesColourId, Colours::white);
	}
	//
	~WusikTreeHolder()
	{
		tree.deleteRootItem(); // this deletes the children too...
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
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikSpxAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener
{
public:
    WusikSpxAudioProcessorEditor (WusikSpxAudioProcessor&);
    ~WusikSpxAudioProcessorEditor();
	void buttonClicked(Button* buttonThatWasClicked);
    void paint (Graphics&) override;
    void resized() override;
	//
	void boundsSet(int xx, int yy, int ww, int hh, Component* compo, double multRatio)
	{
		compo->setBounds(double(xx) * multRatio, double(yy) * multRatio, double(ww) * multRatio, double(hh) * multRatio);
	}
	//
	Image background;
	Image background2; // Options & Close //
	Image background3; // UnSaved Changes //
	Image tempImage;
	double uiRatio = 1.0;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizerConstrainer;
	ScopedPointer<LookAndFeelEx> newLookAndFeel;
	Label* collectionNameLabel;
	bool showingOptions = false;
	WusikTreeHolder* presetsTreeView;
	//
	WTransparentButton* logoButton;
	WTransparentButton* fileButton;
	WTransparentButton* saveButton;
	WTransparentButton* collectionButton;
	//
private:
    WusikSpxAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessorEditor)
};
