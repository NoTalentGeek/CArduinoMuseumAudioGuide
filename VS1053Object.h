#ifndef         VS1053Object_h
#define         VS1053Object_h
#include        "Arduino.h"
#include        <Adafruit_VS1053.h>
#include        <SD.h>
#include        <SPI.h>

class       VS1053Object{

            public:
                                            VS1053Object                ();
                bool                        GetPlayingMusicBool         ();
                char*                       AudioFileIndexCharArray     (int &_audioFileIndexInt);
                void                        SetupVoid                   ();
                void                        VolumeDownVoid              ();
                void                        VolumeUpVoid                ();
                void                        VolumeVoid                  ();
                int 						GetVolumeInt 				();
                Adafruit_VS1053_FilePlayer& GetAdafruitVS1053FilePlayer ();

};
#endif
