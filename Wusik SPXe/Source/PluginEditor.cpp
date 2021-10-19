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
	backgroundImage = ImageCache::getFromMemory(BinaryData::Interface_png, BinaryData::Interface_pngSize);
	redSaveImage = ImageCache::getFromMemory(BinaryData::Interface3_png, BinaryData::Interface3_pngSize);
	//
	double multRatio = double(getHeight()) / double(backgroundImage.getHeight());
	//
	tempImage = Image(Image::ARGB, backgroundImage.getWidth(), backgroundImage.getHeight(), false);
	Graphics gg(tempImage);
	gg.drawImageAt(backgroundImage, 0, 0);
	//
	uiRatio = double(backgroundImage.getWidth()) / double(backgroundImage.getHeight());
	//
	newLookAndFeel = new LookAndFeelEx();
	setLookAndFeel(newLookAndFeel);
	//
	addAndMakeVisible(collectionNameLabel = new Label());
	collectionNameLabel->setColour(Label::ColourIds::textColourId, Colours::white.withAlpha(0.66f));
	collectionNameLabel->setText(processor.collection->name, NotificationType::dontSendNotification);
	collectionNameLabel->setJustificationType(Justification::centred);
	//
	addAndMakeVisible(statusBar = new Label());
	statusBar->setColour(Label::ColourIds::textColourId, Colours::white.withAlpha(0.66f));
	//
	if (processor.collection->file.existsAsFile())
		statusBar->setText(processor.collection->file.getFullPathName(), NotificationType::dontSendNotification);
	else
		statusBar->setText("Unsaved Collection", NotificationType::dontSendNotification);
	//
	statusBar->setJustificationType(Justification::centred);
	//
	addAndMakeVisible(logoButton = new WTransparentButton(this));
	addAndMakeVisible(fileButton = new WTransparentButton(this));
	addAndMakeVisible(saveButton = new WTransparentButton(this));
	addAndMakeVisible(collectionButton = new WTransparentButton(this));
	addAndMakeVisible(previewButton = new WTransparentButton(this));
	//
	addAndMakeVisible(presetsTreeView = new WusikTreeHolder(processor, true, multRatio));
	addAndMakeVisible(soundsTreeView = new WusikTreeHolder(processor, false, multRatio));
	//
	resizerConstrainer.setFixedAspectRatio(uiRatio);
	addAndMakeVisible(resizer = new ResizableCornerComponent(this, &resizerConstrainer));
	//
	updateInterface();
	//
	double defaultH = 900.0;
    setSize (int(defaultH * uiRatio), int(defaultH));
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
WusikSpxAudioProcessorEditor::~WusikSpxAudioProcessorEditor()
{
	cleanInterface();
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
		gg.drawImageAt(redSaveImage, backgroundImage.getWidth() - redSaveImage.getWidth(), 0);
	}
	//
	g.drawImage(tempImage, 0, 0, getWidth(), getHeight(), 0, 0, tempImage.getWidth(), tempImage.getHeight());
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::resized()
{
	double multRatio = double(getHeight()) / double(backgroundImage.getHeight());
	//
	// Status Bar //
	boundsSet(0, 958, 1624, 38, statusBar, multRatio);
	statusBar->setFont(LookAndFeelEx::getCustomFont().withHeight(double(statusBar->getHeight()) * 0.48));
	//
	// Collection Name //
	boundsSet(682, 12, 452, 51, collectionNameLabel, multRatio);
	collectionNameLabel->setFont(LookAndFeelEx::getCustomFont().withHeight(double(collectionNameLabel->getHeight()) * 0.52));
	//
	// Buttons //
	boundsSet(0, 0, 197, 65, logoButton, multRatio);
	boundsSet(550, 11, 82, 49, fileButton, multRatio);
	boundsSet(1185, 12, 84, 50, saveButton, multRatio);
	boundsSet(682, 12, 452, 51, collectionButton, multRatio);
	boundsSet(1507, 11, 96, 50, previewButton, multRatio);
	//
	// Presets TreeView //
	removeChildComponent(presetsTreeView);
	addAndMakeVisible(presetsTreeView = new WusikTreeHolder(processor, true, multRatio));
	boundsSet(1638, 50, 248, 384, presetsTreeView, multRatio);
	//
	// Sounds TreeView //
	removeChildComponent(soundsTreeView);
	addAndMakeVisible(soundsTreeView = new WusikTreeHolder(processor, false, multRatio));
	boundsSet(1638, 508, 248, 474, soundsTreeView, multRatio);
	//
	// Show Edit Objects //
	if (editObject.type == WusikEditObject::kCollection)
	{
		int totalH = 0;
		//
		for (int bb = 0; bb < editOptions.size(); bb++)
		{
			boundsSet(0, totalH, 685, 52, editOptions[bb], multRatio);
			totalH += 56;
		}
		//
		boundsSet(569, 107, 685, 807, editOptionsViewport, multRatio);
		editOptionsComponent->setBounds(0, 0, 685, totalH);
	}
	// Sound Zones //
	{
		for (int zz = 0; zz < soundZones.size(); zz++)
		{
			int xPos = int(double(soundZones[zz]->sound->keyZoneLow) * ((1589.0 * multRatio) / 127.0));
			int yPos = 0;
			int ww = int((double(soundZones[zz]->sound->keyZoneHigh + 1) * ((1589.0 * multRatio) / 127.0)) - xPos);
			int hh = int(856.0 * multRatio);
			//
			soundZones[zz]->setBounds(xPos + int(13.0 * multRatio), yPos + int(84.0 * multRatio), ww, hh);
		}
	}
	//
	resizer->setBounds(getWidth() - 32, getHeight() - 32, 32, 32);
	resizer->toFront(false);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
	WAlert;
}