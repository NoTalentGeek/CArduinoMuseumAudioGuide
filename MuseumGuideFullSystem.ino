#include                    "ButtonObject.h"
#include                    "RFIDReaderObject.h"
#include                    "VS1053Object.h"

#define                     PIN_VOLUME_DOWN                                 22                                 /*Device volume down    button      pin.*/
#define                     PIN_VOLUME_UP                                   24                                 /*Device volume up      button      pin.*/

        char                audioFileAnyString                  [10]        = "92.mp3";
        char                audioFileExhibitionString           [10]        = "93.mp3";
        char                audioFileExplanationString          [10]        = "94.mp3";
        char                audioFileMuseumString               [10]        = "95.mp3";
        char                audioFileOrString                   [10]        = "96.mp3";
        char                audioFilePleaseVisitAndTapString    [10]        = "97.mp3";
        char                audioFileSampleString               [10]        = "98.mp3";
        char                audioFileWelcomeToString            [10]        = "99.mp3";

        int                 audioFileIndexInt                               = 0;
        int                 audioFileIndexMaxInt                            = 3;
        int                 audioFileIndexMinInt                            = 0;
        int                 playerIndexInt                                  = 1;
        int                 exhibitionReceiveIndexInt                       ;
        char                audioFileIndexCharArray[10]                     ;
        String              audioFileIndexString                            = "0.mp3";
        //ButtonObject      volumeDownButtonObject                          = ButtonObject(PIN_VOLUME_DOWN);    /*Device volume down    button   object.*/
        //ButtonObject      volumeUpButtonObject                            = ButtonObject(PIN_VOLUME_UP  );    /*Device volume up      button   object.*/
        RFIDReaderObject    rfidReaderObject                                = RFIDReaderObject();
        VS1053Object        vs1053Object                                    = VS1053Object();

void        setup                                                           (){

                Serial                                                      .begin(9600);
                rfidReaderObject                                            .SetupVoid();
                vs1053Object                                                .SetupVoid();
                //EstablishProcessingConnection                             ();

                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile     (audioFileWelcomeToString);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile     (audioFileMuseumString);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile     (audioFileSampleString);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile     (audioFilePleaseVisitAndTapString);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile     (audioFileAnyString);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile     (audioFileExhibitionString);

}
void        loop                                                            (){

                Serial.println("TEST");
                while(vs1053Object.GetPlayingMusicBool() == true ){ BothLoopVoid(); }
                while(vs1053Object.GetPlayingMusicBool() == false){ BothLoopVoid(); }

}
void        BothLoopVoid                                                (){

                exhibitionReceiveIndexInt                               = 
                    rfidReaderObject                                    .LoopVoid                   (vs1053Object.GetPlayingMusicBool());
                MoveExhibitionVoid                                      ();

                //volumeDownButtonObject                                .ButtonVolumeDownLoopVoid   (vs1053Object);
                //volumeUpButtonObject                                  .ButtonVolumeUpLoopVoid     (vs1053Object);

                if(Serial.available() > 0){
                    String   inputString                                = Serial.readString();
                    if      (inputString == "Next")                     { PlayNextVoid      (false);    }
                    if      (inputString == "Previous")                 { PlayPreviousVoid  (false);    }
                }

}
void        EstablishProcessingConnection                               (){ while(Serial.available() <= 0){ Serial.println("A"); Serial.println(Serial.available()); delay(300); } }
/*Create a function that send String to a connection between Arduino and the Processing sketch.*/
void        MoveExhibitionVoid                                          (){

                //PROTOTYPE: Send command to Processing sketch to move player to new exhibition.
                if(exhibitionReceiveIndexInt != -1){
                    Serial.println("SENT_PLAYER_INDEX = "               + playerIndexInt                                        );
                    Serial.println("SENT_PLAYER_EXHIBITION_NEXT = "     + rfidReaderObject.GetExhibitionReceivedNameAltString() );

                    //PENDING:                                          Move this codes into loop function waiting for Serial.read().
                    //PROTOTYPE:                                        Play: Welcome to.
                    //PROTOTYPE:                                        Play: Exhibition XXX.
                    //PROTOTYPE:                                        Play: Explanation XXX.
                    //PROTOTYPE:                                        Play: Please visit and tap into.
                    //PROTOTYPE:                                        Play: Exhibition Target 1.
                    //PROTOTYPE:                                        Play: Or.
                    //PROTOTYPE:                                        Play: Exhibition Target 2.
                    //PROTOTYPE:                                        Play: Or.
                    //PROTOTYPE:                                        Play: Exhibition Target 3.
                    PlayVoid                                            ("welcome_to.mp3");
                    PlayVoid                                            ("exhibition.mp3");
                    PlayVoid                                            (AudioFileIndexString(exhibitionReceiveIndexInt));                                         
                    PlayVoid                                            ("explanation.mp3");
                    //PENDING:                                          Generate random number to assign an explanation to this player.
                    PlayVoid                                            ("please_visit_and_tap_into.mp3");
                    PlayVoid                                            ("exhibition.mp3");
                    //PENDING:                                          Get the three target exhibitions.
                    PlayVoid                                            ("or.mp3");
                    PlayVoid                                            ("exhibition.mp3");
                    //PENDING:                                          Get the three target exhibitions
                    PlayVoid                                            ("or.mp3");
                    PlayVoid                                            ("exhibition.mp3");
                    //PENDING:                                          Get the three target exhibitions

                }

}
void        PlayNextVoid                                                (bool _playNoInterruptionBool){

                    audioFileIndexInt                                   ++;
                     if( audioFileIndexInt                              >  audioFileIndexMaxInt)    { audioFileIndexInt = audioFileIndexMinInt; }
                     if(_playNoInterruptionBool                         == true )                   { PlayNoInterruptionVoid   (AudioFileIndexString(audioFileIndexInt)); }
                else if(_playNoInterruptionBool                         == false)                   { PlayVoid                 (AudioFileIndexString(audioFileIndexInt)); }

}
void        PlayPreviousVoid                                            (bool _playNoInterruptionBool){

                    audioFileIndexInt                                   --;
                     if( audioFileIndexInt                              <  audioFileIndexMinInt)    { audioFileIndexInt = audioFileIndexMaxInt; }
                     if(_playNoInterruptionBool                         == true )                   { PlayNoInterruptionVoid   (AudioFileIndexString(audioFileIndexInt)); }
                else if(_playNoInterruptionBool                         == false)                   { PlayVoid                 (AudioFileIndexString(audioFileIndexInt)); }

}
void        PlayVoid                                                    (String _audioNameString){

                char                audioNameCharArray[6];
                _audioNameString    .toCharArray                                    (audioNameCharArray, 10);
                if(!vs1053Object    .GetAdafruitVS1053FilePlayer().startPlayingFile (audioNameCharArray)){ Serial.println("FAILED TO PLAY AUDIO FILE."); }

}
void        PlayNoInterruptionVoid                                      (String _audioNameString){

                char                audioNameCharArray[6];
                _audioNameString    .toCharArray                                    (audioNameCharArray, 10);
                if(!vs1053Object    .GetAdafruitVS1053FilePlayer().playFullFile     (audioNameCharArray)){ Serial.println("FAILED TO PLAY AUDIO FILE."); }

}
String      AudioFileIndexString                                        (int  _audioFileIndexInt){

                audioFileIndexInt                                       = _audioFileIndexInt;
                audioFileIndexString                                    = String(audioFileIndexInt) + ".mp3";
                return                                                  audioFileIndexString;

}

