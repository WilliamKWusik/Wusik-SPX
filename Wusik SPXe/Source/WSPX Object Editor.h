/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "WSPX Components.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditObject
{
public:
	void set(int _type, int _index, void* _object = nullptr) { type = _type; object = _object; index = _index; }
	int type = kCollection;
	void* object = nullptr;
	int index = 0;
	//
	enum
	{
		kNone,
		kCollection,
		kPreset,
		kPresetLayer,
		kSound,
		kSoundFile
	};
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditOptionCallback
{
public:
	virtual void process(WusikSpxAudioProcessor* processor) { }
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditOptionCallback_UpdateCollectionName : public WusikEditOptionCallback
{
public:
	virtual void process(WusikSpxAudioProcessor* processor);
};
//
// ------------------------------------------------------------------------------------------------------------------------- //
class WusikEditOption : public Component
{
public:
	WusikEditOption(WusikSpxAudioProcessor* _processor, Component* _editor, int _type, String _label, void* _object = nullptr, 
		String _extraLabel = String(), bool _showEditInstead = false, WusikEditOptionCallback* _callback = nullptr, int _min = 0, 
		int _max = 1, String _popupList = String());
	//
	void mouseMove(const MouseEvent& e) override { repaint(); }
	void mouseExit(const MouseEvent& e) override { repaint(); }
	void mouseEnter(const MouseEvent& e) override { repaint(); }
	//
	void mouseUp(const MouseEvent& e) override
	{
		if (type == kPopupList)
		{
			int index = jlimit(0, popupList.size() - 1, int(((float*)object)[0] * float(popupList.size() - 1)));
			PopupMenu mm;
			//
			for (int tt = 0; tt < popupList.size(); tt++)
			{
				mm.addItem(tt + 1, popupList[tt], true, index == tt);
			}
			//
			int result = mm.show();
			if (result > 0)
			{
				((float*)object)[0] = float(result - 1) / float(popupList.size() - 1);
			}
		}
		else if (type == kOnOffButton)
		{
			if (((float*)object)[0] > 0.0f) ((float*)object)[0] = 0.0f; else ((float*)object)[0] = 1.0f;
		}
		else if (type == kString)
		{
			String sValue;
			AskValue(label, extraLabel, ((String*)object)[0], "", "OK", "Cancel", sValue);
			if (sValue.isNotEmpty())
			{
				((String*)object)[0] = sValue;
				processor->collection->hasUnsavedChanges = true;
			}
		}
		else if (type == kStringInt64)
		{
			String sValue;
			AskValue(label, extraLabel, String(((int64*)object)[0]), "", "OK", "Cancel", sValue);
			if (sValue.isNotEmpty())
			{
				((int64*)object)[0] = jlimit((int64) min, (int64) max, sValue.getLargeIntValue());
				processor->collection->hasUnsavedChanges = true;
			}
		}
		else if (type == kImage)
		{
			bool loadImage = true;
			WSPX_Image* theImage = (WSPX_Image*)object;
			//
			if (File(theImage->imageFilename).existsAsFile())
			{
				loadImage = false;
				PopupMenu mm;
				mm.addItem(1, "View Image");
				mm.addItem(2, "Remove Image");
				mm.addItem(4, "Replace Image");
				//
				int result = mm.show();
				if (result > 0)
				{
					if (result == 2)
					{
						theImage->imageFilename = String();
						theImage->image = Image();
						processor->collection->hasUnsavedChanges = true;
					}
					else if (result == 4) loadImage = true;
					else if (result == 1) File(theImage->imageFilename).startAsProcess();
				}
			}
			//
			if (loadImage)
			{
				FileChooser browseFile("Load Image", File(theImage->imageFilename).getParentDirectory().getFullPathName(), "*.png;*.jpg");
				//
				if (browseFile.browseForFileToOpen())
				{
					theImage->imageFilename = browseFile.getResult().getFullPathName();
					theImage->image = ImageFileFormat::loadFrom(theImage->imageFilename);
					processor->collection->hasUnsavedChanges = true;
				}
			}
		}
		//
		if (callback != nullptr) callback->process(processor);
		editor->repaint();
	}
	//
	void paint(Graphics& g) override
	{
		if (type == kLabel)
		{
			g.fillAll(Colours::black.withAlpha(0.22f));
			g.setColour(Colours::white.withAlpha(0.94f));
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.52f));
			g.drawFittedText(label, 8, 4, getWidth() - 16, getHeight(), Justification::centred, 1);
		}
		else if (type == kLabelSmall)
		{
			g.fillAll(Colours::black.withAlpha(0.22f));
			g.fillAll(Colours::white.withAlpha(0.06f));
			g.setColour(Colours::black.withAlpha(0.32f));
			g.drawRect(0, 0, getWidth(), getHeight(), 2);
			//
			g.setColour(Colours::white.withAlpha(0.88f));
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.32f));
			g.drawFittedText(label, 8, 4, getWidth() - 16, getHeight(), Justification::centred, 1);
		}
		else
		{
			g.fillAll(Colours::black.withAlpha(0.12f));
			if (isMouseOver(true)) g.fillAll(Colours::red.withAlpha(0.12f));
			//
			g.setColour(Colours::white.withAlpha(0.26f));
			g.drawRect(0, 0, getWidth(), getHeight(), 1);
			g.setColour(Colours::white.withAlpha(0.86f));
			g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.32f));
			g.drawFittedText(label, 8, 0, (double(getWidth()) * 0.26) - 16, getHeight(), Justification::centredLeft, 1);
			//
			if (type == kPopupList)
			{
				int index = ((float*)object)[0] * float(popupList.size() - 1);
				String xText = popupList[jlimit(0, popupList.size() - 1, int(index))];
				g.drawFittedText(xText, 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
			}
			else if (type == kOnOffButton)
			{
				if (((float*)object)[0] > 0.0f)
				{
					g.drawFittedText("ENABLED", 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
				}
				else
				{
					g.setFont(LookAndFeelEx::getCustomFont().withHeight(double(getHeight()) * 0.28f));
					g.setColour(Colours::white.withAlpha(0.46f));
					g.drawFittedText("DISABLED", 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
				}
			}
			else if (showEditInstead)
			{
				g.drawFittedText("EDIT", 0, 0, getWidth() - 16, getHeight(), Justification::centredRight, 1);
			}
			else
			{
				if (type == kString)
				{
					g.drawFittedText(((String*)object)[0], (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
				else if (type == kStringInt64)
				{
					g.drawFittedText(String(((int64*)object)[0]), (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
				else if (type == kImage)
				{
					WSPX_Image* theImage = (WSPX_Image*)object;
					if (File(theImage->imageFilename).existsAsFile())
						g.drawFittedText(File(theImage->imageFilename).getFileNameWithoutExtension(), (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
					else
						g.drawFittedText("No File Selected", (double(getWidth()) * 0.26) + 8, 0, getWidth() - 16 - (double(getWidth()) * 0.26), getHeight(), Justification::centredRight, 1);
				}
				else if (type >= kSlider && type <= kSliderIntegerBipolar)
				{
					slider->setBounds(getWidth() - (double(getWidth()) * 0.42), 12, double(getWidth()) * 0.40, getHeight() - 16);
				}
			}
		}
	}
	//
	void* object;
	String label;
	WSlider* slider = nullptr;
	int type = 0;
	int min = 0;
	int max = 1;
	bool showEditInstead = false;
	ScopedPointer<WusikEditOptionCallback> callback = nullptr;
	WusikSpxAudioProcessor* processor;
	String extraLabel;
	Component* editor;
	StringArray popupList;
	//
	enum
	{
		kString,
		kStringInt64,
		kImage,
		kLabel,
		kLabelSmall,
		kPopupList,
		kSlider,
		kSliderBipolar,
		kSliderInteger,
		kSliderIntegerBipolar,
		kOnOffButton
	};
};