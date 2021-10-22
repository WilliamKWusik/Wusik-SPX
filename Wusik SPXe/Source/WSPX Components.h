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
	//
	Font getPopupMenuFont() override
	{
		return getCustomFont().withHeight(24.0f * ratioValue);
	};
	//
	void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, const bool isSeparator, const bool isActive, const bool isHighlighted, const bool isTicked, const bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon, const Colour* const textColourToUse) override
	{
		Colour textColour = Colours::white.withAlpha(0.90f);
		//
		if (isSeparator)
		{
			auto r = area.reduced(5, 0);
			r.removeFromTop(roundToInt((r.getHeight() * 0.5f) - 0.5f));

			g.setColour(textColour.withAlpha(0.3f));
			g.fillRect(r.removeFromTop(1));
		}
		else
		{
			//Colour textColour = (textColourToUse == nullptr ? textColour : *textColourToUse);

			auto r = area.reduced(1);

			if (isHighlighted && isActive)
			{
				g.setColour(Colour::fromString("FFFFECC1").withAlpha(0.22f));
				g.fillRect(r);
				g.setColour(Colours::white);
			}
			else
			{
				g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
			}

			r.reduce(jmin(5, area.getWidth() / 20), 0);

			auto font = getPopupMenuFont();

			auto maxFontHeight = r.getHeight() / 1.3f;

			if (font.getHeight() > maxFontHeight)
				font.setHeight(maxFontHeight);

			g.setFont(font);

			auto iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();

			if (icon != nullptr)
			{
				icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
				r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
			}
			else if (isTicked)
			{
				auto tick = getTickShape(1.0f);
				g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
			}

			if (hasSubMenu)
			{
				auto arrowH = 0.6f * getPopupMenuFont().getAscent();

				auto x = static_cast<float> (r.removeFromRight((int)arrowH).getX());
				auto halfH = static_cast<float> (r.getCentreY());

				Path path;
				path.startNewSubPath(x, halfH - arrowH * 0.5f);
				path.lineTo(x + arrowH * 0.6f, halfH);
				path.lineTo(x, halfH + arrowH * 0.5f);

				g.strokePath(path, PathStrokeType(2.0f));
			}

			r.removeFromRight(3);
			g.drawFittedText(text, r, Justification::centredLeft, 1);

			if (shortcutKeyText.isNotEmpty())
			{
				auto f2 = font;
				f2.setHeight(f2.getHeight() * 0.75f);
				f2.setHorizontalScale(0.95f);
				g.setFont(f2);

				g.drawText(shortcutKeyText, r, Justification::centredRight, true);
			}
		}
	};
	//
	Font getAlertWindowTitleFont() override { return getCustomFont().withHeight(42.0f * ratioValue); }
	Font getAlertWindowMessageFont() override { return getCustomFont().withHeight(30.0f * ratioValue); }
	Font getAlertWindowFont() override { return getCustomFont().withHeight(22.0f * ratioValue); }
	//
	void drawButtonText(Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		Font font = getCustomFont().withHeight(button.getHeight() * 0.72f * ratioValue);
		g.setFont(font);
		g.setColour(Colours::white.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

		if (shouldDrawButtonAsHighlighted) g.setColour(Colour::fromString("FFFFEEEE"));

		const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
		const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

		const int fontHeight = roundToInt(font.getHeight() * 0.6f);
		const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		const int textWidth = button.getWidth() - leftIndent - rightIndent;

		int leJustification = Justification::centred;
		if (button.getConnectedEdgeFlags() == TextButton::ConnectedOnLeft) leJustification = Justification::right;
		if (button.getConnectedEdgeFlags() == TextButton::ConnectedOnRight) leJustification = Justification::left;

		if (textWidth > 0)
			g.drawFittedText(button.getButtonText(),
				leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
				leJustification, 2);
	};
	//
	double ratioValue = 1.0;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WKnob : public Component // Fixed to Horizontal Mode //
{
public:
	void mouseDown(const MouseEvent& e) override
	{
		startDragValue = value[0];
		if (e.mods.isCtrlDown()) value[0] = 0.5f;
		if (e.mods.isMiddleButtonDown()) value[0] = 0.5f;
		repaint();
	};
	//
	void mouseDrag(const MouseEvent& e) override
	{
		float xMultiply = 1.0f;
		if (e.mods.isShiftDown() || e.mods.isRightButtonDown()) xMultiply = 0.1f;
		//
		value[0] = jlimit(0.0f, 1.0f, startDragValue + (float(e.getDistanceFromDragStartX()) * 0.006f * xMultiply));
		repaint();
		processor->setParameterNotifyingHost(parameterIndex, value[0]);
	};
	//
	void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override
	{
		value[0] = jlimit(0.0f, 1.0f, value[0] + (float(wheel.deltaY) * 0.1f));
		repaint();
		processor->setParameterNotifyingHost(parameterIndex, value[0]);
	};
	//
	void paint(Graphics& g) override
	{
		static int oneFrameH = knob->getHeight() / 128;
		g.setImageResamplingQuality(Graphics::highResamplingQuality);
		g.drawImage(knob[0], double(getWidth()) * 0.3, 0, double(getWidth()) * 0.7, getHeight(), 0, int(127.0f * value[0]) * oneFrameH, knob->getWidth(), oneFrameH);
		//
		String xText = String(value[0], 3);
		g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.6));
		g.setColour(Colours::white.withAlpha(0.82f));
		g.drawFittedText(xText, Rectangle<int>(0, 0, double(getWidth()) * 0.3, getHeight()), Justification::centredLeft, 1);
	}
	//
	int parameterIndex = 0;
	AudioProcessor* processor;
	Image* knob;
	float* value;
	float startDragValue = 1.0f;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditObject
{
public:
	void set(char _type, int _index, void* _object = nullptr) { type = _type; object = _object; index = _index; };
	char type = kCollection;
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
	WusikEditOption(WusikSpxAudioProcessor* _processor, Component* _editor, int _type, String _label, void* _object = nullptr, String _extraLabel = String(), bool _showEditInstead = false, WusikEditOptionCallback* _callback = nullptr);
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
					if (File(theImage->imageFilename).existsAsFile())
						g.drawFittedText(File(theImage->imageFilename).getFileNameWithoutExtension(), (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
					else
						g.drawFittedText("No File Selected", (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
				else if (type == kSlider || type == kSliderBipolar)
				{
					slider->setBounds(getWidth() - (double(getWidth()) * 0.42), 12, double(getWidth()) * 0.40, getHeight() - 16);
				}
			}
		}
	};
	//
	void* object;
	String label;
	WKnob* slider = nullptr;
	int type = 0;
	bool showEditInstead = false;
	ScopedPointer<WusikEditOptionCallback> callback = nullptr;
	WusikSpxAudioProcessor* processor;
	String extraLabel;
	Component* editor;
	//
	enum
	{
		kString,
		kImage,
		kLabel,
		kSlider,
		kSliderBipolar
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