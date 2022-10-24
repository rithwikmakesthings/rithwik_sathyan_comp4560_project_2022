/*
 ==============================================================================
 BassPage.cpp
 Created: 13 May 2022 2:18:53am
 Author:  Rithwik Sathyan
 Bass GUI Page
 ==============================================================================
 */
#include "BassPage.h"
#include "MidiGenerator.h"
#include "MainComponent.h"



BassPage::BassPage ()
{
    //Sliders
    bassMainVelocitySlider.reset (new Slider ("Velocity"));
    addAndMakeVisible (bassMainVelocitySlider.get());
    bassMainVelocitySlider->setRange (30, 127, 1);
    if (auto* mc = dynamic_cast<MainContentComponent*>(getParentComponent()))
        bassMainVelocitySlider->setValue(mc->midiGenerator->bass_main_velocity);
    
    
    bassMainVelocitySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    bassMainVelocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    bassMainVelocitySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    bassMainVelocitySlider->addListener (this);
    bassMainVelocitySlider->setBounds (240, 40, 150, 150);
    bassMainVelocitySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("bass_main_velocity").setProperty("bass_main_velocity", bassMainVelocitySlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    
    bassNoteDivisionSlider.reset (new Slider ("Note Division"));
    addAndMakeVisible (bassNoteDivisionSlider.get());
    bassNoteDivisionSlider->setRange (1, 8, 1);
    bassNoteDivisionSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    bassNoteDivisionSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    bassNoteDivisionSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    bassNoteDivisionSlider->addListener (this);
    bassNoteDivisionSlider->setBounds (390, 40, 150, 150);
    bassNoteDivisionSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("bass_note_division").setProperty("bass_note_division", bassNoteDivisionSlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    setSize (640, 480);
    
    
}

BassPage::~BassPage()
{
    bassMainVelocitySlider = nullptr;
    bassNoteDivisionSlider = nullptr;
    
}

//==============================================================================
void BassPage::paint (Graphics& g)
{
    
    const Colour pastel1              { 0xff2d5b6b };
    g.fillAll (pastel1);
    
    
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
        String text (TRANS("Note Division"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
}

void BassPage::resized()
{
    
}

void BassPage::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == bassMainVelocitySlider.get())
    {
    }
    else if (sliderThatWasMoved == bassNoteDivisionSlider.get())
    {
    }
}


void BassPage::updateparams(){
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    
    bassMainVelocitySlider->setValue(vt.getChildWithName("bass_main_velocity").getProperty("bass_main_velocity"));
    bassNoteDivisionSlider->setValue(vt.getChildWithName("bass_note_division").getProperty("bass_note_division"));
    
}
