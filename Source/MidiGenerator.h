/*
 ==============================================================================
 midiGenerator.h
 Created: 9 March 2022
 Author:  Rithwik Sathyan
 Main MIDI Processor
 ==============================================================================
 */

#ifndef midiGenerator_h
#define midiGenerator_h

#include <JuceHeader.h>
#include "Theory.h"
#include "ValueTree.h"


struct NoteData
{   int note_timestamp;
    int note_channel;
    int note_number;
    int note_velocity;
    int note_length;
};

#define instrumentCount 4


enum SequencerSections
{
    CHANNEL_MELODY = 0,
    CHANNEL_HARMONY,
    CHANNEL_BASS,
    CHANNEL_PERC
    
};



#define sequenceStepSize 128 //keep this as a division of 16
#define sequenceNoteSize 512
#define noteOffBuffer 64

#define melodyChannel 1
#define harmonyChannel 2
#define bassChannel 3
#define percussionChannel 4

#define nullNote -1

#define kickNote 36
#define snareNote 38
#define percussion1Note 37
#define percussion2Note 43

typedef std::function<void (float note, float channel)> NodePressCallback;


class MidiGenerator : public Thread
{
    
public:
    //==============================================================================
    
    MidiGenerator();
    ~MidiGenerator();
    
    void setSequenceData();
    void initSequenceData();
    void randomSequenceData();
    //Thread callback function
    void run();
    
    void sendMidiMessage (MidiMessage midiMessage);
    
    void setMidiOutputDevice (int deviceListIndex);
    StringArray getMidiOutputListNames();
    
    void drumSequencer();
    void bassSequencer();
    void melodySequencer();
    void harmonySequencer();
    void sequencerSync();
    
    
    
    //global
    int root_note;
    int scale_to_use;
    
    //melody
    int melody_num_of_octaves;      //controls note range
    int melody_main_velocity;
    int melody_velocity_offset;     //dynamics - a larger offset means a larger range around the main velocity
    int melody_note_length;         //in number of steps
    int melody_note_frequency;
    int melody_note_rest_freq;
    
    //harmony
    int pads_chord_prog_to_use;
    int pads_note_length;           //in number of steps
    int pads_density;               //number of notes in chords
    int pads_main_velocity;
    int pads_chord_interval;
    
    //bass
    int bass_main_velocity;
    int bass_note_division;          //how many 'plucks' a note is played as.
    
    //drums
    int drums_kick_pattern_to_use;
    int drums_snare_pattern_to_use;
    int drums_perc1_pattern_to_use;
    int drums_perc2_pattern_to_use;
    int drums_offbeat_value;         //value to shift patterns by in steps
    int drums_main_velocity;
    int drums_velocity_offset;       //dynamics - a larger offset means a larger range around the main velocity
    
    int tempo;                       //used as a tempo indicator

    
    VTree vtree;
    //==================================================================================
    
    
    //Setters
    void setRoot(int input);
    void setScale(int input);
    
    void updateParams();
    void createRootValueTree();
    void setNoteCallback (NodePressCallback cb) { noteCallback = cb; }
    void setNoteOffCallback (NodePressCallback cb) { noteOffCallback = cb; }
    
    
private:
    
    //MIDI output device setup
    std::unique_ptr<MidiOutput> midiOutputDevice;
    Array <MidiDeviceInfo> midiDeviceInfoArray;
    
    bool firsttime = true;
    
    int virtualMidiOuputDeviceIndex = -1;
    
    //arrays to store sequences of note data
    NoteData noteSequence[instrumentCount][sequenceNoteSize];
    
    int currentArrayPos[instrumentCount];  //stores the current position we are getting notes out of noteSequence for each layer.
    int startArrayPos[instrumentCount];  //stores the first index of noteSequence that stores an actual note
    NodePressCallback noteCallback;
    NodePressCallback noteOffCallback;
    
    
};




#endif 
