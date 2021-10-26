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
class WSPXKeyVelZone : public Component
{
public:
	WSPXKeyVelZone(WSPX_Collection_Sound_File* _sound, MidiKeyboardComponent& _midiKeyboard) : sound(_sound), midiKeyboard(_midiKeyboard) { }
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
	void mouseExit(const MouseEvent& event) override
	{
		setMouseCursor(MouseCursor::NormalCursor);
		repaint();
	}
	//
	void mouseEnter(const MouseEvent& event) override
	{
		repaint();
	}
	//
	void mouseMove(const MouseEvent& event) override
	{
		if (event.getPosition().x < 20)
		{
			setMouseCursor(MouseCursor::LeftEdgeResizeCursor);
		}
		else if (event.getPosition().y < 20)
		{
			setMouseCursor(MouseCursor::TopEdgeResizeCursor);
		}
		else if (event.getPosition().x > (getWidth() - 20))
		{
			setMouseCursor(MouseCursor::RightEdgeResizeCursor);
		}
		else if (event.getPosition().y > (getHeight() - 20))
		{
			setMouseCursor(MouseCursor::BottomEdgeResizeCursor);
		}
		else
		{
			setMouseCursor(MouseCursor::UpDownLeftRightResizeCursor);
		}
		//
		repaint();
	}
	//
	void setPositionOnUI()
	{
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
		setBounds(positionOnUI.getX() + xPos, positionOnUI.getY() + yPos, ww, hh);
	}
	//
	WSPX_Collection_Sound_File* sound;
	MidiKeyboardComponent& midiKeyboard;
	Rectangle<int> positionOnUI;
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