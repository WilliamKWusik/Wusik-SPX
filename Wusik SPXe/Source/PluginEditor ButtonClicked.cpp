/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
#include "PluginProcessor.h"
#include "PluginEditor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == logoButton) showAbout();
	else if (buttonThatWasClicked == collectionButton)
	{
		redoTreeViewsOnResize = false;
		cleanInterface();
		editObject.type = WusikEditObject::kCollection;
		updateInterface();
	}
	else if (buttonThatWasClicked == saveButton)
	{
		bool saveFile = true;
		WSPXeBundle = false;
		//
		if (!processor.collection->file.existsAsFile())
		{
			saveFile = false;
			FileChooser browseFile("Save WSPXe Collection File (this is not the final WSPX file format)", processor.collection->file, "*.WSPXe");
			//
			if (browseFile.browseForFileToSave(true))
			{
				saveFile = true;
				processor.collection->file = browseFile.getResult();
			}
		}
		//
		if (saveFile)
		{
			File backupFile(File::addTrailingSeparator(processor.collection->file.getParentDirectory().getFullPathName()) + processor.collection->file.getFileNameWithoutExtension() + ".WSPXeBackup");
			backupFile.deleteFile();
			processor.collection->file.moveFileTo(backupFile);
			//
			FileOutputStream stream(processor.collection->file);
			processor.saveCompilation(stream);
			//
			repaint();
		}
	}
	else if (buttonThatWasClicked == fileButton)
	{
		PopupMenu mm;
		mm.addItem(1, "Load");
		mm.addItem(2, "New");
		mm.addSeparator();
		mm.addItem(4, "Save");
		mm.addItem(6, "Save As");
		mm.addItem(12, "Save Bundle");
		mm.addSeparator();
		mm.addItem(8, "Export .WSPX");
		mm.addSeparator();
		mm.addItem(14, "About");
		//
		mm.setLookAndFeel(newLookAndFeel);
		//
		int result = mm.show();
		mm.setLookAndFeel(nullptr);
		if (result > 0)
		{
			bool updateTheInterface = false;
			if (result == 1 || result == 2) cleanInterface();
			//
			if (result == 2)
			{
				processor.newCompilation();
				repaint();
			}
			else if (result == 14) showAbout();
			else if (result == 1)
			{
				if (!processor.collection->hasUnsavedChanges || AlertWindow::showOkCancelBox(AlertWindow::NoIcon, "There are unsaved changes!", "Are you sure you want to continue?"))
				{
					FileChooser browseFile("Load WSPXe Collection File", processor.collection->file, "*.WSPXe;*.WSPXeBundle");
					//
					if (browseFile.browseForFileToOpen())
					{
						FileInputStream stream(browseFile.getResult());
						processor.loadCompilation(stream);
					}
				}
			}
			else if (result == 4)
			{
				WSPXeBundle = false;
				buttonClicked(saveButton);
			}
			else if (result == 12)
			{
				FileChooser browseFile("Save WSPXe Bundle Collection File (includes all samples - this is not the final WSPX file format)", processor.collection->file, "*.WSPXeBundle");
				//
				if (browseFile.browseForFileToSave(true))
				{
					WSPXeBundle = true;
					processor.collection->file = browseFile.getResult();
					processor.collection->file.deleteFile();
					//
					processor.processThread = new WSPXThread((void*)&processor, this, WSPXThread::kSaveBundle, "Saving WSPXe Bundle");
					processor.processThread->runThread(4);
					processor.processThread = nullptr;
					WSPXeBundle = false;
					//
					repaint();
				}
			}
			else if (result == 6)
			{
				FileChooser browseFile("Save WSPXe Collection File (this is not the final WSPX file format)", processor.collection->file, "*.WSPXe");
				//
				if (browseFile.browseForFileToSave(true))
				{
					WSPXeBundle = false;
					processor.collection->file = browseFile.getResult();
					processor.collection->file.deleteFile();
					//
					FileOutputStream stream(processor.collection->file);
					processor.saveCompilation(stream);
					//
					repaint();
				}
			}
			else if (result == 8)
			{
				WMessageBox("Not Implemented Yet!", "");
				//
				/*FileChooser browseFile("Export To WSPX Collection File", processor.collection->exportedFile, "*.WSPX");
				//
				if (browseFile.browseForFileToSave(true))
				{
					processor.collection->exportedFile = browseFile.getResult();
				}*/
			}
			//
			if (result == 1 || result == 2)
			{
				addAndMakeVisible(presetsTreeView = new WusikTreeHolder(processor, true, multRatio));
				addAndMakeVisible(soundsTreeView = new WusikTreeHolder(processor, false, multRatio));
				editObject.type = WusikEditObject::kCollection;
				updateInterface();
			}
		}
	}
	else if (buttonThatWasClicked == previewButton)
	{
		processor.loadPreset();
	}
	else if (buttonThatWasClicked == uiButton)
	{
		if (processor.playerPreset != nullptr && processor.processThread != nullptr)
		{
			processor.processThread->openPlayerUI();
		}
		else
		{
			processor.openUIWhenLoadingPreset = true;
			processor.loadPreset(true);
		}
	}
}