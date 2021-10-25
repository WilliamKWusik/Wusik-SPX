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
	if (editObject.type != WusikEditObject::kNone)
	{
		int totalH = 0;
		//
		for (int bb = 0; bb < editOptions.size(); bb++)
		{
			boundsSet(0, totalH, 660, 52, editOptions[bb], multRatio);
			totalH += 56;
		}
		//
		boundsSet(569, 107, 690, 807, editOptionsViewport, multRatio);
		editOptionsComponent->setBounds(0, 0, editOptionsViewport->getWidth(), double(totalH) * multRatio);
		//
		if (editObject.type == WusikEditObject::kSoundFile)
		{
			if (soundFileWaveformThumb.getWidth() > 0)
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