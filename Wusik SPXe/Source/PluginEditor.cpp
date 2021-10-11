/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikSpxAudioProcessorEditor::WusikSpxAudioProcessorEditor (WusikSpxAudioProcessor& p) : AudioProcessorEditor (&p), processor (p)
{
	background = ImageCache::getFromMemory(BinaryData::Interface_png, BinaryData::Interface_pngSize);
	uiRatio = double(background.getWidth()) / double(background.getHeight());
	//
	newLookAndFeel = new LookAndFeelEx();
	setLookAndFeel(newLookAndFeel);
	//
	addAndMakeVisible(collectionNameLabel = new Label());
	collectionNameLabel->setColour(Label::ColourIds::textColourId, Colours::white.withAlpha(0.66f));
	collectionNameLabel->setText(processor.collection.name, NotificationType::dontSendNotification);
	collectionNameLabel->setJustificationType(Justification::centred);
	//
	resizerConstrainer.setFixedAspectRatio(uiRatio);
	addAndMakeVisible(resizer = new ResizableCornerComponent(this, &resizerConstrainer));
	//
	double defaultH = 900.0;
    setSize (int(defaultH * uiRatio), int(defaultH));
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikSpxAudioProcessorEditor::~WusikSpxAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::paint (Graphics& g)
{
	g.drawImage(background, 0, 0, getWidth(), getHeight(), 0, 0, background.getWidth(), background.getHeight());
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::resized()
{
	double multRatio = double(getHeight()) / double(background.getHeight());
	//
	collectionNameLabel->setBounds(682.0 * multRatio, 12.0 * multRatio, 452.0 * multRatio, 51.0 * multRatio);
	collectionNameLabel->setFont(LookAndFeelEx::getCustomFont().withHeight(double(collectionNameLabel->getHeight()) * 0.52));
	//
	resizer->setBounds(getWidth() - 32, getHeight() - 32, 32, 32);
	resizer->toFront(false);
}
