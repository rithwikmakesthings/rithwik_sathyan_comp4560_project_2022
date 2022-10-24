/*
 ==============================================================================
 MainPage.cpp
 Created: 13 May 2022 2:18:53am
 Author:  Rithwik Sathyan
 GUI Tab Organiser
 ==============================================================================
 */

#include "ValueTree.h"
#include "MainPage.h"



//==============================================================================
MainPage::MainPage ()
{   tabbedComponent.reset (new TabbedComponent (TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabbedComponent.get());
    tabbedComponent->addTab (TRANS("Drums"), Colours::pastel2, dp, true);
    tabbedComponent->setCentreRelative(512, 20);
    tabbedComponent->addTab (TRANS("Bass"), Colours::pastel1, bp, true);
    tabbedComponent->addTab (TRANS("Harmony"), Colours::pastel3, hp, true);
    tabbedComponent->addTab (TRANS("Melody"), Colours::pastel4, mp, true);
    tabbedComponent->setCurrentTabIndex (0);
    tabbedComponent->setBounds (0, 0, 1024, 600);
    
    setSize (1024, 768);
    updateParams();
    
    
}

MainPage::~MainPage()
{
    
    tabbedComponent = nullptr;
    
    
}

//==============================================================================
void MainPage::paint (Graphics& g)
{
    
    
    
    
    
    
}

void MainPage::resized()
{
    
}

//Changes slider positions if the values are forcibly changed, usually as a result of the randomise button being pressed.
void MainPage::updateParams()
{
    mp->updateparams();
    bp->updateparams();
    hp->updateparams();
    dp->updateparams();
}
