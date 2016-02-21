#ifndef VS1053Object_h
#define VS1053Object_h





#include "Arduino.h"
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>





class VS1053Object{

    public:
        Adafruit_VS1053_FilePlayer& GetAdafruitVS1053FilePlayer();
        bool GetPlayingMusicBool();
        char* AudioFileIndexCharArray(int &_audioFileIndexInt);
        int GetVolumeInt();
        void SetupVoid();
        void VolumeDownVoid();
        void VolumeUpVoid();
        void VolumeVoid();
        VS1053Object();

};





#endif
