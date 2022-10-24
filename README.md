![oops!](https://github.com/rithwikmakesthings/rithwik_sathyan_comp4560_project_2022/blob/main/Blackbox.jpg?raw=true)
## Getting Started

### Installing

* Clone the repo.
* Download and install [JUCE](https://juce.com/)
  * The software has been tested with Juce version 6.1.6

* Open Blackbox.jucer file with Projucer
  * Open and build project in XCode or Visual Studio.
  * Remember to switch the build target to Release (using all optimizations enables realtime use)
  
  *Important: If the IDE gives a "No member named 'pastel1' in namespace 'juce::Colours'" error at compile time, it is likely that JUCE has been set up to overwrite the project module files. An easy fix for this, (especially if you dont want to dig into JUCE settings) is to add the following lines of code to './JuceLibraryCode/modules/juce_graphics/colour/juce_Colours.h'
  
```cpp
  const Colour pastelblack          { 0xff52494c};
  const Colour pastel1              { 0xff2d5b6b };
  const Colour pastel2              { 0xffc47a53};
  const Colour pastel3              { 0xff8f4731};
  const Colour pastel4              { 0xff7b7d2a };
  const Colour pastel5              { 0xffa2d2ff };
```

### Usage

Blackbox does not output any audio itself, and is designed to let you use it in combination with a Digital Audio Workstation (or any other MIDI Hardware/Software.)

It currently supports generative sequences for four instrument, each of which sends their generated sequences onto a different MIDI channel as follows:
+   Channel 1 - Melody
+   Channel 2 - Harmony
+   Channel 3 - Bass
+   Channel 4 - Percussion

On the DAW/MIDI device of choice, will need to set up four different tracks and initialise them with different software instruments. 
For each track, you then filter out incoming MIDI information by the relevant channel. 

A template for use with Apple's Logic Pro is provided above.

### Demos
+ Usage Demo Video:  https://www.youtube.com/watch?v=dlMYe5MEda8
+ Light Show Demo:   https://www.youtube.com/watch?v=6kybLh5g44A
+ Soundcloud Demo 1: https://soundcloud.com/rithwik-sathyan/blackbox-demo-track
+ Soundcloud Demo 2: https://soundcloud.com/rithwik-sathyan/blackbox-demo-track-dhrune-edition
