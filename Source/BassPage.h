
#pragma once

#include <JuceHeader.h>



class BassPage  : public Component,
                  public Slider::Listener
{
public:
    //==============================================================================
    BassPage ();
    ~BassPage() override;
    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void updateparams();

private:
    std::unique_ptr<Slider> bassMainVelocitySlider;
    juce::Label  bassMainVelocityLabel;
    std::unique_ptr<Slider> bassNoteDivisionSlider;
    juce::Label  bassNoteDivisionLabel;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassPage)
};

