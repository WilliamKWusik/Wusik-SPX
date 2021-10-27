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
		float moveX = event.getDistanceFromDragStartX() * 0.8f * multiply;
		float moveY = event.getDistanceFromDragStartY() * 0.8f * multiply;
		//
		if (type == kMove)
		{
			sound->keyZoneLow = jlimit(0, 127, int(keyZoneLow + moveX));
			sound->keyZoneHigh = jlimit(0, 127, int(keyZoneLow + moveX));
			//
			if (!event.mods.isMiddleButtonDown())
			{
				sound->velZoneLow = jlimit(0, 127, int(velZoneLow - moveY));
				sound->velZoneHigh = jlimit(0, 127, int(velZoneHigh - moveY));
			}
		}
		else if (type == kLeft)
		{
			sound->keyZoneLow = jlimit(0, 127, int(keyZoneLow + moveX));
		}
		else if (type == kRight)
		{
			sound->keyZoneHigh = jlimit(0, 127, int(keyZoneLow + moveX));
		}
		else if (type == kTop)
		{
			sound->velZoneHigh = jlimit(0, 127, int(velZoneHigh - moveY));
		}
		else if (type == kBottom)
		{
			sound->velZoneLow = jlimit(0, 127, int(velZoneLow - moveY));
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
		double keyLow = double(sound->keyZoneLow);
		double keyHigh = double(sound->keyZoneHigh);
		double velLow = double(sound->velZoneLow);
		double velHigh = double(sound->velZoneHigh);
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
	int keyZoneLow = 0;
	int keyZoneHigh = 0;
	int velZoneLow = 0;
	int velZoneHigh = 0;
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