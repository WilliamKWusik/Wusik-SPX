# Wusik SPXe
 Created by William Kalfelz @ Wusik.com (c) 2021
 Open-Source File Format Definition and Editor

Here you will find the definition of the format plus a full free open-source editor. There's no player yet, but soon there will be one. The player will not be open-source. You will need a license to be able to distribute your sounds with the player. Or, you can create your own player. The use of the file format is free and not tied to the player in any way. The editor doesn't feature sample manipulation. So you need to use something like Ocean Audio to edit and loop your sounds. https://www.ocenaudio.com/

Check the Releases folder for the Application (.exe) and VST2/VST3 Plugins. Again, this is an EDITOR, to create presets + sounds. Not the actual PLAYER.

.WSPX Format Main Features
- Single file for Sounds + Presets
- Internal database of presets & sounds positions, no need to read the whole file
- Encripted Sound Data (optional)
- Visual Scripting (from the editor)
- Binary, Flac, and GZIP can be used to store sounds
- Round Robin, Random Probability and Key Switching
- Tag Database For Sounds and Presets
- 16, 24 and 32 bits support
- Collection, Preset and About PNG files
- Skin data for the Player

.WSPX Engine Support
- Multi layer presets. Each layer has a link to a Sound that has its own set of sub-sounds, or Sound Files
- ADSR AMP and Filter Envelope
- 2 LFOs
- Sequencer with any number of steps
- Multi type Filter
- Multi Effects. 4 sends with 4 effects each. Serial and Parallel modes
- Round Robin, Random Probability and Key Switching
- Visual Scripting (from the editor)
- Reverse, Ping Pong and Regular playbacks

When IMPORTING sound files (WAV,AIFF,OGG,MP3,FLAC) you can name those in the following format for easier key/velocity zone setup.

- MY SAMPLE_000_127_000_127_060.wav
- MY SAMPLE_LLL_HHH_VLL_VHH_KRR.wav
- LLL: Key Low
- HHH: Key High
- VLL: Velocity Low
- VHH: Velocity High
- KRR: Key Root

Always keep in a 000 format, so 000 to 127. When you load a sound file the editor will only save the filename and path location. It won’t save any sound data. So keep the original files until you finish up the project. You can save to a BUNDLE format which will save the sound data information along.

![Screenshot](/Screenshots/2021-10-26%2022_06_51-Window.png)
