
#include <JuceHeader.h>

class HarmonyPage  : public Component,
                  public Slider::Listener
{
public:
    //==============================================================================
    HarmonyPage ();
    ~HarmonyPage() override;

    //==============================================================================


    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void updateparams();



private:
 
    
  

    std::unique_ptr<Slider> padsChordSlider;
    juce::Label  padsChordLabel;
    std::unique_ptr<Slider> padsNoteLengthSlider;
    juce::Label  padsNoteLengthLabel;
    std::unique_ptr<Slider> padsChordSizeSlider;
    juce::Label  padsChordSizeLabel;
    std::unique_ptr<Slider> padsMainVelocitySlider;
    juce::Label  padsMainVelocityLabel;
    std::unique_ptr<Slider> padsChordIntervalSlider;
    juce::Label  padsChordIntervalLabel;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonyPage)
};


