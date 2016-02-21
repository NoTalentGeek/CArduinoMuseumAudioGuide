#include "VS1053Object.h"






#define PIN_BREAKOUT_CS     10  /*VS1053 chip select pin.*/
#define PIN_BREAKOUT_DCS    8   /*VS1053 data/command select pin.*/
#define PIN_BREAKOUT_RESET  9   /*VS1053 reset pin.*/
#define PIN_CARD_CS         4   /*VS1053 card chip select pin.*/
#define PIN_DREQ            3   /*VS1053 data request, interrupt pin.*/






int volumeInt       = 20;       /*Volume of the VS1053.*/
int volumeResInt    = 5 ;       /*Volume resolution per incrrease/decrease of the VS1053.*/
int volumeMaxInt    = 0 ;
int volumeMinInt    = 40;
Adafruit_VS1053_FilePlayer filePlayerObject = Adafruit_VS1053_FilePlayer(
    PIN_BREAKOUT_RESET,
    PIN_BREAKOUT_CS,
    PIN_BREAKOUT_DCS,
    PIN_DREQ,
    PIN_CARD_CS
);





/*==================================================
No need to set constructor.*/
VS1053Object::VS1053Object(){}
/*==================================================*/





/*==================================================
Whether this object is currentlyplaying audio file or not.*/
bool VS1053Object::GetPlayingMusicBool(){ return filePlayerObject.playingMusic; }
/*==================================================*/





/*==================================================
Function to initialize the VS1053 audio player module.*/
void VS1053Object::SetupVoid(){

    /*Check the availability of the VS1053.
    Check if the pin connections is wrong, etc.*/
    if(!filePlayerObject.begin()){

        Serial.println("VS1053 FAILED TO INITIALIZE, PLEASE CHECK PINS CONNECTION.");
        /*Stall this Arduino device if the VS1053 unit cannot be found..*/
        while(1);

    }

    /*Check the availability to access the SD card slot.*/
    if(!SD.begin(PIN_CARD_CS)){

        Serial.println("SD CARD SLOT FAILED TO INITIALIZE, PLEASE CHECK PINS CONNECTION.");
        while(1);

    }

    /*Set the initial volume for this Arduino device.*/
    filePlayerObject.setVolume      (volumeInt, volumeInt       );
    /*Use interrupt so that this device can still execute command while playing
        an audio file.*/
    filePlayerObject.useInterrupt   (VS1053_FILEPLAYER_PIN_INT  );

}
/*==================================================*/





/*==================================================
Function to set volume.
One thing to notice here is that the lower the int value the louder the
    volume become.
For example 0 is the maximum value this Arduino device can achieve.
The higher it is the less volume achieved, like 40 in integer is already make
    this Arduino device' volume not apparent.*/
void VS1053Object::VolumeVoid(){

    if(volumeInt <= volumeMaxInt){ volumeInt = volumeMaxInt; }
    else if(volumeInt >= volumeMinInt){ volumeInt = volumeMinInt; }
    filePlayerObject.setVolume( volumeInt, volumeInt);

}
/*==================================================*/





/*==================================================
Pre - made function to decrease the volume.*/
void VS1053Object::VolumeDownVoid(){ filePlayerObject.sineTest(40, 50); volumeInt = volumeInt + volumeResInt; VolumeVoid(); }
/*==================================================*/





/*==================================================
Pre - made function to increase the volume.*/
void VS1053Object::VolumeUpVoid(){ filePlayerObject.sineTest(60, 50); volumeInt = volumeInt - volumeResInt; VolumeVoid(); }
/*==================================================*/





/*==================================================
Get the current volume of this device in integer.*/
int VS1053Object::GetVolumeInt(){ return volumeInt; }
/*==================================================*/





/*==================================================
Return the object file player.*/
Adafruit_VS1053_FilePlayer& VS1053Object::GetAdafruitVS1053FilePlayer(){ return filePlayerObject; }
/*==================================================*/