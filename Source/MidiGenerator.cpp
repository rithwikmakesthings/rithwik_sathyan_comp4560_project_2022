/*
 ==============================================================================
 midiGenerator.cpp
 Created: 9 March 2022
 Author:  Rithwik Sathyan
 Main MIDI Processor
 ==============================================================================
 */

#include "MidiGenerator.h"

MidiGenerator::MidiGenerator()
: Thread("MIDI output Generator")
{
    midiDeviceInfoArray = MidiOutput::getAvailableDevices();
    
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
    virtualMidiOuputDeviceIndex = midiDeviceInfoArray.size();
#endif
}

MidiGenerator::~MidiGenerator()
{
    if (isThreadRunning())
        stopThread(500);
}


void MidiGenerator::initSequenceData()
{
    wait(100);
    //initialise with null notes.
    for (int section = 0; section < instrumentCount; section++)
    {
        for (int note = 0; note < sequenceNoteSize; note++)
        {
            noteSequence[section][note].note_timestamp = nullNote;
        }
    }
    
    //create a random number generator
    Random randomGen(Time::currentTimeMillis());
    //==================================================================================
    
    if(firsttime){
        root_note = 36; //C2
        scale_to_use = 0; //major for now
        tempo = 96; //anywhere from 70-120 works nicely
        
        //==================================================================================
        //Apply melody data values
        
        melody_num_of_octaves = 3;
        melody_main_velocity = 66;
        melody_velocity_offset = 20;
        melody_note_length = 6;
        melody_note_frequency = 2;
        melody_note_rest_freq = 5;
        
        //==================================================================================
        //Apply pad data values
        
        pads_chord_prog_to_use = 1;
        pads_note_length = 24;
        pads_density = 4;
        pads_main_velocity = 54;
        pads_chord_interval = 1;
        
        //==================================================================================
        //Apply bass data values
        
        bass_main_velocity = 51;
        std::cout << "bass velocity: " << bass_main_velocity << std::endl;
        
        bass_note_division = 4;
        std::cout << "Bass note division: " << bass_note_division << std::endl;
        
        //==================================================================================
        //Apply drum data values
        
        drums_kick_pattern_to_use = 3;
        drums_snare_pattern_to_use = 4;
        drums_perc1_pattern_to_use = 5;
        drums_perc2_pattern_to_use = 3;
        drums_offbeat_value = 1;
        drums_main_velocity = 73;
        drums_velocity_offset = 15;
        firsttime = false;
    }
    else {
        updateParams();}
}

void MidiGenerator::randomSequenceData()
{
    wait(100);
    //initialise with null notes.
    for (int section = 0; section < instrumentCount; section++)
    {
        for (int note = 0; note < sequenceNoteSize; note++)
        {
            noteSequence[section][note].note_timestamp = nullNote;
        }
    }
    
    //create a random number generator for various uses below
    Random randomGen(Time::currentTimeMillis());
    //==================================================================================
    
    root_note = randomGen.nextInt (juce::Range<int> (36, 48)); //C2
    
    scale_to_use = randomGen.nextInt (juce::Range<int> (0, scaleCount-1));
    
    tempo = randomGen.nextInt (juce::Range<int> (50, 100)); //anywhere from 70-120 works nicely
    
    //==================================================================================
    //Apply melody data values
    
    melody_num_of_octaves = randomGen.nextInt (juce::Range<int> (1, 3));
    melody_main_velocity = randomGen.nextInt (juce::Range<int> (30, 127));
    melody_velocity_offset = randomGen.nextInt (juce::Range<int> (1, 30));
    melody_note_length = randomGen.nextInt (juce::Range<int> (1, 16));
    melody_note_frequency = randomGen.nextInt (juce::Range<int> (1, noteLengthCount-1));
    melody_note_rest_freq = randomGen.nextInt (juce::Range<int> (1, 16));
    
    //==================================================================================
    //Apply pad data values
    
    pads_chord_prog_to_use = randomGen.nextInt (juce::Range<int> (1, 16));
    pads_note_length = randomGen.nextInt (juce::Range<int> (16, 64));
    pads_density = randomGen.nextInt (juce::Range<int> (1, 4));
    pads_main_velocity = randomGen.nextInt (juce::Range<int> (30, 127));
    pads_chord_interval = randomGen.nextInt (juce::Range<int> (1, 4));
    
    //==================================================================================
    //Apply bass data values
    
    bass_main_velocity = randomGen.nextInt (juce::Range<int> (30, 127));
    bass_note_division = randomGen.nextInt (juce::Range<int> (1, 8));
    
    //==================================================================================
    //Apply drum data values
    
    drums_kick_pattern_to_use = randomGen.nextInt (juce::Range<int> (2, drumPatternCount-2));
    drums_snare_pattern_to_use = randomGen.nextInt (juce::Range<int> (2, drumPatternCount-2));
    drums_perc1_pattern_to_use = randomGen.nextInt (juce::Range<int> (2, drumPatternCount-2));
    drums_perc2_pattern_to_use = randomGen.nextInt (juce::Range<int> (2, drumPatternCount-2));
    drums_offbeat_value = randomGen.nextInt (juce::Range<int> (1, 8));
    drums_main_velocity = randomGen.nextInt (juce::Range<int> (30,127));
    drums_velocity_offset = randomGen.nextInt (juce::Range<int> (1, 30));
    
    setSequenceData();
}


void MidiGenerator::setSequenceData()
{
    wait(100);
    //initialise with null notes.
    for (int section = 0; section < instrumentCount; section++)
    {
        for (int note = 0; note < sequenceNoteSize; note++)
        {
            noteSequence[section][note].note_timestamp = nullNote;
        }
    }
    createRootValueTree();
    
    //==================================================================================
    //Add notes to melody sequence
    //create buffer of notes to use based on root note, scale, and num of octaves.
    melodySequencer();
    
    //==================================================================================
    //Add notes to pad sequence
    //Chord are applied in a pattern of four, depend on the chosen chord progression and chosen scale.
    //The step numbers of the four notes depends on the max number of steps.
    //note numbers applied here are the global root note + the index of the chosen scale relating to the values of the chosen chord progression.
    harmonySequencer();
    
    //==================================================================================
    //Add notes to bass sequence
    bassSequencer();
    
    //==================================================================================
    //Add notes to drum sequence
    //Drum patterns are applied from the pallete of patterns in Scale&Patterns.h
    drumSequencer();
    
    //==================================================================================
    //Sort arrays so that they are in ascending NoteData.note_timestamp order
    //Exchange Sort method - http://mathbits.com/MathBits/CompSci/Arrays/Sorting.htm
    sequencerSync();
}




void MidiGenerator::run()
{
    //This thread runs using Time::getMillisecondCounterHiRes() and a set time interval.
    //It uses a current step number and searches through all the note sequence arrays
    //to determine if a note needs to be turned on or off at this particular step in time.
    
    //==================================================================================
    //Setup
    
    int current_time = Time::getMillisecondCounterHiRes();
    int step = 0;
    
    for (int layer = 0; layer < instrumentCount; layer++)
        currentArrayPos[layer] = startArrayPos[layer];
    
    //With this array of NoteData we don't use the length or vel params
    NoteData note_off_buffer[noteOffBuffer];
    for (int i = 0; i < noteOffBuffer; i++)
        note_off_buffer[i].note_timestamp = nullNote;
    
    //Main Loop
    while (!threadShouldExit())
    {
        //if we have reach the next 'step' in time, process the sequence
        if (Time::getMillisecondCounterHiRes() >= current_time)
        {
            //increase currentTime
            current_time += tempo;
            
            //search through the note-off buffer for notes that need to be turned off on this step
            for (int i = 0; i < noteOffBuffer; i++)
            {
                //if the notes step number matches the current step number
                if (note_off_buffer[i].note_timestamp == step)
                {
                    //turn off the note
                    MidiMessage midiMessage = MidiMessage::noteOff(note_off_buffer[i].note_channel, note_off_buffer[i].note_number);
                    sendMidiMessage(midiMessage);
                    noteOffCallback (note_off_buffer[i].note_channel, note_off_buffer[i].note_number);
                    
                    
                    //'remove' this note from the note-off buffer by shuffling down the notes higher in the buffer
                    for (int j = i; j < noteOffBuffer - 1; j++)
                    {
                        note_off_buffer[j] = note_off_buffer[j + 1];
                    }
                    
                    //move i back one incase the moved next note needs turning off too.
                    i--;
                    
                }
                
                //if we have reached the end of actual notes in the buffer, exit the loop
                if (note_off_buffer[i].note_timestamp == nullNote)
                    break;
                
            }
            
            
            //search through the appropriate range of the sequence arrays for notes that need to be turned on on this step
            for (int layer = 0; layer < instrumentCount; layer++)
            {
                for (int i = currentArrayPos[layer]; i < sequenceNoteSize; i++)
                {
                    //if this object/note has a matching step number
                    if (noteSequence[layer][i].note_timestamp == step)
                    {
                        //play the note
                        MidiMessage midiMessage = MidiMessage::noteOn(noteSequence[layer][i].note_channel,
                                                                      noteSequence[layer][i].note_number,
                                                                      (uint8)noteSequence[layer][i].note_velocity);
                        sendMidiMessage(midiMessage);
                        noteCallback (noteSequence[layer][i].note_channel,noteSequence[layer][i].note_number);
                        
                        
                        //add this note to the note-off buffer
                        for (int j = 0; j < noteOffBuffer; j++)
                        {
                            //if this index of note_off_buffer is currently not being used
                            if (note_off_buffer[j].note_timestamp == nullNote)
                            {
                                //set the note_timestamp to be the current step number + the notes length
                                //may need to wrap round to the beggining for step number
                                int off_step = step + noteSequence[layer][i].note_length;
                                
                                if (off_step >= sequenceStepSize)
                                    off_step -= sequenceStepSize;
                                
                                note_off_buffer[j].note_timestamp = off_step;
                                
                                note_off_buffer[j].note_channel = noteSequence[layer][i].note_channel;
                                note_off_buffer[j].note_number = noteSequence[layer][i].note_number;
                                break;
                                
                            }
                        }
                    }
                    //if the step number doesn't match, we have finished processing all notes for this step...
                    else
                    {
                        //...so store this step number, so for the next step we can start searching at this index in the array.
                        
                        currentArrayPos[layer] = i;
                        
                        
                        //break out of searching through this layers array
                        break;
                    }
                }
            }
            
            //==================================================================================
            step++;
            //SEQUENCE END
            //if we have reached the last step in the sequence, reset the needed variables/
            if (step >= sequenceStepSize)
            {
                updateParams();
                setSequenceData();
                step = 0;
                
                for (int layer = 0; layer < instrumentCount; layer++)
                    currentArrayPos[layer] = startArrayPos[layer];
            }
        }
        wait(1);
    }
    
    //==================================================================================
    
    
    for (int i = 0; i < noteOffBuffer; i++)
    {
        //if this index is a note
        if (note_off_buffer[i].note_timestamp != nullNote)
        {
            //turn off the note
            MidiMessage midiMessage = MidiMessage::noteOff(note_off_buffer[i].note_channel, note_off_buffer[i].note_number);
            sendMidiMessage(midiMessage);
            
            //'remove' this note from the note-off buffer
            note_off_buffer[i].note_timestamp = nullNote;
            
        }
    }
}

void MidiGenerator::bassSequencer (){
    
    int chord_length = sequenceStepSize / chordProgressionLength;
    int noteSeqIndex = 0; //this needs to be iterated after everytime we add a note, so we store the next note in a new index
    
    for (int div = 0; div < chordProgressionLength; div++)
    {
        noteSequence[CHANNEL_BASS][noteSeqIndex].note_timestamp = div * chord_length;
        noteSequence[CHANNEL_BASS][noteSeqIndex].note_channel = bassChannel;
        noteSequence[CHANNEL_BASS][noteSeqIndex].note_number = (root_note + Theory::mode[scale_to_use][Theory::chordProgression[pads_chord_prog_to_use][div]]) - 24;
        noteSequence[CHANNEL_BASS][noteSeqIndex].note_velocity = bass_main_velocity;
        noteSequence[CHANNEL_BASS][noteSeqIndex].note_length = bass_note_division; //FIXME: Add bass notelength slider?
        noteSeqIndex++;
    }
}

void MidiGenerator::drumSequencer (){
    Random randomGen(Time::currentTimeMillis());
    int noteSeqIndex = 0; //this needs to be iterated after everytime we add a note, so we store the next note in a new index
    
    //see if we need to repeat the pattern based on the currently set sequence length
    int repeat_val = sequenceStepSize / drumPatternLength;
    
    for (int step = 0; step < drumPatternLength; step++)
    {
        for (int repeat = 0; repeat < repeat_val; repeat++)
        {
            //add kick pattern
            
            if (DrumPatterns::kickPattern[drums_kick_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_timestamp = step + (repeat * drumPatternLength);
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_channel = percussionChannel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_number = kickNote;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_velocity = vel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_length = 4;
                
                noteSeqIndex++;
            }
            
            //add snare pattern
            
            if (DrumPatterns::snarePattern[drums_snare_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                if (vel > 127)
                    vel = 127;
                
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_timestamp = step + (repeat * drumPatternLength);
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_channel = percussionChannel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_number = snareNote;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_velocity = vel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
            if (DrumPatterns::perc1Pattern[drums_perc1_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                if (vel > 127)
                    vel = 127;
                
                
                int step_num = step + drums_offbeat_value;
                if (step_num >= sequenceStepSize)
                    step -= sequenceStepSize;
                
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_timestamp = step + (repeat * drumPatternLength);
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_channel = percussionChannel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_number = percussion1Note;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_velocity = vel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
            if (DrumPatterns::perc2Pattern[drums_perc2_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                if (vel > 127)
                    vel = 127;
                
                //change the step number based on the offbeat value
                int step_num = step + drums_offbeat_value;
                if (step_num >= sequenceStepSize)
                    step -= sequenceStepSize;
                
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_timestamp = step + (repeat * drumPatternLength);
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_channel = percussionChannel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_number = percussion2Note;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_velocity = vel;
                noteSequence[CHANNEL_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
        }
        
    }
    
}

void MidiGenerator::harmonySequencer (){
    
    //Create a new scale here that expands on the set scale
    Array<int>pad_note_buffer;
    
    //populate buffer
    for (int oct = 0; oct < 5; oct++)
    {
        for (int scale_index = 0; scale_index < scaleLength; scale_index++)
        {
            pad_note_buffer.add (root_note + Theory::mode[scale_to_use][scale_index] + (oct * 12));
            //note that this note buffer starts from 0, unlike the melody one that starts from root note
        }
    }
    
    int chord_length = sequenceStepSize / chordProgressionLength;
    int noteSeqIndex = 0; //this needs to be iterated after everytime we add a note, so we store the next note in a new index
    
    for (int div = 0; div < chordProgressionLength; div++)
    {
        for (int chord = 0; chord < pads_density; chord++)
        {
            //set the note to be the global root note
            int note_number = pad_note_buffer[(Theory::chordProgression[pads_chord_prog_to_use][div]) + (chord * pads_chord_interval)];
            
            //add on the index of the note in the scale relating to current chord progression position.
            //note_number += pad_note_buffer[Theory::chordProgression[pads_chord_prog_to_use][div]];
            //add on the note to the chord with the set interval (interval used as scale index, not note number).
            noteSequence[CHANNEL_HARMONY][noteSeqIndex].note_timestamp = div * chord_length;
            noteSequence[CHANNEL_HARMONY][noteSeqIndex].note_channel = harmonyChannel;
            noteSequence[CHANNEL_HARMONY][noteSeqIndex].note_number = note_number;
            noteSequence[CHANNEL_HARMONY][noteSeqIndex].note_velocity = pads_main_velocity;
            noteSequence[CHANNEL_HARMONY][noteSeqIndex].note_length = chord_length;
            noteSeqIndex++;
            
        }
    }
}

void MidiGenerator::melodySequencer (){
    Random randomGen(Time::currentTimeMillis());
    int noteSeqIndex = 0;
    int rest_counter = 0;
    Array<int>mel_note_buffer;
    int mel_note_buffer_size = scaleLength * melody_num_of_octaves;
    
    //populate buffer
    for (int oct = 0; oct < melody_num_of_octaves; oct++)
    {
        for (int scale_index = 0; scale_index < scaleLength; scale_index++)
        {
            mel_note_buffer.add (root_note + Theory::mode[scale_to_use][scale_index] + (oct * 12));
        }
    }
    
    //add notes to sequence array
    //iterate for loop by the set noteLength (so it doesn't just go up in 1's and end up being placed on every step)
    for (int i = 0; i < sequenceStepSize; i += noteLength[melody_note_frequency])
    {
        if (rest_counter != melody_note_rest_freq)
        {
            //get a random note from note_buffer
            int note_index = randomGen.nextInt(mel_note_buffer_size-1);
            int note_number = mel_note_buffer[note_index];
            
            //generate random velocity value based on main velocity and velocity offset
            int vel = melody_main_velocity + randomGen.nextInt(melody_velocity_offset);
            if (vel > 127)
                vel = 127;
            
            //add notes to sequence
            noteSequence[CHANNEL_MELODY][noteSeqIndex].note_timestamp = i;
            noteSequence[CHANNEL_MELODY][noteSeqIndex].note_channel = melodyChannel;
            noteSequence[CHANNEL_MELODY][noteSeqIndex].note_number = note_number;
            noteSequence[CHANNEL_MELODY][noteSeqIndex].note_velocity = vel;
            noteSequence[CHANNEL_MELODY][noteSeqIndex].note_length = melody_note_length;
            noteSeqIndex++;
            rest_counter++;
        }
        else
        {
            rest_counter = 0;
        }
        
        
    }
}

//Sort and arrange notes for output
void MidiGenerator::sequencerSync(){
    NoteData temp;
    
    // element to be compared
    for (int i = 0; i < (sequenceNoteSize - 1); i++)
    {
        // rest of the elements
        for (int j = (i + 1); j < sequenceNoteSize; j++)
        {
            // sort each layer
            for (int layer = 0; layer < instrumentCount; layer++)
            {
                // ascending order
                if (noteSequence[layer][i].note_timestamp > noteSequence[layer][j].note_timestamp)
                {
                    //swap
                    temp = noteSequence[layer][i];
                    noteSequence[layer][i] = noteSequence[layer][j];
                    noteSequence[layer][j] = temp;
                    
                }
            }
            
        }
        
    }
    
    //get the position of the first actual note in each array
    for (int layer = 0; layer < instrumentCount; layer++)
    {
        for (int note = 0; note < sequenceNoteSize; note++)
        {
            if (noteSequence[layer][note].note_timestamp != nullNote)
            {
                startArrayPos[layer] = note;
                break;
            }
            
        }
    }
}

void MidiGenerator::sendMidiMessage (MidiMessage midiMessage)
{
    if (midiOutputDevice != nullptr)
    {
        midiOutputDevice->sendMessageNow(midiMessage);
    }
}


void MidiGenerator::setMidiOutputDevice (int deviceListIndex)
{
    if (midiOutputDevice != nullptr)
        midiOutputDevice->stopBackgroundThread();
    
    if (deviceListIndex == virtualMidiOuputDeviceIndex)
    {
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
        //Create a virtual MIDI output device
        midiOutputDevice = MidiOutput::createNewDevice("Main");
#endif
    }
    else
    {
        //Open a system MIDI device
        midiOutputDevice = MidiOutput::openDevice(midiDeviceInfoArray[deviceListIndex].identifier);
    }
    
    if(midiOutputDevice)
        midiOutputDevice->startBackgroundThread();
    else
        std::cout << "Failed to create or open a MIDI output device!" << std::endl;
    
}

StringArray MidiGenerator::getMidiOutputListNames()
{
    StringArray midiOutputList;
    
    for (auto i = 0; i < midiDeviceInfoArray.size(); i++)
    {
        midiOutputList.add (midiDeviceInfoArray[i].name);
    }
    
    
    
    
    
    
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
    midiOutputList.add ("Virtual Output");
#endif
    
    return midiOutputList;
}


//Updates values if a slider has been moved
void MidiGenerator::updateParams(){
    String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    root_note=vt.getChildWithName("root_note").getProperty("root_note");
    scale_to_use=vt.getChildWithName("scale_to_use").getProperty("scale_to_use");
    tempo=vt.getChildWithName("tempo").getProperty("tempo");

    
    
    melody_num_of_octaves=vt.getChildWithName("melody_num_of_octaves").getProperty("melody_num_of_octaves");
    melody_main_velocity=vt.getChildWithName("melody_main_velocity").getProperty("melody_main_velocity");
    melody_velocity_offset=vt.getChildWithName("melody_velocity_offset").getProperty("melody_velocity_offset");
    melody_note_length=vt.getChildWithName("melody_note_length").getProperty("melody_note_length");
    melody_note_frequency=vt.getChildWithName("melody_note_frequency").getProperty("melody_note_frequency");
    melody_note_rest_freq=vt.getChildWithName("melody_note_rest_freq").getProperty("melody_note_rest_freq");
    
    pads_chord_prog_to_use=vt.getChildWithName("pads_chord_prog_to_use").getProperty("pads_chord_prog_to_use");
    pads_note_length=vt.getChildWithName("pads_note_length").getProperty("pads_note_length");
    pads_density=vt.getChildWithName("pads_density").getProperty("pads_density");
    pads_main_velocity=vt.getChildWithName("pads_main_velocity").getProperty("pads_main_velocity");
    pads_chord_interval=vt.getChildWithName("pads_chord_interval").getProperty("pads_chord_interval");
    
    bass_main_velocity=vt.getChildWithName("bass_main_velocity").getProperty("bass_main_velocity");
    bass_note_division=vt.getChildWithName("bass_note_division").getProperty("bass_note_division");
    
    drums_kick_pattern_to_use=vt.getChildWithName("drums_kick_pattern_to_use").getProperty("drums_kick_pattern_to_use");
    drums_snare_pattern_to_use=vt.getChildWithName("drums_snare_pattern_to_use").getProperty("drums_snare_pattern_to_use");
    drums_perc1_pattern_to_use=vt.getChildWithName("drums_perc1_pattern_to_use").getProperty("drums_perc1_pattern_to_use");
    drums_perc2_pattern_to_use=vt.getChildWithName("drums_perc2_pattern_to_use").getProperty("drums_perc2_pattern_to_use");
    drums_offbeat_value=vt.getChildWithName("drums_offbeat_value").getProperty("drums_offbeat_value");
    drums_main_velocity=vt.getChildWithName("drums_main_velocity").getProperty("drums_main_velocity");
    drums_velocity_offset=vt.getChildWithName("drums_velocity_offset").getProperty("drums_velocity_offset");
}

//Initialise tree on first launch
void MidiGenerator::createRootValueTree()
{
    File file = File::getCurrentWorkingDirectory().getChildFile ("params.xml");
    
    auto vt = vtree.createTree ("Parameters", 0);
    vt.appendChild (vtree.createTree ("tempo", tempo), nullptr);
    
    vt.appendChild (vtree.createTree ("root_note", root_note), nullptr);
    vt.appendChild (vtree.createTree ("scale_to_use", scale_to_use), nullptr);
    vt.appendChild (vtree.createTree ("melody_num_of_octaves", melody_num_of_octaves), nullptr);
    vt.appendChild (vtree.createTree ("melody_main_velocity",melody_main_velocity), nullptr);
    vt.appendChild (vtree.createTree ("melody_velocity_offset",melody_velocity_offset), nullptr);
    vt.appendChild (vtree.createTree ("melody_note_length",melody_note_length), nullptr);
    vt.appendChild (vtree.createTree ("melody_note_frequency",melody_note_frequency), nullptr);
    vt.appendChild (vtree.createTree ("melody_note_rest_freq",melody_note_rest_freq), nullptr);
    
    vt.appendChild (vtree.createTree ("pads_chord_prog_to_use", pads_chord_prog_to_use), nullptr);
    vt.appendChild (vtree.createTree ("pads_note_length", pads_note_length), nullptr);
    vt.appendChild (vtree.createTree ("pads_density",pads_density), nullptr);
    vt.appendChild (vtree.createTree ("pads_main_velocity",pads_main_velocity), nullptr);
    vt.appendChild (vtree.createTree ("pads_chord_interval",pads_chord_interval), nullptr);
    
    vt.appendChild (vtree.createTree ("bass_main_velocity", bass_main_velocity), nullptr);
    vt.appendChild (vtree.createTree ("bass_note_division",bass_note_division), nullptr);
    
    vt.appendChild (vtree.createTree ("drums_kick_pattern_to_use", drums_kick_pattern_to_use), nullptr);
    vt.appendChild (vtree.createTree ("drums_snare_pattern_to_use",drums_snare_pattern_to_use), nullptr);
    vt.appendChild (vtree.createTree ("drums_perc1_pattern_to_use",drums_perc1_pattern_to_use), nullptr);
    vt.appendChild (vtree.createTree ("drums_perc2_pattern_to_use",drums_perc2_pattern_to_use), nullptr);
    vt.appendChild (vtree.createTree ("drums_offbeat_value",drums_offbeat_value), nullptr);
    vt.appendChild (vtree.createTree ("drums_main_velocity",drums_main_velocity), nullptr);
    vt.appendChild (vtree.createTree ("drums_velocity_offset",drums_velocity_offset), nullptr);
    
    vtree.saveValueTreeToXml(vt,file);
}

