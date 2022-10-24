/*
 ==============================================================================
 HarmonyPage.cpp
 Created: 13 May 2022 2:18:53am
 Author:  Rithwik Sathyan
 Harmony GUI Page
 ==============================================================================
 */



#include "HarmonyPage.h"
#include "MidiGenerator.h"




//==============================================================================
HarmonyPage::HarmonyPage ()
{
    padsChordSlider.reset (new Slider ("amp sustain"));
    addAndMakeVisible (padsChordSlider.get());
    padsChordSlider->setRange (0, chordProgressionCount-1, 1);
    padsChordSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    padsChordSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    padsChordSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    padsChordSlider->addListener (this);
    padsChordSlider->setBounds (90, 40, 150, 150);
    
    padsNoteLengthSlider.reset (new Slider ("amp release"));
    addAndMakeVisible (padsNoteLengthSlider.get());
    padsNoteLengthSlider->setRange (16, 64, 1);
    padsNoteLengthSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    padsNoteLengthSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    padsNoteLengthSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    padsNoteLengthSlider->addListener (this);
    padsNoteLengthSlider->setBounds (240, 40, 150, 150);
    
    padsChordSizeSlider.reset (new Slider ("amp decay"));
    addAndMakeVisible (padsChordSizeSlider.get());
    padsChordSizeSlider->setRange (1, 4, 1);
    padsChordSizeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    padsChordSizeSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    padsChordSizeSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    padsChordSizeSlider->addListener (this);
    padsChordSizeSlider->setBounds (390, 40, 150, 150);
    
    padsMainVelocitySlider.reset (new Slider ("amp sustain"));
    addAndMakeVisible (padsMainVelocitySlider.get());
    padsMainVelocitySlider->setRange (30, 127, 1);
    padsMainVelocitySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    padsMainVelocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    padsMainVelocitySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    padsMainVelocitySlider->addListener (this);
    padsMainVelocitySlider->setBounds (540, 40, 150, 150);
    
    padsChordIntervalSlider.reset (new Slider ("amp release"));
    addAndMakeVisible (padsChordIntervalSlider.get());
    padsChordIntervalSlider->setRange (1, 4, 1);
    padsChordIntervalSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    padsChordIntervalSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    padsChordIntervalSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    padsChordIntervalSlider->addListener (this);
    padsChordIntervalSlider->setBounds (90, 226, 150, 150);
    setSize (640, 480);
    
    
    padsChordSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("pads_chord_prog_to_use").setProperty("pads_chord_prog_to_use", padsChordSlider->getValue(), nullptr);
        
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    padsNoteLengthSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("pads_note_length").setProperty("pads_note_length", padsNoteLengthSlider->getValue(), nullptr);
        
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    padsChordSizeSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("pads_density").setProperty("pads_density", padsChordSizeSlider->getValue(), nullptr);
        
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    padsMainVelocitySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("pads_main_velocity").setProperty("pads_main_velocity", padsMainVelocitySlider->getValue(), nullptr);
        
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    padsChordIntervalSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("pads_chord_interval").setProperty("pads_chord_interval", padsChordIntervalSlider->getValue(), nullptr);
        
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    
    
}

HarmonyPage::~HarmonyPage()
{
    padsChordSlider = nullptr;
    padsNoteLengthSlider = nullptr;
    padsChordSizeSlider = nullptr;
    padsMainVelocitySlider = nullptr;
    padsChordIntervalSlider = nullptr;
    
}

//==============================================================================
void HarmonyPage::paint (Graphics& g)
{
    
    
    g.fillAll (Colours::pastel3);
    
    
    {
        int x = 65, y = 100, width = 200, height = 30;
        String text (TRANS("Harmony Chords"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 215, y = 100, width = 200, height = 30;
        String text (TRANS("Note Length"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 365, y = 100, width = 200, height = 30;
        String text (TRANS("Chord Size"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 515, y = 100, width = 200, height = 30;
        String text (TRANS("Velocity"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 65, y = 286, width = 200, height = 30;
        String text (TRANS("Interval"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    
}

void HarmonyPage::resized()
{
    
}

void HarmonyPage::sliderValueChanged (Slider* sliderThatWasMoved)
{
    
    
    if (sliderThatWasMoved == padsChordSlider.get())
    {
        
    }
    
}


void HarmonyPage::updateparams(){
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    
    padsChordSlider->setValue(vt.getChildWithName("pads_chord_prog_to_use").getProperty("pads_chord_prog_to_use"));
    padsNoteLengthSlider->setValue(vt.getChildWithName("pads_note_length").getProperty("pads_note_length"));
    padsChordSizeSlider->setValue(vt.getChildWithName("pads_density").getProperty("pads_density"));
    padsMainVelocitySlider->setValue(vt.getChildWithName("pads_main_velocity").getProperty("pads_main_velocity"));
    padsChordIntervalSlider->setValue(vt.getChildWithName("pads_chord_interval").getProperty("pads_chord_interval"));
}

