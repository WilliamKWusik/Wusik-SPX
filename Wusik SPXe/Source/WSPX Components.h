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
class WSPXSoundFileThumbThreadCreation : public ThreadWithProgressWindow
{
public:
	WSPXSoundFileThumbThreadCreation(void* _editor, File _fileToProcess, File _thumbFile) :
		ThreadWithProgressWindow("Sound Thumb Processing", true, false, 0, String(), (Component*) _editor), 
		fileToProcess(_fileToProcess), editor(_editor), thumbFile(_thumbFile) { }
	//
	void run() override;
	File fileToProcess, thumbFile;
	void* editor;
};
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
class WSPXStatusLabel : public Component
{
public:
	void paint(Graphics& g) override
	{
		g.setColour(Colours::black.withAlpha(0.66f));
		g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4.0f);
		g.setColour(Colours::white.withAlpha(0.66f));
		g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 4.0f, 2.0f);
		//
		g.setColour(Colours::white.withAlpha(0.92f));
		g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.42f));
		g.drawFittedText(text, 6, 6, getWidth() - 12, getHeight() - 12, Justification::centred, 99);
	}
	//
	String text;
};
//
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXKeyVelZone : public Component
{
public:
	WSPXKeyVelZone(void* _theEditor, WSPX_Collection_Sound_File* _sound) : sound(_sound), theEditor(_theEditor) { }
	//
	void updateKeysAndLabel(bool mouseAway = false);
	void mouseUp(const MouseEvent& event) override;
	//
	void paint(Graphics& g) override
	{
		g.fillAll(Colour::fromString("BB94B7C4").darker(0.6f));
		//
		if (isMouseOverOrDragging())
		{
			g.fillAll(Colours::red.withAlpha(0.12f));
			g.setColour(Colours::red.withAlpha(0.66f));
		}
		else g.setColour(Colours::white.withAlpha(0.66f));
		//
		g.drawRect(0, 0, getWidth(), getHeight(), 2);
	}
	//
	void mouseDrag(const MouseEvent& event) override
	{
		hasMoved = true;
		float multiply = 1.0f;
		if (event.mods.isShiftDown() || event.mods.isRightButtonDown()) multiply = 0.1f;
		//
		float moveX = event.getDistanceFromDragStartX() * 0.001f * multiply;
		float moveY = event.getDistanceFromDragStartY() * 0.001f * multiply;
		//
		if (type == kMove)
		{
			sound->keyZoneLow = jlimit(0.0f, 1.0f, keyZoneLow + moveX);
			sound->keyZoneHigh = jlimit(0.0f, 1.0f, keyZoneHigh + moveX);
			//
			if (!event.mods.isMiddleButtonDown())
			{
				sound->velZoneLow = jlimit(0.0f, 1.0f, velZoneLow - moveY);
				sound->velZoneHigh = jlimit(0.0f, 1.0f, velZoneHigh - moveY);
			}
		}
		else if (type == kLeft)
		{
			sound->keyZoneLow = jlimit(0.0f, 1.0f, keyZoneLow + moveX);
		}
		else if (type == kRight)
		{
			sound->keyZoneHigh = jlimit(0.0f, 1.0f, keyZoneHigh + moveX);
		}
		else if (type == kTop)
		{
			sound->velZoneHigh = jlimit(0.0f, 1.0f, velZoneHigh - moveY);
		}
		else if (type == kBottom)
		{
			sound->velZoneLow = jlimit(0.0f, 1.0f, velZoneLow - moveY);
		}
		//
		setPositionOnUI();
		updateKeysAndLabel();
		repaint();
	}
	//
	void mouseDown(const MouseEvent& event) override
	{
		hasMoved = false;
		keyZoneLow = sound->keyZoneLow;
		keyZoneHigh = sound->keyZoneHigh;
		velZoneLow = sound->velZoneLow;
		velZoneHigh = sound->velZoneHigh;
		//
		if (event.getPosition().x < 20) type = kLeft;
		else if (event.getPosition().y < 20) type = kTop;
		else if (event.getPosition().x > (getWidth() - 20)) type = kRight;
		else if (event.getPosition().y > (getHeight() - 20)) type = kBottom;
		else type = kMove;
	}
	//
	void mouseExit(const MouseEvent& event) override
	{
		setMouseCursor(MouseCursor::NormalCursor);
		updateKeysAndLabel(true);
		repaint();
	}
	//
	void mouseEnter(const MouseEvent& event) override
	{
		updateKeysAndLabel();
		repaint();
	}
	//
	void mouseMove(const MouseEvent& event) override
	{
		if (event.getPosition().x < 20) setMouseCursor(MouseCursor::LeftEdgeResizeCursor);
		else if (event.getPosition().y < 20) setMouseCursor(MouseCursor::TopEdgeResizeCursor);
		else if (event.getPosition().x > (getWidth() - 20)) setMouseCursor(MouseCursor::RightEdgeResizeCursor);
		else if (event.getPosition().y > (getHeight() - 20)) setMouseCursor(MouseCursor::BottomEdgeResizeCursor);
		else setMouseCursor(MouseCursor::UpDownLeftRightResizeCursor);
		//
		updateKeysAndLabel();
		repaint();
	}
	//
	void setPositionOnUI()
	{
		int xExtraX = int((double(positionOnUI.getWidth()) / 127.0) / 2.0);
		int xExtraY = int((double(positionOnUI.getHeight()) / 127.0) / 2.0);
		//
		double keyLow = double(int(sound->keyZoneLow * 127.0f)) / 127.0;
		double keyHigh = double(int(sound->keyZoneHigh * 127.0f)) / 127.0;
		double velLow = double(int(sound->velZoneLow * 127.0f)) / 127.0;
		double velHigh = double(int(sound->velZoneHigh * 127.0f)) / 127.0;
		//
		double xPos = keyLow * double(positionOnUI.getWidth());
		double yPos = (double(positionOnUI.getHeight()) - (velHigh * double(positionOnUI.getHeight())));
		double ww = (keyHigh * double(positionOnUI.getWidth())) - xPos;
		double hh = (double(positionOnUI.getHeight()) - (velLow * double(positionOnUI.getHeight()))) - yPos;
		//
		setBounds(positionOnUI.getX() + xPos - xExtraX, positionOnUI.getY() + yPos - xExtraY, ww + (xExtraX * 2), hh + (xExtraY * 2));
	}
	//
	void* theEditor;
	WSPX_Collection_Sound_File* sound;
	Rectangle<int> positionOnUI;
	//
	float keyZoneLow = 0.0;
	float keyZoneHigh = 0.0f;
	float velZoneLow = 0.0;
	float velZoneHigh = 0.0f;
	int type = kMove;
	bool hasMoved = false;
	//
	enum
	{
		kMove = 0,
		kLeft,
		kRight,
		kTop,
		kBottom,
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WTransparentButton : public TextButton
{
public:
	WTransparentButton(Button::Listener* _owner) : TextButton(""), owner(_owner) { addListener(owner); }
	~WTransparentButton() { removeListener(owner); }
	void paint(Graphics& g) override { }
	Button::Listener* owner;
};