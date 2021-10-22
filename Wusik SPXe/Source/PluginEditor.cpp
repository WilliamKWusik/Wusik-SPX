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
	sliderBackground = ImageCache::getFromMemory(BinaryData::SliderBack_png, BinaryData::SliderBack_pngSize);
	sliderFilled = ImageCache::getFromMemory(BinaryData::SliderFilled_png, BinaryData::SliderFilled_pngSize);
	sliderThumb = ImageCache::getFromMemory(BinaryData::SliderThumb_png, BinaryData::SliderThumb_pngSize);
	//
	multRatio = double(getHeight()) / double(backgroundImage.getHeight());
	//
	originalBackgroundImage = Image(Image::ARGB, backgroundImage.getWidth(), backgroundImage.getHeight(), false);
	Graphics gg(originalBackgroundImage);
	gg.drawImageAt(backgroundImage, 0, 0);
	//
	uiRatio = double(backgroundImage.getWidth()) / double(backgroundImage.getHeight());
	//
	newLookAndFeel = new LookAndFeelEx();
	setLookAndFeel(newLookAndFeel);
	LookAndFeel::setDefaultLookAndFeel(newLookAndFeel);
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
	LookAndFeel::setDefaultLookAndFeel(nullptr);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::paint (Graphics& g)
{
	if (processor.collection != nullptr)
	{
		if (prevHasUnsavedChanges != processor.collection->hasUnsavedChanges)
		{
			prevHasUnsavedChanges = processor.collection->hasUnsavedChanges;
			Graphics gg(originalBackgroundImage);
			//
			if (processor.collection->hasUnsavedChanges)
			{
				gg.drawImageAt(redSaveImage, backgroundImage.getWidth() - redSaveImage.getWidth(), 0);
			}
			else
			{
				gg.drawImageAt(backgroundImage, 0, 0);
			}
		}
	}
	//
	g.drawImage(originalBackgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, originalBackgroundImage.getWidth(), originalBackgroundImage.getHeight());
	//
	if (processor.collection != nullptr && editObject.type == WusikEditObject::kCollection)
	{
		if (processor.collection->imageIcon.image.getWidth() > 0)
		{
			g.setColour(Colours::darkblue.withAlpha(0.88f));
			g.fillRect(Rectangle<int>((1282.0 - 4.0) * multRatio, (108.0 - 4.0) * multRatio, (314.0 + 8.0) * multRatio, (314.0 + 8.0) * multRatio));
			//
			g.drawImage(processor.collection->imageIcon.image, 1282.0 * multRatio, 108.0 * multRatio, 314.0 * multRatio, 314.0 * multRatio, 0, 0,
				processor.collection->imageIcon.image.getWidth(), processor.collection->imageIcon.image.getHeight());
		}
		//
		if (processor.collection->imageAbout.image.getWidth() > 0)
		{
			g.setColour(Colours::darkblue.withAlpha(0.88f));
			g.fillRect(Rectangle<int>((20.0 - 4.0) * multRatio, (108.0 - 4.0) * multRatio, (528.0 + 8.0) * multRatio, (528.0 + 8.0) * multRatio));
			//
			g.drawImage(processor.collection->imageAbout.image, 20.0 * multRatio, 108.0 * multRatio, 528.0 * multRatio, 528.0 * multRatio, 0, 0,
				processor.collection->imageAbout.image.getWidth(), processor.collection->imageAbout.image.getHeight());
		}
	}
	//
	if (processor.collection != nullptr && editObject.type == WusikEditObject::kPreset)
	{
		Image* presetImage = &((WSPX_Collection_Preset*)editObject.object)->imagePresetIcon.image;
		if (presetImage->getWidth() > 0)
		{
			g.setColour(Colours::darkblue.withAlpha(0.88f));
			g.fillRect(Rectangle<int>((1282.0 - 4.0) * multRatio, (108.0 - 4.0) * multRatio, (314.0 + 8.0) * multRatio, (314.0 + 8.0) * multRatio));
			g.drawImage(*presetImage, 1282.0 * multRatio, 108.0 * multRatio, 314.0 * multRatio, 314.0 * multRatio, 0, 0, presetImage->getWidth(), presetImage->getHeight());
		}
	}
}