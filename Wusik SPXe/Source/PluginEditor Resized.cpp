/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::resized()
{
	multRatio = double(getHeight()) / double(backgroundImage.getHeight());
	newLookAndFeel->ratioValue = multRatio;
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
	boundsSet(1455, 11, 52, 50, uiButton, multRatio);
	//
	if (redoTreeViewsOnResize)
	{
		// Presets TreeView //
		addAndMakeVisible(presetsTreeView = new WusikTreeHolder(processor, true, multRatio));
		boundsSet(1638, 50, 248, 384, presetsTreeView, multRatio);
		//
		// Sounds TreeView //
		addAndMakeVisible(soundsTreeView = new WusikTreeHolder(processor, false, multRatio));
		boundsSet(1638, 508, 248, 474, soundsTreeView, multRatio);
	}
	//
	redoTreeViewsOnResize = true;
	//
	// Show Edit Objects //
	if (editObject.type != WusikEditObject::kNone && editOptionsComponent != nullptr)
	{
		int totalH = 0;
		//
		for (int bb = 0; bb < editOptions.size(); bb++)
		{
			boundsSet(0, totalH, 660, 52, editOptions[bb], multRatio);
			totalH += 56;
		}
		//
		boundsSet(569, 90, 690, 794, editOptionsViewport, multRatio);
		editOptionsComponent->setBounds(0, 0, editOptionsViewport->getWidth(), double(totalH) * multRatio);
		//
		if (editObject.type == WusikEditObject::kSoundFile)
		{
			if (soundFileThumbView.ready.get() == 1 && soundFileThumbView.waveform.getWidth() > 0)
			{
				editOptionsViewport->setBounds(
					editOptionsViewport->getBounds().getX(),
					editOptionsViewport->getBounds().getY() + (270 * multRatio) + (24.0 * multRatio),
					editOptionsViewport->getBounds().getWidth(),
					editOptionsViewport->getBounds().getHeight() - (270 * multRatio) - (24.0 * multRatio));
			}
		}
	}
	// Sound Zones //
	boundsSet(20, 905, 1592, 93, &midiKeyboard, multRatio);
	midiKeyboard.setAvailableRange(0, 127);
	midiKeyboard.setKeyWidth(double(midiKeyboard.getWidth()) / 76.0);
	statusLabel->setVisible(false);
	//
	if (keyVelZones.size() > 0)
	{
		Rectangle<int> positionOnUI(midiKeyboard.getBounds().getX() + int(8.0 * multRatio), int(88.0 * multRatio), midiKeyboard.getWidth() - int(32.0 * multRatio),int(798.0 * multRatio));
		//
		for (int zz = 0; zz < keyVelZones.size(); zz++)
		{
			keyVelZones[zz]->positionOnUI = positionOnUI;
			keyVelZones[zz]->setPositionOnUI();
		}
	}
	//
	resizer->setBounds(getWidth() - 32, getHeight() - 32, 32, 32);
	resizer->toFront(false);
}