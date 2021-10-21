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
class WusikSpxAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener
{
public:
    WusikSpxAudioProcessorEditor (WusikSpxAudioProcessor&);
    ~WusikSpxAudioProcessorEditor();
	void buttonClicked(Button* buttonThatWasClicked);
    void paint (Graphics&) override;
    void resized() override;
	void showAbout()
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Wusik WSPX Editor", "Created by William Kalfelz @ Wusik.com (c) 2021\n\nVersion " + String(WVERSION));
	};
	//
	void boundsSet(int xx, int yy, int ww, int hh, Component* compo, double multRatio)
	{
		compo->setBounds(double(xx) * multRatio, double(yy) * multRatio, double(ww) * multRatio, double(hh) * multRatio);
	};
	//
	void updateInterface();
	void cleanInterface();
	//
	Image backgroundImage;
	Image redSaveImage; // UnSaved Changes //
	Image originalBackgroundImage;
	double uiRatio = 1.0;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizerConstrainer;
	ScopedPointer<LookAndFeelEx> newLookAndFeel;
	Label* collectionNameLabel;
	Label* statusBar;
	ScopedPointer<WusikTreeHolder> presetsTreeView;
	ScopedPointer<WusikTreeHolder> soundsTreeView;
	OwnedArray<WSPXSoundZone> soundZones;
	WusikEditObject editObject;
	OwnedArray<WusikEditOption> editOptions;
	Component* editOptionsComponent;
	ScopedPointer<Viewport> editOptionsViewport;
	bool prevHasUnsavedChanges = false;
	double multRatio = 1.0;
	//
	WTransparentButton* logoButton;
	WTransparentButton* fileButton;
	WTransparentButton* saveButton;
	WTransparentButton* collectionButton;
	WTransparentButton* previewButton;
	//
private:
    WusikSpxAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessorEditor)
};
