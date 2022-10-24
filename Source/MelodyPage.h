
#include <JuceHeader.h>


class MelodyPage  : public Component,
                  public Slider::Listener
{
public:
    MelodyPage ();
    ~MelodyPage() override;



    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    
    void updateparams();



private:

  


    std::unique_ptr<Slider> melodyOctaveSlider;
    juce::Label  melodyOctaveLabel;
    std::unique_ptr<Slider> melodyMainVelocitySlider;
    juce::Label  melodyMainVelocityLabel;
    std::unique_ptr<Slider> melodyOffsetVelocitySlider;
    juce::Label  melodyOffsetVelocityLabel;
    std::unique_ptr<Slider> melodyNoteLengthSlider;
    juce::Label  melodyNoteLengthLabel;
    std::unique_ptr<Slider> melodyNoteFrequencySlider;
    juce::Label  melodyNoteFrequencyLabel;
    std::unique_ptr<Slider> melodyRestFrequencySlider;
    juce::Label  melodyRestFrequencyLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MelodyPage)
};
