/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "WSPX Components.h"
#include "WSPX TreeViews.h"
#include "WSPX Object Editor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WSPXSoundFileThumbView
{
public:
	void reset()
	{
		ready.set(0);
		loopStart = loopEnd = 0;
		waveform = Image();
	}
	//
	Atomic<int> ready = 0;
	Image waveform;
	int loopStart = 0;
	int loopEnd = 0;
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikSpxAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener, public Timer
{
public:
    WusikSpxAudioProcessorEditor (WusikSpxAudioProcessor&);
    ~WusikSpxAudioProcessorEditor();
	void buttonClicked(Button* buttonThatWasClicked);
    void paint (Graphics&) override;
    void resized() override;
	void loadSoundFileThumb(WSPX_Collection_Sound_File* soundFile);
	void presetChanged() { processor.collection->hasUnsavedChanges = true; }
	void showAbout()
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Wusik WSPX Editor", "Created by William Kalfelz @ Wusik.com (c) 2021\n\nVersion " + String(WVERSION));
	}
	//
	void boundsSet(int xx, int yy, int ww, int hh, Component* compo, double multRatio)
	{
		compo->setBounds(double(xx) * multRatio, double(yy) * multRatio, double(ww) * multRatio, double(hh) * multRatio);
	}
	//
	void createAction(int _type, int _value1 = 0, int _value2 = 0, int _value3 = 0, int _value4 = 0, void* _object = nullptr)
	{
		stopTimer();
		timerActionValue1 = _value1;
		timerActionValue2 = _value2;
		timerActionValue3 = _value3;
		timerActionValue4 = _value4;
		timerActionValueObject = _object;
		timerAction.set(_type);
		startTimer(200);
	}
	//
	void updateInterface();
	void cleanInterface();
	void timerCallback() override;
	//
	Image backgroundImage;
	Image redSaveImage; // UnSaved Changes //
	Image originalBackgroundImage;
	Image sliderBackground, sliderFilled, sliderThumb;
	double uiRatio = 1.0;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizerConstrainer;
	ScopedPointer<LookAndFeelEx> newLookAndFeel;
	Label* collectionNameLabel;
	ScopedPointer<WusikTreeHolder> presetsTreeView;
	ScopedPointer<WusikTreeHolder> soundsTreeView;
	OwnedArray<WSPXKeyVelZone> keyVelZones;
	WusikEditObject editObject;
	OwnedArray<WusikEditOption> editOptions;
	Component* editOptionsComponent;
	ScopedPointer<Viewport> editOptionsViewport;
	bool prevHasUnsavedChanges = false;
	double multRatio = 1.0;
	bool redoTreeViewsOnResize = true;
	WSPXSoundFileThumbView soundFileThumbView;
	MidiKeyboardComponent midiKeyboard;
	MidiKeyboardComponent midiKeyboardBottom;
	WSPXKeyVelZoneStatusBar* keyVelZoneStatusBar;
	//
	Atomic<int> timerAction = 0;
	int timerActionValue1 = 0;
	int timerActionValue2 = 0;
	int timerActionValue3 = 0;
	int timerActionValue4 = 0;
	void* timerActionValueObject = nullptr;
	//
	WTransparentButton* logoButton;
	WTransparentButton* fileButton;
	WTransparentButton* saveButton;
	WTransparentButton* collectionButton;
	WTransparentButton* previewButton;
	//
	enum
	{
		kTimerAction_None,
		kTimerAction_Remove_Preset,
		kTimerAction_Remove_Layer,
		kTimerAction_Remove_Sound,
		kTimerAction_Remove_Sound_Link,
		kTimerAction_Remove_Sound_File,
		kTimerAction_Update_Interface,
		kTimerAction_Update_Interface_Not_TreeViews,
		kTimerAction_Update_Interface_Show_Collection
	};
	//
private:
    WusikSpxAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessorEditor)
};
