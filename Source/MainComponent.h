/*
 ==============================================================================
 MainComponent.h
 Created: 20 May 2022
 Author:  Rithwik Sathyan
 Main Application screen
 ==============================================================================
 */


#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED


#include "MidiGenerator.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainPage.h"





//Creates the ball animations that accompany the Melody during play.
struct BallComponent  : public Component

{
    BallComponent (Point<float> pos)
    : position (pos),
    speed (Random::getSystemRandom().nextFloat() *  4.0f - 2.0f,
           Random::getSystemRandom().nextFloat() * -6.0f - 2.0f),
    colour (Colours::white)
    
    {   setAlpha (0.9);
        setSize (20, 20);
        step();
    }
    
    bool step()
    {
        setSize(getHeight()+5, getWidth()+5);
        setAlpha(getAlpha()-0.05);
        
        setCentrePosition ((int) position.x,
                           (int) position.y);
        
        if (auto* parent = getParentComponent())
            return isPositiveAndBelow (position.x, (float) parent->getWidth())
            && position.y < (float) parent->getHeight();
        
        return position.y < 400.0f && position.x >= -10.0f;
    }
    
    void paint (Graphics& g) override
    {
        g.setColour (colour);
        g.fillEllipse (2.0f, 2.0f, (float) getWidth() - 4.0f, (float) getHeight() - 4.0f);
        
        
        g.setColour (Colours::darkgrey);
        g.drawEllipse (2.0f, 2.0f, (float) getWidth() - 4.0f, (float) getHeight() - 4.0f, 1.0f);
    }
    
    Point<float> position, speed;
    Colour colour;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BallComponent)
};

//============================================


class MainContentComponent   :  public Component,
public Button::Listener,
public ComboBox::Listener,
private Timer,
public Slider::Listener
{
public:
    //=========================================
    MainContentComponent();
    ~MainContentComponent();
    
    
    
    //=========================================
    //GUI stuff
    
    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked (Button *button) override;
    void comboBoxChanged (ComboBox* comboBox) override;
    
    //=========================================
    //Backend stuff
    void playInput();
    void randomiseInput();
    void updateTempo();
    void updateRoot();
    
    std::unique_ptr<MidiGenerator> midiGenerator;
    std::unique_ptr<Slider> tempoSlider;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    
private:
    //==========================================
    
    //GUI stuff
    TextButton randomiseButton;
    TextButton playButton;
    ComboBox midiOutputComboBox;
    ComboBox rootNoteComboBox;
    ComboBox scaleComboBox;
    Label midiOutputLabel;
    Label rootNoteLabel;
    Label scaleLabel;
    Label tempoLabel;
    
    MainPage m_Controls;
    
    OwnedArray<Component> componentsToAnimate;
    OwnedArray<BallComponent> balls;
    ComponentAnimator animator;
    int cycleCount;
    bool firstCallback = true;
    void triggerAnimation();
    void timerCallback()override;
    void noteCallback(float channel, float note);
    void noteOffCallback(float channel, float note);
    void addBall(float floatValue);
    
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
#endif


