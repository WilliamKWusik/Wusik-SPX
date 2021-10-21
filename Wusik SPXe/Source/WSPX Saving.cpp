/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#include "PluginProcessor.h"
//
// ------------------------------------------------------------------------------------------------------------------------- //
void WusikSpxAudioProcessor::saveCompilation(OutputStream& stream)
{
	// This saves the Compilation file in WSPXe format (editor format) not the final WSPX format (player format) //
	//
	stream.writeString("WSPXe Source Compilation File");
	stream.writeByte(1); // WSPXe File Version //
	//
	stream.writeString(collection->name);
	stream.writeString(collection->author);
	stream.writeString(collection->description);
	stream.writeString(collection->company);
	stream.writeString(collection->tags);
	stream.writeString(collection->version);
	stream.writeString(collection->file.getFullPathName());
	stream.writeString(collection->exportedFile.getFullPathName());
	stream.writeString(collection->protectionKey);
	stream.writeString(collection->imageAbout.imageFilename.getFullPathName());
	stream.writeString(collection->imageIcon.imageFilename.getFullPathName());
	//
	stream.writeInt(collection->presets.size());
	stream.writeInt(collection->soundGroups.size());
	//
	for (int pp = 0; pp < collection->presets.size(); pp++)
	{
		stream.writeString(collection->presets[pp]->name);
		stream.writeString(collection->presets[pp]->author);
		stream.writeString(collection->presets[pp]->description);
		stream.writeString(collection->presets[pp]->tags);
		stream.writeFloat(collection->presets[pp]->volume);
		stream.writeFloat(collection->presets[pp]->pan);
		stream.writeFloat(collection->presets[pp]->fineTune);
		stream.writeByte((uint8_t)collection->presets[pp]->coarseTune);
		stream.writeInt(collection->presets[pp]->effects.size());
		stream.writeInt(collection->presets[pp]->layers.size());
		//
		if (collection->presets[pp]->imagePresetIcon == nullptr) stream.writeString("");
		else stream.writeString(collection->presets[pp]->imagePresetIcon->imageFilename.getFullPathName());
		//
		for (int ll = 0; ll < collection->presets[pp]->layers.size(); ll++)
		{
			stream.writeFloat(collection->presets[pp]->layers[ll]->volume);
			stream.writeFloat(collection->presets[pp]->layers[ll]->pan);
			stream.writeFloat(collection->presets[pp]->layers[ll]->fineTune);
			stream.writeFloat(collection->presets[pp]->layers[ll]->glide);
			stream.writeInt(collection->presets[pp]->layers[ll]->keyZoneHigh);
			stream.writeInt(collection->presets[pp]->layers[ll]->keyZoneLow);
			stream.writeInt(collection->presets[pp]->layers[ll]->velZoneHigh);
			stream.writeInt(collection->presets[pp]->layers[ll]->velZoneLow);
			stream.writeBool(collection->presets[pp]->layers[ll]->autoGlide);
			stream.writeBool(collection->presets[pp]->layers[ll]->reverse);
			stream.writeByte((uint8_t)collection->presets[pp]->layers[ll]->coarseTune);
			stream.writeByte(collection->presets[pp]->layers[ll]->output);
			stream.writeByte(collection->presets[pp]->layers[ll]->voices);
			collection->presets[pp]->layers[ll]->filter.save(stream);
			collection->presets[pp]->layers[ll]->ampEnvelope.save(stream);
			collection->presets[pp]->layers[ll]->lfos[0].save(stream);
			collection->presets[pp]->layers[ll]->lfos[1].save(stream);
			//
			stream.writeInt(collection->presets[pp]->layers[ll]->scripting.getSize());
			if (collection->presets[pp]->layers[ll]->scripting.getSize() > 0) stream.write(collection->presets[pp]->layers[ll]->scripting.getData(), collection->presets[pp]->layers[ll]->scripting.getSize());
			//
			stream.writeInt(collection->presets[pp]->layers[ll]->channels.size());
			for (int ch = 0; ch < collection->presets[pp]->layers[ll]->channels.size(); ch++)
			{
				stream.writeString(collection->presets[pp]->layers[ll]->channels[ch]->name);
				stream.writeFloat(collection->presets[pp]->layers[ll]->channels[ch]->volume);
			}
			//
			stream.writeInt(collection->presets[pp]->layers[ll]->soundGroupIDs.size());
			for (int ss = 0; ss < collection->presets[pp]->layers[ll]->soundGroupIDs.size(); ss++)
			{
				stream.writeInt(collection->presets[pp]->layers[ll]->soundGroupIDs[ss]);
			}
		}
	}
	//
	stream.flush();
	collection->hasUnsavedChanges = false;
}