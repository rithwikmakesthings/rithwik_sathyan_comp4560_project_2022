#include <MIDI.h>  // Add Midi Library
#include <Adafruit_NeoPixel.h>

#define PIN 6


Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  strip.begin();
  strip.setBrightness(255);
  strip.show();                                                                           


  MIDI.begin(MIDI_CHANNEL_OMNI);                                                                          
  Serial.begin(115200);                                                                   

  MIDI.setHandleNoteOn(MyHandleNoteOn);  
}

void loop() {
  MIDI.read();                                                                            
}


// MyHandleNoteOn/Off are the functions that will be called by the Midi Library
// when a MIDI Note message is received.
// It will be passed bytes for Channel, Pitch, and Value

void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {

 if(channel==1){
 long randNumber;
 long randNumber2;
 randNumber = random(0, 120);
 randNumber2 = random(0, 120);
 strip.setPixelColor(randNumber,Wheel(255)); 
 strip.setPixelColor(randNumber2,Wheel(128)); 
 strip.show();
 delay(0.5);
 strip.setPixelColor(randNumber,(0,0,0)); 
 strip.setPixelColor(randNumber2,(0,0,0)); 
 }
}


uint32_t Wheel(byte WheelPos) {
  if (WheelPos == 255) {
    return strip.Color(255, 255, 255);
  } else if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
