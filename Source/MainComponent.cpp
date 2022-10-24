/*
 ==============================================================================
 MainComponent.cpp
 Created: 20 May 2022
 Author:  Rithwik Sathyan
 Main Application screen
 ==============================================================================
 */



#include "MainComponent.h"
#include "ValueTree.h"



//==============================================================================
MainContentComponent::MainContentComponent()
{   //Used when selecting root note and scale
    const StringArray notes = {"C", "C#", "D", "D#","E","F","F#","G","G#","A","A#","B"};
    const StringArray scales = {"Ionian", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Aolian", "Locrian"};
    
    //Set up
    setLookAndFeel(new LookAndFeel_V4 (LookAndFeel_V4::getGreyColourScheme()));
    midiGenerator = std::make_unique<MidiGenerator>();
    midiGenerator->createRootValueTree();
    addAndMakeVisible(m_Controls);
    
    //GUI Elements
    addAndMakeVisible(&playButton);
    playButton.setClickingTogglesState(true);
    playButton.setButtonText("Play");
    playButton.addListener(this);
    playButton.setColour(TextButton::buttonColourId, Colours::wheat);
    playButton.setColour(TextButton::buttonOnColourId, Colours::red);
    playButton.setColour(TextButton::textColourOnId, Colours::white);
    playButton.setColour(TextButton::textColourOffId, Colours::black);
    
    playButton.setEnabled(true);
    playButton.setAlpha(1.0);
    
    addAndMakeVisible(&randomiseButton);
    randomiseButton.setClickingTogglesState(true);
    randomiseButton.setButtonText("Randomise");
    randomiseButton.setColour(TextButton::buttonColourId, Colours::wheat);
    randomiseButton.setColour(TextButton::buttonOnColourId, Colours::wheat);
    randomiseButton.setColour(TextButton::textColourOnId, Colours::black);
    randomiseButton.setColour(TextButton::textColourOffId, Colours::black);
    randomiseButton.addListener(this);
    randomiseButton.setEnabled(true);
    randomiseButton.setAlpha(1.0);
    
    addAndMakeVisible(&rootNoteComboBox);
    rootNoteComboBox.addListener(this);
    rootNoteComboBox.addItemList(notes, 1);
    rootNoteComboBox.setSelectedItemIndex (0, sendNotification);
    
    addAndMakeVisible(&scaleComboBox);
    scaleComboBox.addListener(this);
    scaleComboBox.addItemList(scales, 1);
    scaleComboBox.setSelectedItemIndex (0, sendNotification);
    
    tempoSlider.reset (new Slider ("Tempo"));
    addAndMakeVisible (tempoSlider.get());
    tempoSlider->setRange (50, 100, 1);
    tempoSlider->setDoubleClickReturnValue(true, 80);
    
    tempoSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    tempoSlider->setColour (Slider::thumbColourId, Colour (0xff0a2463));
    tempoSlider->addListener (this);
    tempoSlider->onValueChange = [this]() {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        int i = 75-tempoSlider->getValue();
        vt.getChildWithName("tempo").setProperty("tempo", (tempoSlider->getValue()+(2*i)), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    };
    
    
    addAndMakeVisible(&rootNoteLabel);
    rootNoteLabel.setText("Root Note:", dontSendNotification);
    rootNoteLabel.setColour(Label::textColourId, Colours::wheat);
    rootNoteLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible(&scaleLabel);
    scaleLabel.setText("Mode:", dontSendNotification);
    scaleLabel.setColour(Label::textColourId, Colours::wheat);
    scaleLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible(&tempoLabel);
    tempoLabel.setText("Tempo:", dontSendNotification);
    tempoLabel.setColour(Label::textColourId, Colours::wheat);
    tempoLabel.setJustificationType(Justification::centred);
    
    
    addAndMakeVisible(&midiOutputComboBox);
    midiOutputComboBox.addListener(this);
    auto midiOutputDeviceListNames = midiGenerator->getMidiOutputListNames();
    
    for (int i = 0; i < midiOutputDeviceListNames.size(); i++)
    {
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
        if (i == midiOutputDeviceListNames.size() - 1)
            midiOutputComboBox.addSeparator();
#endif
        midiOutputComboBox.addItem (midiOutputDeviceListNames[i], i + 1);
    }
    
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
    midiOutputComboBox.setSelectedItemIndex (midiOutputComboBox.getNumItems() - 1, sendNotification);
#else
    midiOutputComboBox.setSelectedItemIndex (0, sendNotification);
#endif
    
    
    addAndMakeVisible(&midiOutputLabel);
    midiOutputLabel.setText("MIDI Ouput:", dontSendNotification);
    midiOutputLabel.setColour(Label::textColourId, Colours::wheat);
    midiOutputLabel.setJustificationType(Justification::centred);
    
    //Application window size
    setSize (800, 560);
    cycleCount = 2;
    
    //Animation refresh rate
    startTimerHz (60);
    
    
    
    midiGenerator->setNoteCallback([this](float channel, float note)
                                   {
        noteCallback(channel, note);
    });
    
    midiGenerator->setNoteOffCallback([this](float channel, float note)
                                      {
        noteOffCallback(channel, note);
    });
    
}

MainContentComponent::~MainContentComponent()
{}

void MainContentComponent::paint (Graphics& g)
{
 
    g.fillAll (Colours::transparentBlack);
}

void MainContentComponent::resized()
{
    playButton.setBounds((getWidth()/2) +10, getHeight() - 60, 100, 40);
    randomiseButton.setBounds((getWidth()/2) - 150, getHeight() - 60, 100, 40);
    midiOutputComboBox.setBounds (getWidth() - 160, getHeight() - 35, 150, 20);
    midiOutputLabel.setBounds (getWidth() - 260, getHeight() - 35, 100, 20);
    rootNoteComboBox.setBounds (getWidth() - 160, getHeight() - 65, 150, 20);
    rootNoteLabel.setBounds (getWidth() - 260, getHeight() - 65, 100, 20);
    scaleComboBox.setBounds (60, getHeight() - 65, 150, 20);
    scaleLabel.setBounds (-20, getHeight() - 65, 100, 20);
    tempoSlider->setBounds(60, getHeight() - 40, 160, 30);
    tempoLabel.setBounds (-20, getHeight() - 35, 100, 20);
}

void MainContentComponent::buttonClicked (Button *button)
{
    if (button == &playButton)
    {
        if (button->getToggleState())
        {   playInput();
            midiGenerator->startThread();
            button->setButtonText("Stop");
        }
        else
        {
            midiGenerator->stopThread(500);
            button->setButtonText("Play");
            balls.clear();
        }
    }
    
    else if (button == &randomiseButton)
    {
        
        midiGenerator->stopThread(500);
        randomiseInput();
        button->setButtonText("Randomised");
        playButton.setToggleState(false, NULL);
        playButton.setButtonText("Play");
        balls.clear();
        
        
        
    }
    
}

void MainContentComponent::comboBoxChanged (ComboBox* comboBox)
{
    if (comboBox == &midiOutputComboBox)
    {
        midiGenerator->setMidiOutputDevice (comboBox->getSelectedItemIndex());
    }
    
    else if (comboBox == &rootNoteComboBox)
    {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("root_note").setProperty("root_note", comboBox->getSelectedItemIndex()+36, nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    }
    
    else if (comboBox == &scaleComboBox)
    {
        const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
        File ptfFile = pathToFile;
        VTree vtree;
        ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
        vt.getChildWithName("scale_to_use").setProperty("scale_to_use", comboBox->getSelectedItemIndex(), nullptr);
        vtree.saveValueTreeToXml(vt, ptfFile);
    }
}


//Responds to Play Button press
void MainContentComponent::playInput()
{
    midiGenerator->initSequenceData();
    m_Controls.updateParams();
    midiGenerator->setSequenceData();
}

//Responds to Randomise Button press
void MainContentComponent::randomiseInput()
{
    midiGenerator->randomSequenceData();
    m_Controls.updateParams();
    midiGenerator->setSequenceData();
    updateRoot();
    updateTempo();
}


//Update ball animation
void MainContentComponent::timerCallback()
{
    for (int i = balls.size(); --i >= 0;)
        if (! balls.getUnchecked (i)->step())
            balls.remove (i);
}



void MainContentComponent::noteCallback(float channel, float note)
{
    
    juce::MessageManager::Lock managerLock;
    
    // Generate new balls when a Melody note is detected
    if (Random::getSystemRandom().nextInt (100) < 100){
        if(balls.size()>20){
            managerLock.enter();
            balls.remove(0);
            managerLock.exit();
        }
        if(channel == 1){
            managerLock.enter();
            addAndMakeVisible (balls.add(new BallComponent({(Random::getSystemRandom().nextFloat())*getWidth(),(Random::getSystemRandom().nextFloat())*getHeight()})));
            managerLock.exit();
        }
    }
}

//Clear ball after the note has been killed
void MainContentComponent::noteOffCallback(float channel, float note)
{
    if(channel==1){
        juce::MessageManager::Lock managerLock;
        managerLock.enter();
        balls.remove(0);
        managerLock.exit();
    }
}


//Updates Root Note and scale combobox after randomise
void MainContentComponent::updateRoot()
{
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    int root_note=vt.getChildWithName("root_note").getProperty("root_note");
    rootNoteComboBox.setSelectedItemIndex (root_note-36, sendNotification);
    int scale_to_use=vt.getChildWithName("scale_to_use").getProperty("scale_to_use");
    scaleComboBox.setSelectedItemIndex (scale_to_use, sendNotification);
}

//Updates Tempo slider after randomise
void MainContentComponent::updateTempo()
{
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    int tempo=vt.getChildWithName("tempo").getProperty("tempo");
    tempoSlider->setValue(tempo);
}

void MainContentComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile;
    VTree vtree;
    ValueTree vt = vtree.loadValueTreeFromXml(ptfFile);
    
    
    if (sliderThatWasMoved == tempoSlider.get())
    {
        vt.setProperty ("tempo", tempoSlider->getValue(), nullptr);
    }
    
    
}
