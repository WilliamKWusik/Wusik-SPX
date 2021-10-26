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
	if (keyVelZones.size() > 0)
	{
		Rectangle<int> positionOnUI(int(24.0 * multRatio), int(88.0 * multRatio), int(1582.0 * multRatio), int(798.0 * multRatio));
		//
		for (int zz = 0; zz < keyVelZones.size(); zz++)
		{
			/*double keyLow = double(int(keyVelZones[zz]->sound->keyZoneLow * 127.0f)) / 127.0;
			double keyHigh = double(int(keyVelZones[zz]->sound->keyZoneHigh * 127.0f)) / 127.0;
			double velLow = double(int(keyVelZones[zz]->sound->velZoneLow * 127.0f)) / 127.0;
			double velHigh = double(int(keyVelZones[zz]->sound->velZoneHigh * 127.0f)) / 127.0;
			//
			double xPos = keyLow * 1582.0;
			double yPos = (798.0 - (velHigh * 798.0));
			double ww = (keyHigh * 1582.0) - xPos;
			double hh = (798.0 - (velLow * 798.0)) - yPos;
			//
			boundsSet(24.0 + xPos - 5, 88.0 + yPos, ww + 10, hh, keyVelZones[zz], multRatio);*/
			//
			keyVelZones[zz]->positionOnUI = positionOnUI;
			keyVelZones[zz]->setPositionOnUI();
		}
		//
		boundsSet(24 - 5, 905, 1582 + 10, 93, &midiKeyboard, multRatio);
		midiKeyboard.setKeyWidth(double(midiKeyboard.getWidth()) / 76.0);
	}
	else
	{
		midiKeyboard.setKeyWidth(26.0 * multRatio);
		boundsSet(0, 905, 1622, 93, &midiKeyboard, multRatio);
	}
	//
	resizer->setBounds(getWidth() - 32, getHeight() - 32, 32, 32);
	resizer->toFront(false);
}