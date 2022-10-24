#pragma once

#include <JuceHeader.h>
#include "DrumPage.h"
#include "BassPage.h"
#include "MelodyPage.h"
#include "HarmonyPage.h"

class MainPage  : public Component
{
public:
    //==============================================================================
    MainPage ();
    ~MainPage() override;

    //==============================================================================

    TabbedComponent* getTabbedComponent() {
        return tabbedComponent.get();
    }

    DrumPage* dp = new DrumPage();
    BassPage* bp = new BassPage();
    HarmonyPage* hp = new HarmonyPage();
    MelodyPage* mp = new MelodyPage();
    
    void paint (Graphics& g) override;
    void resized() override;
    void updateParams();



private:

    //==============================================================================
    std::unique_ptr<TabbedComponent> tabbedComponent;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPage)
};

