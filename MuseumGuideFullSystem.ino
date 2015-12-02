#include                    "ButtonObject.h"
#include                    "RFIDReaderObject.h"
#include                    "VS1053Object.h"

#define                     PIN_VOLUME_DOWN                                     22                                 /*Device volume down    button      pin.*/
#define                     PIN_VOLUME_UP                                       24                                 /*Device volume up      button      pin.*/

        char                audioFileAnyCharArray               [10]            = "92.mp3";
        char                audioFileExhibitionCharArray        [10]            = "93.mp3";
        char                audioFileExplanationCharArray       [10]            = "94.mp3";
        char                audioFileIndexCharArray             [10]            = "1.mp3";
        char                audioFileMuseumCharArray            [10]            = "95.mp3";
        char                audioFileOrCharArray                [10]            = "96.mp3";
        char                audioFilePleaseVisitAndTapCharArray [10]            = "97.mp3";
        char                audioFileSampleCharArray            [10]            = "98.mp3";
        char                audioFileWelcomeToCharArray         [10]            = "99.mp3";
        int                 audioFileIndexInt                                   = 0;
        int                 audioFileIndexMaxInt                                = 8;
        int                 audioFileIndexMinInt                                = 1;
        int                 exhibitionReceiveIndexInt                           = -1;
        int                 exhibitionReceiveIndexSaveInt                       = -1;
        int                 playerIndexInt                                      = 1;
        //ButtonObject      volumeDownButtonObject                              = ButtonObject(PIN_VOLUME_DOWN);    /*Device volume down    button   object.*/
        //ButtonObject      volumeUpButtonObject                                = ButtonObject(PIN_VOLUME_UP  );    /*Device volume up      button   object.*/
        RFIDReaderObject    rfidReaderObject                                    = RFIDReaderObject();
        VS1053Object        vs1053Object                                        = VS1053Object();

void        setup                                                               (){

                Serial                                                          .begin(9600);
                rfidReaderObject                                                .SetupVoid();
                vs1053Object                                                    .SetupVoid();
                EstablishProcessingConnection                                   ();

                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileWelcomeToCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileMuseumCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileSampleCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFilePleaseVisitAndTapCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileAnyCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileExhibitionCharArray);

}
void        loop                                                                (){

                while(vs1053Object.GetPlayingMusicBool() == true ){ BothLoopVoid(); }
                while(vs1053Object.GetPlayingMusicBool() == false){ BothLoopVoid(); }

}
void        BothLoopVoid                                                        (){

                exhibitionReceiveIndexInt                                       = rfidReaderObject.LoopVoid(vs1053Object.GetPlayingMusicBool());
                if(exhibitionReceiveIndexInt != -1)                             { exhibitionReceiveIndexSaveInt = exhibitionReceiveIndexInt; }
                MoveExhibitionVoid                                              ();

                //volumeDownButtonObject                                        .ButtonVolumeDownLoopVoid   (vs1053Object);
                //volumeUpButtonObject                                          .ButtonVolumeUpLoopVoid     (vs1053Object);

                if(Serial.available() > 0){
                    String   inputString                                        = Serial.readString                                         ();
                    if      (inputString == "PLAY_EXHIBITION")                  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileExhibitionCharArray)                          ; }
                    if      (inputString == "PLAY_EXHIBITION_VISITED")          { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (AudioIndexCharArray(exhibitionReceiveIndexSaveInt))    ; exhibitionReceiveIndexSaveInt = -1; }
                    if      (inputString == "PLAY_EXPLANATION")                 { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileExplanationCharArray)                         ; }
                    if      (inputString == "PLAY_OR")                          { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileOrCharArray)                                  ; }
                    if      (inputString == "PLAY_PLEASE_VISIT_AND_TAP")        { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFilePleaseVisitAndTapCharArray)                   ; }
                    if      (inputString == "PLAY_WELCOME")                     { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileWelcomeToCharArray)                           ; }
                    for     (int i = audioFileIndexMinInt; i <= audioFileIndexMaxInt; i ++){
                                if(inputString == String(i))                    { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (AudioIndexCharArray(i))                                ; }
                    }
                }

}
void        EstablishProcessingConnection                                       (){ while(Serial.available() <= 0){ Serial.println("HANDSHAKE"); delay(300); } }
/*Create a function that send CharArray to a connection between Arduino and the Processing sketch.*/
void        MoveExhibitionVoid                                                  (){

                if(exhibitionReceiveIndexInt != -1){
                    Serial.println  ("SENT_PLAYER_IND_XXX="                     + String(playerIndexInt)                                );
                    delay           (300);
                    Serial.println  ("SENT_PLAYER_EXH_NXT="                     + rfidReaderObject.GetExhibitionReceivedNameAltString() );
                    delay           (300);
                }

}
void        PlayNextVoid                                                        (bool _playNoInterruptionBool){

                    audioFileIndexInt                                           ++;
                     if( audioFileIndexInt                                      >  audioFileIndexMaxInt)    { audioFileIndexInt = audioFileIndexMinInt; }
                     if(_playNoInterruptionBool                                 == true )                   { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile       (AudioIndexCharArray(audioFileIndexInt)); }
                else if(_playNoInterruptionBool                                 == false)                   { vs1053Object.GetAdafruitVS1053FilePlayer().startPlayingFile   (AudioIndexCharArray(audioFileIndexInt)); }

}
void        PlayPreviousVoid                                                    (bool _playNoInterruptionBool){

                    audioFileIndexInt                                           --;
                     if( audioFileIndexInt                                      <  audioFileIndexMinInt)    { audioFileIndexInt = audioFileIndexMaxInt; }
                     if(_playNoInterruptionBool                                 == true )                   { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile       (AudioIndexCharArray(audioFileIndexInt)); }
                else if(_playNoInterruptionBool                                 == false)                   { vs1053Object.GetAdafruitVS1053FilePlayer().startPlayingFile   (AudioIndexCharArray(audioFileIndexInt)); }

}
char*       AudioIndexCharArray                                                 (int  _audioFileIndexInt){

                audioFileIndexInt                                               = _audioFileIndexInt;
                String audioFileIndexString                                     = String(audioFileIndexInt + 1) + ".mp3";
                audioFileIndexString                                            .toCharArray(audioFileIndexCharArray, 10);
                return                                                          audioFileIndexCharArray;

}

