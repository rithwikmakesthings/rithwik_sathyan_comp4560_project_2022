/*
 ==============================================================================
 DrumPage.cpp
 Created: 13 May 2022 2:18:53am
 Author:  Rithwik Sathyan
 Drum GUI Page
 ==============================================================================
 */
#include "DrumPage.h"
#include "MidiGenerator.h"
#include "Theory.h"

//==============================================================================
DrumPage::DrumPage ()
{
    drumKickPatternSlider.reset (new Slider ("amp sustain"));
    addAndMakeVisible (drumKickPatternSlider.get());
    drumKickPatternSlider->setRange (0, drumPatternCount-1, 1);
    drumKickPatternSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumKickPatternSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumKickPatternSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumKickPatternSlider->addListener (this);
    drumKickPatternSlider->setBounds (90, 40, 150, 150);
    
    drumSnarePatternSlider.reset (new Slider ("amp release"));
    addAndMakeVisible (drumSnarePatternSlider.get());
    drumSnarePatternSlider->setRange (0, drumPatternCount-1, 1);
    drumSnarePatternSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumSnarePatternSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumSnarePatternSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumSnarePatternSlider->addListener (this);
    drumSnarePatternSlider->setBounds (240, 40, 150, 150);
    
    drumPerc1PatternSlider.reset (new Slider ("amp decay"));
    addAndMakeVisible (drumPerc1PatternSlider.get());
    drumPerc1PatternSlider->setRange (0, drumPatternCount-1, 1);
    drumPerc1PatternSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumPerc1PatternSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumPerc1PatternSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumPerc1PatternSlider->addListener (this);
    drumPerc1PatternSlider->setBounds (390, 40, 150, 150);
    
    drumPerc2PatternSlider.reset (new Slider ("amp sustain"));
    addAndMakeVisible (drumPerc2PatternSlider.get());
    drumPerc2PatternSlider->setRange (0, drumPatternCount-1, 1);
    drumPerc2PatternSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumPerc2PatternSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumPerc2PatternSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumPerc2PatternSlider->addListener (this);
    drumPerc2PatternSlider->setBounds (540, 40, 150, 150);
    
    drumOffbeatSlider.reset (new Slider ("amp release"));
    addAndMakeVisible (drumOffbeatSlider.get());
    drumOffbeatSlider->setRange (0, 128, 1);
    drumOffbeatSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumOffbeatSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumOffbeatSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumOffbeatSlider->addListener (this);
    drumOffbeatSlider->setBounds (90, 226, 150, 150);
    
    drumMainVelocitySlider.reset (new Slider ("filter decay"));
    addAndMakeVisible (drumMainVelocitySlider.get());
    drumMainVelocitySlider->setRange (30, 127, 1);
    drumMainVelocitySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumMainVelocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumMainVelocitySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumMainVelocitySlider->addListener (this);
    drumMainVelocitySlider->setBounds (240, 226, 150, 150);
    
    drumOffsetVelocitySlider.reset (new Slider ("filter sustain"));
    addAndMakeVisible (drumOffsetVelocitySlider.get());
    drumOffsetVelocitySlider->setRange (1, 30, 1);
    drumOffsetVelocitySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drumOffsetVelocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    drumOffsetVelocitySlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    drumOffsetVelocitySlider->addListener (this);
    drumOffsetVelocitySlider->setBounds (390, 226, 150, 150);
    
    setSize (640, 480);
    
    drumKickPatternSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_kick_pattern_to_use").setProperty("drums_kick_pattern_to_use", drumKickPatternSlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    drumSnarePatternSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_snare_pattern_to_use").setProperty("drums_snare_pattern_to_use", drumSnarePatternSlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    drumPerc1PatternSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_perc1_pattern_to_use").setProperty("drums_perc1_pattern_to_use", drumPerc1PatternSlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
        
    };
    
    drumPerc2PatternSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_perc2_pattern_to_use").setProperty("drums_perc2_pattern_to_use", drumPerc2PatternSlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
        
        
    };
    
    drumOffbeatSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_offbeat_value").setProperty("drums_offbeat_value", drumOffbeatSlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
        
        
    };
    
    drumMainVelocitySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_main_velocity").setProperty("drums_main_velocity", drumMainVelocitySlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
        
    };
    
    drumOffsetVelocitySlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("drums_velocity_offset").setProperty("drums_velocity_offset", drumOffsetVelocitySlider->getValue(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
        
        
    };
    
}

DrumPage::~DrumPage()
{
    
    
    
    drumSnarePatternSlider = nullptr;
    drumKickPatternSlider = nullptr;
    drumPerc1PatternSlider = nullptr;
    drumPerc2PatternSlider = nullptr;
    drumOffbeatSlider = nullptr;
    drumMainVelocitySlider = nullptr;
    drumOffsetVelocitySlider = nullptr;
    
}

//==============================================================================
void DrumPage::paint (Graphics& g)
{
    
    
    g.fillAll (Colours::pastel2);
    
    
    {
        int x = 65, y = 100, width = 200, height = 30;
        String text (TRANS("Kick Pattern"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 215, y = 100, width = 200, height = 30;
        String text (TRANS("Snare Pattern"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 365, y = 100, width = 200, height = 30;
        String text (TRANS("Perc1"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 515, y = 100, width = 200, height = 30;
        String text (TRANS("Perc2"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 65, y = 286, width = 200, height = 30;
        String text (TRANS("Offbeat"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 215, y = 286, width = 200, height = 30;
        String text (TRANS("Velocity"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    {
        int x = 365, y = 286, width = 200, height = 30;
        String text (TRANS("Velocity Offset"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
    
    
}

void DrumPage::resized()
{
}

void DrumPage::sliderValueChanged (Slider* sliderThatWasMoved)
{
}

void DrumPage::updateparams(){
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    
    drumKickPatternSlider->setValue(vt.getChildWithName("drums_kick_pattern_to_use").getProperty("drums_kick_pattern_to_use"));
    drumSnarePatternSlider->setValue(vt.getChildWithName("drums_snare_pattern_to_use").getProperty("drums_snare_pattern_to_use"));
    drumPerc1PatternSlider->setValue(vt.getChildWithName("drums_perc1_pattern_to_use").getProperty("drums_perc1_pattern_to_use"));
    drumPerc2PatternSlider->setValue(vt.getChildWithName("drums_perc2_pattern_to_use").getProperty("drums_perc2_pattern_to_use"));
    drumOffbeatSlider->setValue(vt.getChildWithName("drums_offbeat_value").getProperty("drums_offbeat_value"));
    drumMainVelocitySlider->setValue(vt.getChildWithName("drums_main_velocity").getProperty("drums_main_velocity"));
    drumOffsetVelocitySlider->setValue(vt.getChildWithName("drums_velocity_offset").getProperty("drums_velocity_offset"));
    
}
