#include            "ButtonObject.h"
#include            "RFIDReaderObject.h"
#include            "VS1053Object.h"

#define             PIN_VOLUME_DOWN              22                                 /*Device volume down    button      pin.*/
#define             PIN_VOLUME_UP                24                                 /*Device volume up      button      pin.*/

int                 audioFileIndexInt           = 0;
int                 audioFileIndexMaxInt        = 3;
int                 audioFileIndexMinInt        = 0;
int                 playerIndexInt              = 1;
int                 exhibitionReceiveIndexInt   ;
char                audioFileIndexCharArray[10] ;
String              audioFileIndexString        = "0.mp3";
ButtonObject        volumeDownButtonObject      = ButtonObject(PIN_VOLUME_DOWN);    /*Device volume down    button   object.*/
ButtonObject        volumeUpButtonObject        = ButtonObject(PIN_VOLUME_UP  );    /*Device volume up      button   object.*/
RFIDReaderObject    rfidReaderObject            = RFIDReaderObject();
VS1053Object        vs1053Object                = VS1053Object();

void        setup                               (){

                Serial                              .begin(9600);
                rfidReaderObject                    .SetupVoid();
                EstablishProcessingConnection       ();

}
void        loop                                (){

                while(vs1053Object.GetPlayingMusicBool() == true ){ BothLoopVoid(); }
                while(vs1053Object.GetPlayingMusicBool() == false){ BothLoopVoid(); }

}
void        BothLoopVoid                        (){

                exhibitionReceiveIndexInt               = 
                    rfidReaderObject                    .LoopVoid                   (vs1053Object.GetPlayingMusicBool());
                MoveExhibitionVoid                      ();

                volumeDownButtonObject                  .ButtonVolumeDownLoopVoid   (vs1053Object);
                volumeUpButtonObject                    .ButtonVolumeUpLoopVoid     (vs1053Object);
                if(Serial.available() > 0){
                    String   inputString                = Serial.readString();
                    if      (inputString == "Next")     { PlayNextVoid();        }
                    if      (inputString == "Previous") { PlayPreviousVoid();    }
                }

}
void        EstablishProcessingConnection       (){ while(Serial.available() <= 0){ Serial.println("A"); Serial.println(Serial.available()); delay(300); } }
/*Create a function that send String to a connection between Arduino and the Processing sketch.*/
void        MoveExhibitionVoid                  (){

                //PROTOTYPE: Send command to Processing sketch to move player to new exhibition.
                if(exhibitionReceiveIndexInt != -1){
                    Serial.println(playerIndexInt);
                    Serial.println(rfidReaderObject.GetExhibitionReceivedNameAltString());
                }

}
void        PlayNextVoid                        (){

                    audioFileIndexInt           ++;
                if( audioFileIndexInt           >  audioFileIndexMaxInt){ audioFileIndexInt = audioFileIndexMinInt; }
                PlayVoid                        (AudioFileIndexCharArray(audioFileIndexInt));

}
void        PlayPreviousVoid                    (){

                    audioFileIndexInt           --;
                if( audioFileIndexInt           <  audioFileIndexMinInt){ audioFileIndexInt = audioFileIndexMaxInt; }
                PlayVoid                        (AudioFileIndexCharArray(audioFileIndexInt));

}
void        PlayVoid                            (char _audioNameCharArray[10]){ vs1053Object.GetAdafruitVS1053FilePlayer().startPlayingFile(_audioNameCharArray); }
char*       AudioFileIndexCharArray             (int  _audioFileIndexInt){

                audioFileIndexInt               = _audioFileIndexInt;
                audioFileIndexString            = String(audioFileIndexInt) + ".mp3";
                audioFileIndexString            .toCharArray(audioFileIndexCharArray, 10);
                return                          audioFileIndexCharArray;

}

