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
class WSPXKeyVelZoneStatusBar : public Component
{
public:
	void paint(Graphics& g) override
	{

	}
	//
	String topText;
	String bottomText;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXKeyVelZone : public Component
{
public:
	WSPXKeyVelZone(WSPX_Collection_Sound_File* _sound, MidiKeyboardComponent& _midiKeyboard, Component& _statusBar) 
		: sound(_sound), midiKeyboard(_midiKeyboard), statusBar(_statusBar) { }
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
		midiKeyboard.setAvailableRange(int(sound->keyZoneLow * 127.0f), int(sound->keyZoneHigh * 127.0f));
		midiKeyboard.setBounds(getBounds().getX(), midiKeyboard.getBounds().getY(), getWidth(), midiKeyboard.getHeight());
		repaint();
	}
	//
	void mouseDown(const MouseEvent& event) override
	{
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
		statusBar.setVisible(false);
		midiKeyboard.setAvailableRange(0, 127);
		midiKeyboard.setBounds(positionOnUI.getX(), midiKeyboard.getBounds().getY(), positionOnUI.getWidth(), midiKeyboard.getHeight());
		repaint();
	}
	//
	void mouseEnter(const MouseEvent& event) override
	{
		statusBar.setVisible(true);
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
		midiKeyboard.setAvailableRange(int(sound->keyZoneLow * 127.0f), int(sound->keyZoneHigh * 127.0f));
		midiKeyboard.setBounds(getBounds().getX(), midiKeyboard.getBounds().getY(), getWidth(), midiKeyboard.getHeight());
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
		setBounds(	positionOnUI.getX() + xPos - xExtraX, 
					positionOnUI.getY() + yPos - xExtraY,
					ww + (xExtraX * 2), 
					hh + (xExtraY * 2));
		//
		statusBar.setBounds(getBounds().getX(), getBounds().getY(), 200, 200);
	}
	//
	WSPX_Collection_Sound_File* sound;
	MidiKeyboardComponent& midiKeyboard;
	Rectangle<int> positionOnUI;
	Component& statusBar;
	//
	float keyZoneLow = 0.0;
	float keyZoneHigh = 0.0f;
	float velZoneLow = 0.0;
	float velZoneHigh = 0.0f;
	int type = kMove;
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