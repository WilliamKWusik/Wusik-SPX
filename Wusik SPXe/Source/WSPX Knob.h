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
#define WAskValue(Label1, Label2, Value, ValueLabel, OK, CANCEL, FinalValue) AlertWindow w(Label1, Label2, AlertWindow::NoIcon);\
	w.addTextEditor("Value", Value, ValueLabel); \
	w.addButton(OK, 1, KeyPress(KeyPress::returnKey, 0, 0)); \
	w.addButton(CANCEL, 0, KeyPress(KeyPress::escapeKey, 0, 0)); \
	if (w.runModalLoop() == 1) FinalValue = w.getTextEditorContents("Value"); else FinalValue = String();
//
// ------------------------------------------------------------------------------------------------------------------------- //
static const uint8 whiteNotes[] = { 0, 2, 4, 5, 7, 9, 11 };
static const uint8 blackNotes[] = { 1, 3, 6, 8, 10 };
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WMidiKeyboardComponent : public MidiKeyboardComponent
{
public:
	WMidiKeyboardComponent(MidiKeyboardState& mstate)
		: MidiKeyboardComponent(mstate, MidiKeyboardComponent::Orientation::horizontalKeyboard) { }
	//
	void drawWhiteNote(int midiNoteNumber, Graphics& g, Rectangle<float> area, bool isDown, bool isOver, Colour lineColour, Colour textColour) override
	{
		auto c = Colours::transparentWhite;
		if (isDown)  c = findColour(keyDownOverlayColourId);
		if (isOver)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));
		if (midiNoteNumber >= selectedLow && midiNoteNumber <= selectedHigh) c = Colours::red.withAlpha(0.48f);
		if (midiNoteNumber == rootKey) c = Colours::yellow.withAlpha(0.48f);
		//
		g.setColour(c);
		g.fillRect(area);
		//
		auto text = getWhiteNoteText(midiNoteNumber);
		//
		if (text.isNotEmpty())
		{
			auto fontHeight = jmin(12.0f, keyWidth * 0.9f);
			g.setColour(textColour);
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(fontHeight).withHorizontalScale(0.8f));
			g.drawText(text, area.withTrimmedLeft(1.0f).withTrimmedBottom(2.0f), Justification::centredBottom, false);
		}
		//
		if (!lineColour.isTransparent())
		{
			g.setColour(lineColour);
			g.fillRect(area.withWidth(1.0f));
			if (midiNoteNumber == rangeEnd) g.fillRect(area.expanded(1.0f, 0).removeFromRight(1.0f));
		}
	}
	//
	void drawBlackNote(int midiNoteNumber, Graphics& g, Rectangle<float> area, bool isDown, bool isOver, Colour noteFillColour) override
	{
		auto c = noteFillColour;
		if (isDown)  c = c.overlaidWith(findColour(keyDownOverlayColourId));
		if (isOver)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));
		if (midiNoteNumber >= selectedLow && midiNoteNumber <= selectedHigh) c = Colours::red.darker(0.8f);
		if (midiNoteNumber == rootKey) c = Colours::yellow.darker(0.72f);
		//
		g.setColour(c);
		g.fillRect(area);
		//
		if (isDown)
		{
			g.setColour(noteFillColour);
			g.drawRect(area);
		}
		else
		{
			g.setColour(c.brighter());
			auto sideIndent = 1.0f / 8.0f;
			auto topIndent = 7.0f / 8.0f;
			auto w = area.getWidth();
			auto h = area.getHeight();
			g.fillRect(area.reduced(w * sideIndent, 0).removeFromTop(h * topIndent));
		}
	}
	//
	int selectedLow = -1;
	int selectedHigh = -1;
	int rootKey = -1;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSlider : public Component
{
public:
	WSlider(Image& _background, Image& _filled, Image& _thumb, float& _value, bool _bipolar,
		bool _integer = false, int _min = 0, int _max = 1)
		: background(_background), filled(_filled), thumb(_thumb), value(_value), bipolar(_bipolar), integer(_integer), min(_min), max(_max)
	{ 
		originalSizeImage = Image(Image::ARGB, background.getWidth(), background.getHeight(), true);
		tempFill = Image(Image::ARGB, filled.getWidth(), filled.getHeight(), true);
		if (integer) totalInt = max - min;
	}
	//
	void mouseEnter(const MouseEvent& e) override
	{
		if (getParentComponent() != nullptr) getParentComponent()->repaint();
	}
	//
	void mouseExit(const MouseEvent& e) override
	{
		if (getParentComponent() != nullptr) getParentComponent()->repaint();
		//
		if (midiKeyboard != nullptr)
		{
			midiKeyboardValue[0] = value * 127.0f;
			midiKeyboard->repaint();
		}
	}
	//
	void mouseUp(const MouseEvent& e) override
	{
		if (wasOntopOfValue)
		{
			String xVal;
			String xLabel;
			//
			if (integer)
			{
				if (min > 1 || max < 0) xLabel = "Min: " + String(min) + " - Max: " + String(max);
				xVal = String(int(value * totalInt) + min);
			}
			else if (bipolar) xVal = String((value * 2.0f) - 1.0f, 6);
			else xVal = String(value, 6);
			//
			WAskValue("Edit Value", xLabel, xVal, "", "OK", "Cancel", xVal);
			if (xVal.isNotEmpty())
			{
				if (integer) value = jlimit(0.0f, 1.0f, (float(xVal.getFloatValue() - min) / totalInt));
				else if (bipolar) value = jlimit(0.0f, 1.0f, (xVal.getFloatValue() + 1.0f) * 0.5f);
				else value = jlimit(0.0f, 1.0f, xVal.getFloatValue());
			}
			repaint();
		}
		//
		if (getParentComponent() != nullptr) getParentComponent()->repaint();
		//
		if (midiKeyboard != nullptr)
		{
			midiKeyboardValue[0] = value * 127.0f;
			midiKeyboard->repaint();
		}
	}
	//
	void mouseDown(const MouseEvent& e) override
	{
		wasOntopOfValue = e.getPosition().x <= int(double(getWidth()) * 0.28);
		startDragValue = value;
		if (e.mods.isCtrlDown()) value = 0.5f;
		if (e.mods.isMiddleButtonDown()) value = 0.5f;
		repaint();
		if (getParentComponent() != nullptr) getParentComponent()->repaint();
	}
	//
	void mouseDrag(const MouseEvent& e) override
	{
		if (wasOntopOfValue) return;
		//
		float xMultiply = 1.0f;
		if (e.mods.isShiftDown() || e.mods.isRightButtonDown()) xMultiply = 0.1f;
		//
		value = jlimit(0.0f, 1.0f, startDragValue + (float(e.getDistanceFromDragStartX()) * 0.006f * xMultiply));
		repaint();
		if (getParentComponent() != nullptr) getParentComponent()->repaint();
		//
		if (midiKeyboard != nullptr)
		{
			midiKeyboardValue[0] = value * 127.0f;
			midiKeyboard->repaint();
		}
	}
	//
	void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override
	{
		value = jlimit(0.0f, 1.0f, value + (float(wheel.deltaY) * 0.1f));
		repaint();
		if (getParentComponent() != nullptr) getParentComponent()->repaint();
		//
		if (midiKeyboard != nullptr)
		{
			midiKeyboardValue[0] = value * 127.0f;
			midiKeyboard->repaint();
		}
	}
	//
	void paint(Graphics& g) override
	{
		originalSizeImage.clear(Rectangle<int>(0, 0, originalSizeImage.getWidth(), originalSizeImage.getHeight()), Colours::transparentBlack);
		Graphics gg(originalSizeImage);
		gg.drawImageAt(background, 0, 0);
		//
		int thumbPos; 
		if (integer) thumbPos = (double(background.getWidth() - thumb.getWidth()) * (int(value * totalInt) / totalInt));
			else thumbPos = (double(background.getWidth() - thumb.getWidth()) * value);
		//
		tempFill.clear(Rectangle<int>(0, 0, tempFill.getWidth(), tempFill.getHeight()), Colours::transparentBlack);
		Graphics gr(tempFill);
		gr.drawImageAt(filled, 0, 0);
		//
		if (bipolar)
		{
			if (value > 0.5)
			{
				tempFill.clear(Rectangle<int>(0, 0, filled.getWidth() / 2, tempFill.getHeight()), Colours::transparentBlack);
				tempFill.clear(Rectangle<int>(thumbPos + 8, 0, filled.getWidth(), tempFill.getHeight()), Colours::transparentBlack);
			}
			else if (value < 0.5)
			{
				tempFill.clear(Rectangle<int>(filled.getWidth() / 2, 0, filled.getWidth() / 2, tempFill.getHeight()), Colours::transparentBlack);
				tempFill.clear(Rectangle<int>(0, 0, thumbPos + 8, tempFill.getHeight()), Colours::transparentBlack);
			}
			else
			{
				tempFill.clear(Rectangle<int>(0, 0, tempFill.getWidth(), tempFill.getHeight()), Colours::transparentBlack);
			}
		}
		else
		{
			tempFill.clear(Rectangle<int>(thumbPos + 8, 0, background.getWidth() - thumbPos, tempFill.getHeight()), Colours::transparentBlack);
		}
		//
		gg.drawImageAt(tempFill, 0, 0);
		gg.drawImageAt(thumb, thumbPos, 0);
		//
		g.setImageResamplingQuality(Graphics::highResamplingQuality);
		g.drawImage(originalSizeImage, double(getWidth()) * 0.4, 0, double(getWidth()) * 0.6, getHeight(), 0, 0, originalSizeImage.getWidth(), originalSizeImage.getHeight());
		//
		String xText;
		if (integer) xText = String(int(value * totalInt) + min); 
		else if (bipolar) xText = String((value * 2.0f) - 1.0f, 3);
		else xText = String(value, 3);
		//
		g.setColour(Colours::black.withAlpha(0.06f));
		g.fillRect(Rectangle<int>(0, 0, double(getWidth()) * 0.3, getHeight()));
		//
		g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.4));
		g.setColour(Colours::white.withAlpha(0.82f));
		g.drawFittedText(xText, Rectangle<int>(0, 0, double(getWidth()) * 0.3, getHeight()), Justification::centred, 1);
	}
	//
	WMidiKeyboardComponent* midiKeyboard = nullptr;
	int* midiKeyboardValue = nullptr;
	Image originalSizeImage;
	Image tempFill;
	Image& background;
	Image& filled;
	Image& thumb;
	float& value;
	float startDragValue = 1.0f;
	bool wasOntopOfValue = false;
	bool bipolar = false;
	bool integer = false;
	int min = 0;
	int max = 1;
	float totalInt = 1.0f;
};