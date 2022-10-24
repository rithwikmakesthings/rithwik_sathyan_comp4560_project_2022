/*
  ==============================================================================
    MelodyPage.cpp
    Created: 13 May 2022 2:18:53am
    Author:  Rithwik Sathyan
    Melody GUI Page
  ==============================================================================
*/



#include "MelodyPage.h"
#include "MidiGenerator.h"
#include "ValueTree.h"

//==============================================================================
MelodyPage::MelodyPage ()
{
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    
    melodyOctaveSlider.reset (new Slider ("amp sustain"));
    addAndMakeVisible (melodyOctaveSlider.get());
    melodyOctaveSlider->setRange (1, 5, 1);
    melodyOctaveSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    melodyOctaveSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    melodyOctaveSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    melodyOctaveSlider->addListener (this);
    melodyOctaveSlider->setBounds (90, 40, 150, 150);

    melodyMainVelocitySlider.reset (new Slider ("amp release"));
    addAndMakeVisible (melodyMainVelocitySlider.get());
    melodyMainVelocitySlider->setRange (30, 127, 1);
    melodyMainVelocitySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    melodyMainVelocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    melodyMainVelocitySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    melodyMainVelocitySlider->addListener (this);
    melodyMainVelocitySlider->setBounds (240, 40, 150, 150);

    melodyOffsetVelocitySlider.reset (new Slider ("amp decay"));
    addAndMakeVisible (melodyOffsetVelocitySlider.get());
    melodyOffsetVelocitySlider->setRange (0, 30, 1);
    melodyOffsetVelocitySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    melodyOffsetVelocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    melodyOffsetVelocitySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    melodyOffsetVelocitySlider->addListener (this);
    melodyOffsetVelocitySlider->setBounds (390, 40, 150, 150);

    melodyNoteLengthSlider.reset (new Slider ("amp sustain"));
    addAndMakeVisible (melodyNoteLengthSlider.get());
    melodyNoteLengthSlider->setRange (1, 16, 1);
    melodyNoteLengthSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    melodyNoteLengthSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    melodyNoteLengthSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    melodyNoteLengthSlider->addListener (this);
    melodyNoteLengthSlider->setBounds (540, 40, 150, 150);

    melodyNoteFrequencySlider.reset (new Slider ("amp release"));
    addAndMakeVisible (melodyNoteFrequencySlider.get());
    melodyNoteFrequencySlider->setRange (0, noteLengthCount-1, 1);
    melodyNoteFrequencySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    melodyNoteFrequencySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    melodyNoteFrequencySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    melodyNoteFrequencySlider->addListener (this);
    melodyNoteFrequencySlider->setBounds (90, 226, 150, 150);
    
    melodyRestFrequencySlider.reset (new Slider ("amp release"));
    addAndMakeVisible (melodyRestFrequencySlider.get());
    melodyRestFrequencySlider->setRange (1, 16, 1);
    melodyRestFrequencySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    melodyRestFrequencySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    melodyRestFrequencySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    melodyRestFrequencySlider->addListener (this);
    melodyRestFrequencySlider->setBounds (240, 226, 150, 150);


    setSize (640, 480);
   

    //[Constructor] You can add your own custom stuff here..
    melodyOctaveSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("melody_num_of_octaves").setProperty("melody_num_of_octaves", melodyOctaveSlider->getValue(), nullptr);

        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    melodyMainVelocitySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("melody_main_velocity").setProperty("melody_main_velocity", melodyMainVelocitySlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    melodyOffsetVelocitySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("melody_velocity_offset").setProperty("melody_velocity_offset", melodyOffsetVelocitySlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    

    melodyNoteLengthSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("melody_note_length").setProperty("melody_note_length", melodyNoteLengthSlider->getValue(), nullptr);

        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    

    melodyNoteFrequencySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("melody_note_frequency").setProperty("melody_note_frequency", melodyNoteFrequencySlider->getValue(), nullptr);

        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    

    melodyRestFrequencySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("melody_note_rest_freq").setProperty("melody_note_rest_freq", melodyRestFrequencySlider->getValue(), nullptr);

        
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    




}

MelodyPage::~MelodyPage()
{

    melodyOctaveSlider = nullptr;
    melodyMainVelocitySlider = nullptr;
    melodyOffsetVelocitySlider = nullptr;
    melodyNoteLengthSlider = nullptr;
    melodyNoteFrequencySlider = nullptr;
    melodyRestFrequencySlider = nullptr;

}

//==============================================================================
void MelodyPage::paint (Graphics& g)
{


    const Colour pastel4              { 0xff7b7d2a };
    g.fillAll (pastel4);


    {
        int x = 65, y = 100, width = 200, height = 30;
        String text (TRANS("Octave"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 215, y = 100, width = 200, height = 30;
        String text (TRANS("Velocity"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 365, y = 100, width = 200, height = 30;
        String text (TRANS("Velocity Offset"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 515, y = 100, width = 200, height = 30;
        String text (TRANS("Note Length"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 65, y = 286, width = 200, height = 30;
        String text (TRANS("Note spacing"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 215, y = 286, width = 200, height = 30;
        String text (TRANS("Rest spacing"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
}

void MelodyPage::resized()
{

}

void MelodyPage::sliderValueChanged (Slider* sliderThatWasMoved)
{
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    

    if (sliderThatWasMoved == melodyOctaveSlider.get())
    {
        vt.setProperty ("melody_num_of_octaves", melodyOctaveSlider->getValue(), nullptr);
    }
    

}

void MelodyPage::updateparams(){
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    melodyMainVelocitySlider->setValue(vt.getChildWithName("melody_main_velocity").getProperty("melody_main_velocity"));
    melodyOffsetVelocitySlider->setValue(vt.getChildWithName("melody_velocity_offset").getProperty("melody_velocity_offset"));
    melodyNoteLengthSlider->setValue(vt.getChildWithName("melody_note_length").getProperty("melody_note_length"));
    melodyNoteFrequencySlider->setValue(vt.getChildWithName("melody_note_frequency").getProperty("melody_note_frequency"));
    melodyRestFrequencySlider->setValue(vt.getChildWithName("melody_note_rest_freq").getProperty("melody_note_rest_freq"));
    melodyOctaveSlider->setValue(vt.getChildWithName("melody_num_of_octaves").getProperty("melody_num_of_octaves"));
}



//==============================================================================
#if 0
#endif




