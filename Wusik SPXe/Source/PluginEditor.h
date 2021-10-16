/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "WSPX Components.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditObject
{
public:
	void set(char _type, void* _object) { type = _type; object = _object; };
	char type = 0;
	void* object = nullptr;
	//
	enum
	{
		kNone,
		kPreset,
		kPresetLayer,
		kSoundGroup,
		kSound
	};
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
	void updateInterface();
	void cleanInterface();
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
	Label* statusBar;
	bool showingOptions = false;
	WusikTreeHolder* presetsTreeView;
	WusikTreeHolder* soundsTreeView;
	OwnedArray<WSPXSoundZone> soundZones;
	WusikEditObject editObject;
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
