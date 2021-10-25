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
class WSPXSoundFileThumb : public ThreadWithProgressWindow
{
public:
	WSPXSoundFileThumb(void* _editor, File _fileToProcess, File _thumbFile) :
		ThreadWithProgressWindow("Sound Thumb Processing", true, false, 0, String(), (Component*) _editor), 
		fileToProcess(_fileToProcess), editor(_editor), thumbFile(_thumbFile) { }
	//
	void run() override;
	File fileToProcess, thumbFile;
	void* editor;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXSoundZone : public Component
{
public:
	WSPXSoundZone(WSPX_Collection_Sound_File* _sound) : sound(_sound) { }
	//
	void paint(Graphics& g) override
	{
		g.fillAll(Colours::yellow.withAlpha(0.22f));
		g.setColour(Colours::white.withAlpha(0.66f));
		g.drawRect(0, 0, getWidth(), getHeight(), 2);
	}
	//
	WSPX_Collection_Sound_File* sound;
};
//
class WTransparentButton : public TextButton
{
public:
	WTransparentButton(Button::Listener* _owner) : TextButton(""), owner(_owner) { addListener(owner); }
	~WTransparentButton() { removeListener(owner); }
	void paint(Graphics& g) override { }
	Button::Listener* owner;
};