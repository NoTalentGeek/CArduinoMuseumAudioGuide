#include                                "VS1053Object.h"

#define                                 PIN_BREAKOUT_CS              10                         /*VS1053 chip select                pin.*/
#define                                 PIN_BREAKOUT_DCS             8                          /*VS1053 data/command select        pin.*/
#define                                 PIN_BREAKOUT_RESET           9                          /*VS1053 reset                      pin.*/
#define                                 PIN_CARD_CS                  4                          /*VS1053 card chip select           pin.*/
#define                                 PIN_DREQ                     3                          /*VS1053 data request, interrupt    pin.*/
#define                                 PIN_SHIELD_CS                7                          /*VS1053 chip select                pin.*/
#define                                 PIN_SHIELD_DCS               6                          /*VS1053 data/command select        pin.*/
#define                                 PIN_SHIELD_RESET            -1                          /*VS1053 reset                      pin.*/

unsigned    int                         volumeInt                   = 20;                       /*Volume                                   of the VS1053.*/
            int                         volumeResInt                = 5 ;                       /*Volume resolution per incrrease/decrease of the VS1053.*/
            int                         volumeMaxInt                = 0 ;
            int                         volumeMinInt                = 40;
            Adafruit_VS1053_FilePlayer  filePlayerObject            =
                                            Adafruit_VS1053_FilePlayer(
                                                PIN_BREAKOUT_RESET  ,
                                                PIN_BREAKOUT_CS     ,
                                                PIN_BREAKOUT_DCS    ,
                                                PIN_DREQ            ,
                                                PIN_CARD_CS
                                            );

                            VS1053Object::VS1053Object                  (){}
bool                        VS1053Object::GetPlayingMusicBool           (){ return filePlayerObject.playingMusic; }
void                        VS1053Object::SetupVoid                     (){

                                if(!filePlayerObject.begin()) {
                                    Serial.println                      ("VS1053 FAILED TO INITIALIZE, PLEASE CHECK PINS CONNECTION.");
                                    while                               (1);
                                }
                                filePlayerObject                        .useInterrupt   (VS1053_FILEPLAYER_PIN_INT  );
                                filePlayerObject                        .setVolume      (volumeInt, volumeInt       );

                                if(!SD.begin(PIN_CARD_CS))              {
                                    Serial.println                      ("SD CARD SLOT FAILED TO INITIALIZE, PLEASE CHECK PINS CONNECTION.");
                                    while                               (1);
                                }

}
void                        VS1053Object::VolumeVoid                    (){

                                if(volumeInt <= volumeMaxInt)           { volumeInt = volumeMaxInt; }
                                if(volumeInt >= volumeMinInt)           { volumeInt = volumeMinInt; }
                                filePlayerObject.setVolume              ( volumeInt, volumeInt);

}
void                        VS1053Object::VolumeDownVoid                (){ volumeInt = volumeInt + volumeResInt; VolumeVoid(); }
void                        VS1053Object::VolumeUpVoid                  (){ volumeInt = volumeInt - volumeResInt; VolumeVoid(); }
Adafruit_VS1053_FilePlayer& VS1053Object::GetAdafruitVS1053FilePlayer   (){ return filePlayerObject; }
