
#include <JuceHeader.h>

class DrumPage  : public Component,
                  public Slider::Listener
{
public:
    //==============================================================================
    DrumPage ();
    ~DrumPage() override;

    //==============================================================================


    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void updateparams();



private:

    //==============================================================================

    
  
    
    std::unique_ptr<Slider> drumKickPatternSlider;
    juce::Label  drumKickPatternLabel;
    std::unique_ptr<Slider> drumSnarePatternSlider;
    juce::Label  drumSnarePatternLabel;
    std::unique_ptr<Slider> drumPerc1PatternSlider;
    juce::Label  drumPerc1PatternLabel;
    std::unique_ptr<Slider> drumPerc2PatternSlider;
    juce::Label  drumPerc2PatternLabel;
    std::unique_ptr<Slider> drumOffbeatSlider;
    juce::Label  drumOffbeatLabel;
    std::unique_ptr<Slider> drumMainVelocitySlider;
    juce::Label  drumMainVelocityLabel;
    std::unique_ptr<Slider> drumOffsetVelocitySlider;
    juce::Label  drumOffsetVelocityLabel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumPage)
};

