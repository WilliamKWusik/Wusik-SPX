/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
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
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikSpxAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    WusikSpxAudioProcessorEditor (WusikSpxAudioProcessor&);
    ~WusikSpxAudioProcessorEditor();
    void paint (Graphics&) override;
    void resized() override;
	//
	Image background;
	double uiRatio = 1.0;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizerConstrainer;
	ScopedPointer<LookAndFeelEx> newLookAndFeel;
	Label* collectionNameLabel;
	//
private:
    WusikSpxAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WusikSpxAudioProcessorEditor)
};
