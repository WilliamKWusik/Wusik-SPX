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
	background2 = ImageCache::getFromMemory(BinaryData::Interface2_png, BinaryData::Interface2_pngSize);
	background3 = ImageCache::getFromMemory(BinaryData::Interface3_png, BinaryData::Interface3_pngSize);
	//
	double multRatio = double(getHeight()) / double(background.getHeight());
	//
	tempImage = Image(Image::ARGB, background.getWidth(), background.getHeight(), false);
	Graphics gg(tempImage);
	gg.drawImageAt(background, 0, 0);
	//
	uiRatio = double(background.getWidth()) / double(background.getHeight());
	//
	newLookAndFeel = new LookAndFeelEx();
	setLookAndFeel(newLookAndFeel);
	//
	addAndMakeVisible(collectionNameLabel = new Label());
	collectionNameLabel->setColour(Label::ColourIds::textColourId, Colours::white.withAlpha(0.66f));
	collectionNameLabel->setText(processor.collection->name, NotificationType::dontSendNotification);
	collectionNameLabel->setJustificationType(Justification::centred);
	//
	addAndMakeVisible(logoButton = new WTransparentButton(this));
	addAndMakeVisible(fileButton = new WTransparentButton(this));
	addAndMakeVisible(saveButton = new WTransparentButton(this));
	addAndMakeVisible(collectionButton = new WTransparentButton(this));
	//
	addAndMakeVisible(presetsTreeView = new WusikTreeHolder(processor, true, multRatio));
	addAndMakeVisible(soundsTreeView = new WusikTreeHolder(processor, false, multRatio));
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
	Graphics gg(tempImage);
	//
	if (processor.collection != nullptr && processor.collection->hasUnsavedChanges)
	{
		gg.drawImageAt(background3, background.getWidth() - background3.getWidth(), 0);
	}
	//
	if (showingOptions)
	{
		gg.drawImageAt(background2, background.getWidth() - background2.getWidth(), background.getHeight() - background2.getHeight());
	}
	//
	g.drawImage(tempImage, 0, 0, getWidth(), getHeight(), 0, 0, tempImage.getWidth(), tempImage.getHeight());
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::resized()
{
	double multRatio = double(getHeight()) / double(background.getHeight());
	//
	boundsSet(682, 12, 452, 51, collectionNameLabel, multRatio);
	collectionNameLabel->setFont(LookAndFeelEx::getCustomFont().withHeight(double(collectionNameLabel->getHeight()) * 0.52));
	//
	boundsSet(0, 0, 197, 65, logoButton, multRatio);
	boundsSet(550, 11, 82, 49, fileButton, multRatio);
	boundsSet(1185, 12, 84, 50, saveButton, multRatio);
	boundsSet(682, 12, 452, 51, collectionButton, multRatio);
	//
	removeChildComponent(presetsTreeView);
	addAndMakeVisible(presetsTreeView = new WusikTreeHolder(processor, true, multRatio));
	boundsSet(1638, 50, 248, 384, presetsTreeView, multRatio);
	//
	removeChildComponent(soundsTreeView);
	addAndMakeVisible(soundsTreeView = new WusikTreeHolder(processor, false, multRatio));
	boundsSet(1638, 508, 248, 474, soundsTreeView, multRatio);
	//
	resizer->setBounds(getWidth() - 32, getHeight() - 32, 32, 32);
	resizer->toFront(false);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
	AlertWindow::showMessageBox(AlertWindow::NoIcon, "!", "!");
}