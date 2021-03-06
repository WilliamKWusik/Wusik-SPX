/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
WSPXSoundTreeItem::WSPXSoundTreeItem(WusikSpxAudioProcessor& _processor, double _ui_ratio, int _level, String _name, 
	int _specialItem, WSPX_Collection_Sound* _sound, WSPX_Collection_Sound_File* _soundFile)
	: WSPXTreeItem(_processor, _level, _specialItem, _ui_ratio, _name), sound(_sound), soundFile(_soundFile)
{
	if (level == kLevel_Add_Sound)
	{
		for (int gg = 0; gg < processor.collection->sounds.size(); gg++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "", kRegular_Item, processor.collection->sounds[gg]));
		}
	}
	else if (level == kLevel_Sounds)
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "Zones", kSound_Zones, sound));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "Remove", kSound_Remove, sound));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "New File(s)", kSound_Add, sound));
		//
		for (int ss = 0; ss < sound->soundFiles.size(); ss++)
		{
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_Files, "", kRegular_Item, sound, sound->soundFiles[ss]));
		}
	}
	else if (level == kLevel_Sound_Files && name.isEmpty())
	{
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_File_Options, "Remove", kSound_File_Remove, sound, soundFile));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_File_Options, "Open", kSound_File_Open, sound, soundFile));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_File_Options, "Load", kSound_File_Load, sound, soundFile));
		addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sound_File_Options, "Load M.Channel", kSound_File_Multi_Channel_Load, sound, soundFile));
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXSoundTreeItem::paintItem(Graphics& g, int width, int height)
{
	if (isSelected()) g.fillAll(Colours::red.withAlpha(0.32f));  else g.fillAll(Colours::black.withAlpha(0.16f));
	//
	g.setColour(Colours::white.withAlpha(0.82f));
	g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(height) * 0.72f));
	//
	if (name.isNotEmpty())
	{
		g.setColour(Colours::lightblue.withAlpha(0.82f));
		g.drawText(name, 0, 0, width, height, Justification::left);
	}
	else if (level == kLevel_Sounds) g.drawText(sound->name, 0, 0, width, height, Justification::left);
	else if (level == kLevel_Sound_Files)
	{
		if (soundFile->files.size() > 0 && File(soundFile->files[0]).existsAsFile())
			g.drawFittedText(File(soundFile->files[0]).getFileNameWithoutExtension(), 0, 0, width, height, Justification::left, 1);
		else 
			g.drawText("No File", 0, 0, width, height, Justification::left);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXSoundTreeItem::itemClicked(const MouseEvent& e)
{
	#define WLOADSOUNDMSG "Load New Sound File(s) (multi select will group files as zones - when possible - use FILENAME_LLL_HHH_VLL_VHH_RRR.EXT)"
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)processor.getActiveEditor();
	//
	if (level == kLevel_Add_Sound)
	{
		FileChooser browseFile(WLOADSOUNDMSG, processor.getLastSoundFilePath(), processor.audioFormatManager.getWildcardForAllFormats());
		//
		if (browseFile.browseForMultipleFilesToOpen())
		{
			processor.collection->sounds.add(new WSPX_Collection_Sound);
			sound = processor.collection->sounds.getLast();
			addSubItem(new WSPXSoundTreeItem(processor, ui_ratio, kLevel_Sounds, "", kRegular_Item, sound));
			openOnlyLast(this);
			//
			processor.addSoundFiles(browseFile.getResults(), *sound, getSubItem(getNumSubItems() - 1));
			editor->editObject.set(WusikEditObject::kSound, 0, (void*)sound);
			//
			#if WSPXPLAYERPREVIEW
				if (processor.playerPreset != nullptr) processor.loadPreset(true);
					else editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
			#else
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
			#endif
		}
	}
	else if (level == kLevel_Sounds)
	{
		openOnly(this);
		editor->editObject.set(WusikEditObject::kSound, 0, (void*)sound);
		editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
	}
	else if (level == kLevel_Sound_Files)
	{
		if (specialItem == kRegular_Item)
		{
			openOnly(this);
			editor->loadSoundFileThumb(soundFile);
			editor->editObject.set(WusikEditObject::kSoundFile, 0, (void*)soundFile);
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
		else if (specialItem == kSound_Zones)
		{
			editor->editObject.set(WusikEditObject::kSoundZones, 0, (void*)sound);
			editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
		}
		else if (specialItem == kSound_Add)
		{
			FileChooser browseFile(WLOADSOUNDMSG, processor.getLastSoundFilePath(), processor.audioFormatManager.getWildcardForAllFormats());
			//
			if (browseFile.browseForMultipleFilesToOpen())
			{
				processor.addSoundFiles(browseFile.getResults(), *sound, getParentItem());
				editor->editObject.set(WusikEditObject::kSound, 0, (void*)sound);
				//
				#if WSPXPLAYERPREVIEW
					if (processor.playerPreset != nullptr) processor.loadPreset(true);
						else editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				#else
					editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				#endif
			}
			else reselectParent();
		}
		else if (specialItem == kSound_Remove)
		{
			StringArray presetsUsingThisSound;
			//
			for (int pp = 0; pp < processor.collection->presets.size(); pp++)
			{
				for (int ll = 0; ll < processor.collection->presets[pp]->layers.size(); ll++)
				{
					for (int ss = 0; ss < processor.collection->presets[pp]->layers[ll]->soundLinks.size(); ss++)
					{
						WSPX_Collection_Sound* checkSound = processor.collection->presets[pp]->layers[ll]->soundLinks[ss];
						if (checkSound == sound) presetsUsingThisSound.add(processor.collection->presets[pp]->name);
					}
				}
			}
			//
			if (presetsUsingThisSound.size() > 0)
			{
				WMessageBox("The following presets are using this sound. You must remove those presets before you can continue...",
					presetsUsingThisSound.joinIntoString("\n"));
			}
			else if (WConfirmBox("Remove Sound", "Are you sure?"))
			{
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sound,
					processor.collection->sounds.indexOf(sound), getParentItem()->getIndexInParent(), 0, 0, (void*)getParentItem());
				return; // we exit quickly as this is about to go down //
			}
			//
			reselectParent();
		}
	}
	else if (level == kLevel_Sound_File_Options)
	{
		if (specialItem == kSound_File_Open)
		{
			if (soundFile->files.size() > 1)
			{
				PopupMenu mm;
				for (int ff = 0; ff < soundFile->files.size(); ff++)
				{
					mm.addItem(ff + 1, File(soundFile->files[ff]).getFileName());
				}
				int result = mm.show();
				if (result > 0) File(soundFile->files[result - 1]).startAsProcess();
			}
			else if (File(soundFile->files[0]).existsAsFile()) File(soundFile->files[0]).startAsProcess();
			reselectParent();
		}
		else if (specialItem == kSound_File_Remove)
		{
			if (WConfirmBox("Remove Sound File", "Are you sure?"))
			{
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sound_File,
					processor.collection->sounds.indexOf(sound), sound->soundFiles.indexOf(soundFile), 
					getParentItem()->getIndexInParent(), 0, (void*)getParentItem());
				return; // we exit quickly as this is about to go down //
			}
			else reselectParent();
		}
		else if (specialItem == kSound_File_Multi_Channel_Load)
		{
			FileChooser browseFile("Load/Replace Sound Files to Multi Channels (merge all files)", processor.getLastSoundFilePath(""), processor.audioFormatManager.getWildcardForAllFormats());
			//
			if (browseFile.browseForMultipleFilesToOpen())
			{
				soundFile->files.clear();
				soundFile->channelsInfo.clear();
				//
				for (int ff = 0; ff < browseFile.getResults().size(); ff++)
				{
					soundFile->files.add(browseFile.getResults()[ff].getFullPathName());
				}
				//
				soundFile->sampleDataMetaValuesRead = false;
				processor.loadSoundFileDetails(soundFile);
				editor->loadSoundFileThumb(soundFile);
				//
				editor->presetChanged();
				editor->editObject.set(WusikEditObject::kSoundFile, 0, (void*)soundFile);
				//
				#if WSPXPLAYERPREVIEW
					if (processor.playerPreset != nullptr) processor.loadPreset(true);
						else editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				#else
					editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				#endif
			}
			//
			reselectParent();
		}
		else // Load File //
		{
			FileChooser browseFile("Load/Replace Sound File", processor.getLastSoundFilePath(""), processor.audioFormatManager.getWildcardForAllFormats());
			//
			if (browseFile.browseForFileToOpen())
			{
				soundFile->files.clear();
				soundFile->channelsInfo.clear();
				soundFile->files.add(browseFile.getResult().getFullPathName());
				//
				soundFile->sampleDataMetaValuesRead = false;
				processor.loadSoundFileDetails(soundFile);
				editor->loadSoundFileThumb(soundFile);
				//
				editor->presetChanged();
				editor->editObject.set(WusikEditObject::kSoundFile, 0, (void*)soundFile);
				//
				#if WSPXPLAYERPREVIEW
					if (processor.playerPreset != nullptr) processor.loadPreset(true);
						else editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				#else
					editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				#endif
			}
			//
			reselectParent();
		}
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::loadSoundFileThumb(WSPX_Collection_Sound_File* soundFile)
{
	soundFileThumbView.reset();
	if (soundFile->files.size() == 0 || soundFile->files[0].isEmpty() || !File(soundFile->files[0]).existsAsFile()) return;
	//
	// Check for a thumb file first //
	//
	File thumbFile = File::addTrailingSeparator(File(soundFile->files[0]).getParentDirectory().getFullPathName()) +
		File(soundFile->files[0]).getFileNameWithoutExtension() + ".WSPXeThumb";
	//
	soundFileThumbView.loopEnd = soundFile->loopEnd;
	soundFileThumbView.loopStart = soundFile->loopStart;
	//
	if (thumbFile.existsAsFile())
	{
		// Check for the file dates first //
		//
		ScopedPointer<FileInputStream> stream = thumbFile.createInputStream();
		//
		String header = stream->readString();
		char version = stream->readByte();
		String time = stream->readString();
		//
		if (File(soundFile->files[0]).getLastModificationTime().toString(true, true).compare(time) == 0)
		{
			PNGImageFormat pngReader;
			soundFileThumbView.waveform = pngReader.loadFrom(*stream);
			soundFileThumbView.ready.set(1);
			return;
		}
	}
	//
	WSPXSoundFileThumbThreadCreation soundFileThumbThread(this, soundFile->files[0], thumbFile);
	soundFileThumbThread.getAlertWindow()->setColour(AlertWindow::ColourIds::outlineColourId, Colours::red);
	soundFileThumbThread.runThread(6);
	if (soundFileThumbView.waveform.getWidth() > 0) soundFileThumbView.ready.set(1);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXSoundFileThumbThreadCreation::run()
{
	WusikSpxAudioProcessorEditor* theEditor = (WusikSpxAudioProcessorEditor*)editor;
	WusikSpxAudioProcessor* processor = (WusikSpxAudioProcessor*)theEditor->getAudioProcessor();
	ScopedPointer<AudioFormatReader> audioReader = processor->audioFormatManager.createReaderFor(fileToProcess);
	//
	if (audioReader != nullptr)
	{
		setProgress(0.0);
		//
		int channels = audioReader->numChannels;
		int totalSamples = audioReader->lengthInSamples;
		theEditor->soundFileThumbView.waveform = Image(Image::ARGB, 1280 * 2, 270 * 2, true);
		//
		AudioSampleBuffer soundBuffer(channels, totalSamples);
		audioReader->read(&soundBuffer, 0, totalSamples, 0, true, true);
		//
		// Normalize First //
		float maxValue = soundBuffer.getMagnitude(0, soundBuffer.getNumSamples());
		if (maxValue < 1.0f) soundBuffer.applyGain(1.0f / maxValue);
		setProgress(0.1);
		//
		Graphics gg(theEditor->soundFileThumbView.waveform);
		//
		setProgress(0.2);
		//
		if (totalSamples > theEditor->soundFileThumbView.waveform.getWidth())
		{
			double xPos = 0;
			double rate = double(theEditor->soundFileThumbView.waveform.getWidth()) / double(totalSamples);
			int xPosInt = 0;
			float sample[2] = { 0.0f, 0.0f };
			//
			// Create a normalize version of the buffer x area size //
			//
			AudioSampleBuffer soundBuffer2(channels, theEditor->soundFileThumbView.waveform.getWidth() + 100);
			setProgress(0.4);
			//
			for (int ss = 0; ss < soundBuffer.getNumSamples(); ss++)
			{
				sample[0] += soundBuffer.getReadPointer(0)[ss];
				if (channels > 1) sample[1] += soundBuffer.getReadPointer(1)[ss];
				xPos += rate;
				//
				if (int(xPos) != xPosInt)
				{
					sample[0] *= rate;
					sample[1] *= rate;
					//
					xPosInt = int(xPos);
					soundBuffer2.setSample(0, xPosInt, sample[0]);
					if (channels > 1) soundBuffer2.setSample(1, xPosInt, sample[1]);
					sample[0] = sample[1] = 0.0f;
				}
			}
			// Normalize Again //
			float maxValue = soundBuffer2.getMagnitude(0, soundBuffer2.getNumSamples());
			if (maxValue < 1.0f) soundBuffer2.applyGain(1.0f / maxValue);
			setProgress(0.6);
			//
			for (int ss = 0; ss < theEditor->soundFileThumbView.waveform.getWidth(); ss++)
			{
				for (int cc = 0; cc < channels; cc++)
				{
					sample[0] = soundBuffer2.getReadPointer(cc)[ss];
					//
					if (cc == 0) gg.setColour(Colours::white.withAlpha(0.90f));
					else gg.setColour(Colours::white.withAlpha(0.78f));
					//
					if (sample[0] > 0.0f)
					{
						gg.drawLine(ss, theEditor->soundFileThumbView.waveform.getHeight() / 2, ss,
							(theEditor->soundFileThumbView.waveform.getHeight() / 2) - ((double(theEditor->soundFileThumbView.waveform.getHeight() / 2) * sample[0])));
					}
					else
					{
						gg.drawLine(ss,
							theEditor->soundFileThumbView.waveform.getHeight() / 2,
							ss,
							(theEditor->soundFileThumbView.waveform.getHeight() / 2) +
							((float(theEditor->soundFileThumbView.waveform.getHeight() / 2) * fabs(sample[0]))));
					}
				}
			}
		}
		else
		{
			setProgress(0.6);
			//
			for (int ss = 0; ss < soundBuffer.getNumSamples(); ss++)
			{
				for (int cc = 0; cc < channels; cc++)
				{
					float sample = soundBuffer.getReadPointer(cc)[ss];
					//
					if (cc == 0) gg.setColour(Colours::white.withAlpha(0.90f));
					else gg.setColour(Colours::white.withAlpha(0.78f));
					//
					if (sample > 0.0f)
					{
						gg.drawLine(ss, theEditor->soundFileThumbView.waveform.getHeight() / 2, ss,
							(theEditor->soundFileThumbView.waveform.getHeight() / 2) - ((double(theEditor->soundFileThumbView.waveform.getHeight() / 2) * sample)));
					}
					else
					{
						gg.drawLine(ss,
							theEditor->soundFileThumbView.waveform.getHeight() / 2,
							ss,
							(theEditor->soundFileThumbView.waveform.getHeight() / 2) +
							((float(theEditor->soundFileThumbView.waveform.getHeight() / 2) * fabs(sample))));
					}
				}
			}
		}
		//
		gg.setColour(Colours::black.withAlpha(0.76f));
		gg.drawRect(0, 0, theEditor->soundFileThumbView.waveform.getWidth(), theEditor->soundFileThumbView.waveform.getHeight(), 4);
		gg.drawLine(0, theEditor->soundFileThumbView.waveform.getHeight() / 2, theEditor->soundFileThumbView.waveform.getWidth(), theEditor->soundFileThumbView.waveform.getHeight() / 2, 2);
		//
		// Create thumb file //
		//
		setProgress(0.8);
		if (thumbFile.existsAsFile()) thumbFile.deleteFile();
		ScopedPointer<FileOutputStream> stream = thumbFile.createOutputStream();
		//
		stream->writeString("WSPXeThumb");
		stream->writeByte(1); // Version //
		stream->writeString(fileToProcess.getLastModificationTime().toString(true, true));
		//
		PNGImageFormat pngWriter;
		pngWriter.writeImageToStream(theEditor->soundFileThumbView.waveform, *stream);
		stream->flush();
	}
	else
	{
		AlertWindow::showMessageBox(AlertWindow::NoIcon, "Error Loading Sound File! Couldn't create a reader for it...", fileToProcess.getFullPathName());
	}
	//
	setProgress(1.0);
	sleep(100);
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXKeyVelZone::updateKeysAndLabel(bool mouseAway)
{
	WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)theEditor;
	//
	if (mouseAway)
	{
		editor->midiKeyboard.selectedHigh = editor->midiKeyboard.selectedLow = editor->midiKeyboard.rootKey = -1;
		editor->midiKeyboard.repaint();
		editor->statusLabel->setVisible(false);
	}
	else
	{
		editor->midiKeyboard.selectedHigh = sound->keyZoneHigh;
		editor->midiKeyboard.selectedLow = sound->keyZoneLow;
		editor->midiKeyboard.rootKey = sound->keyRoot;
		editor->midiKeyboard.repaint();
		//
		if (sound->files.size() == 0 || sound->files[0].isEmpty()) ((WSPXStatusLabel*)editor->statusLabel)->text = "No File";
		else if (File(sound->files[0]).existsAsFile()) ((WSPXStatusLabel*)editor->statusLabel)->text = File(sound->files[0]).getFileName();
		else ((WSPXStatusLabel*)editor->statusLabel)->text = "Missing File";
		//
		editor->statusLabel->setBounds(getBounds().getX() + 8, getBounds().getY() + 8, 320, 42);
		editor->statusLabel->setAlwaysOnTop(true);
		editor->statusLabel->setVisible(true);
	}
}
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WSPXKeyVelZone::mouseUp(const MouseEvent& event)
{
	if (!hasMoved)
	{
		PopupMenu mm;
		mm.addItem(1, "Load");
		mm.addItem(2, "Open");
		mm.addItem(4, "Edit");
		mm.addItem(6, "Remove");
		//
		int result = mm.show();
		if (result > 0)
		{
			WusikSpxAudioProcessorEditor* editor = (WusikSpxAudioProcessorEditor*)theEditor;
			editor->statusLabel->setVisible(false);
			//
			if (result == 2)
			{
				if (sound->files.size() > 0 && File(sound->files[0]).existsAsFile()) File(sound->files[0]).startAsProcess();
			}
			else if (result == 6)
			{
				if (WConfirmBox("Remove Sound File", "Are you sure?"))
				{
					editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Remove_Sound_File_From_Zones, 0, 0, 0, 0, (void*)sound);
					return; // we exit quickly as this is about to go down //
				}
			}
			else if (result == 1)
			{
				WusikSpxAudioProcessor* processor = ((WusikSpxAudioProcessor*)editor->getAudioProcessor());
				FileChooser browseFile("Load/Replace Sound File", processor->getLastSoundFilePath(sound->files[0]), processor->audioFormatManager.getWildcardForAllFormats());
				//
				if (browseFile.browseForFileToOpen())
				{
					sound->files.clear();
					sound->channelsInfo.clear();
					//
					sound->files.add(browseFile.getResult().getFullPathName());
					sound->sampleDataMetaValuesRead = false;
					processor->loadSoundFileDetails(sound);
					editor->loadSoundFileThumb(sound);
					editor->presetChanged();
					editor->repaint();
				}
			}
			else if (result == 4)
			{
				editor->editObject.type = WusikEditObject::kSoundFile;
				editor->editObject.object = (void*) sound;
				editor->createAction(WusikSpxAudioProcessorEditor::kTimerAction_Update_Interface_Not_TreeViews);
				return; // we exit quickly as this is about to go down //
			}
		}
	}
}