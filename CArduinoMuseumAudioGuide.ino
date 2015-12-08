/*Arduiono program for museum guide.*/
#include                    "ButtonObject.h"
#include                    "RFIDReaderObject.h"
#include                    "VS1053Object.h"

#define                     PIN_VOLUME_DOWN                                     22                                  /*Device volume down    button      pin.                                */
#define                     PIN_VOLUME_UP                                       24                                  /*Device volume up      button      pin.                                */

        boolean             justDonePlayingAudioFileBoolean                     = false;                            /*Whether or not the device just finished playing an audio file.        */
        char                audioFileAnyCharArray               [10]            = "92.mp3";                         /*Reference to audio file that plays any.mp3.                           */
        char                audioFileExhibitionCharArray        [10]            = "93.mp3";                         /*Reference to audio file that plays exhibition.mp3.                    */
        char                audioFileExplanationCharArray       [10]            = "94.mp3";                         /*Reference to audio file that plays explanation.mp3.                   */
        char                audioFileIndexCharArray             [10]            = "1.mp3";                          /*A temporary char array that will contain a converted string.          */
        char                audioFileMuseumCharArray            [10]            = "95.mp3";                         /*Reference to audio file that plays museum.mp3.                        */
        char                audioFileOrCharArray                [10]            = "96.mp3";                         /*Reference to audio file that plays or.mp3.                            */
        char                audioFilePleaseVisitAndTapCharArray [10]            = "97.mp3";                         /*Reference to audio file that plays pleaseTapAndVisit.mp3.             */
        char                audioFileSampleCharArray            [10]            = "98.mp3";                         /*Reference to audio file that plays sample.mp3.                        */
        char                audioFileWelcomeToCharArray         [10]            = "99.mp3";                         /*Reference to audio file that plays welcomeTo.mp3.                     */
        int                 audioFileIndexInt                                   = 0;                                /*PROTOTYPE: Current audio index.                                       */
        int                 audioFileIndexMaxInt                                = 8;                                /*The latest exhibition index in this device.                           */
        int                 audioFileIndexMinInt                                = 1;                                /*The least exhibition index in this device.                            */
        int                 exhibitionReceiveIndexInt                           = -1;                               /*The receive index from RFID.                                          */
        int                 exhibitionReceiveIndexSaveInt                       = -1;                               /*The receive index from RFID (non null).                               */
        int                 playerIndexInt                                      = 1;                                /*The player index of this device.                                      */
        ButtonObject        volumeDownButtonObject                              = ButtonObject(PIN_VOLUME_DOWN);    /*Device volume down    button   object.                                */
        ButtonObject        volumeUpButtonObject                                = ButtonObject(PIN_VOLUME_UP  );    /*Device volume up      button   object.                                */
        RFIDReaderObject    rfidReaderObject                                    = RFIDReaderObject();
        VS1053Object        vs1053Object                                        = VS1053Object();

void        setup                                                               (){

                Serial                                                          .begin(9600);                       /*Initiate Sereial connection.                                          */
                rfidReaderObject                                                .SetupVoid();                       /*Setup the RFID component.                                             */
                vs1053Object                                                    .SetupVoid();                       /*Setup the VS1053 audio player.                                        */
                EstablishProcessingConnection                                   ();                                 /*Establish handshake connection for software controller.               */

                /*Play the file and welcome the visitor for the first time he/she enter the museum.*/
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileWelcomeToCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileSampleCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileMuseumCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFilePleaseVisitAndTapCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileAnyCharArray);
                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile         (audioFileExhibitionCharArray);

}
void        loop                                                                (){

                exhibitionReceiveIndexInt                                       = rfidReaderObject.LoopVoid(vs1053Object.GetPlayingMusicBool());    /*Always request exhibition index value from the RFID reader.                                                           */
                if(exhibitionReceiveIndexInt != -1)                             { exhibitionReceiveIndexSaveInt = exhibitionReceiveIndexInt; }      /*The the exhibition index value received from the RFID is -1 then do not process the value.                            */
                MoveExhibitionVoid                                              ();                                                                 /*If the exhibition index value received is legit (not -1) then process with moving the player into new exhibition.     */

                volumeDownButtonObject                                          .ButtonVolumeDownLoopVoid   (vs1053Object);
                volumeUpButtonObject                                            .ButtonVolumeUpLoopVoid     (vs1053Object);

                /*Send instruction to software controller that this device has finished playing an audio file.*/
                if( justDonePlayingAudioFileBoolean    == true ){
                    Serial                                                      .println("DONE_PLAYING_AUDIO_FILE");
                    justDonePlayingAudioFileBoolean                             =  false;
                }

                /*Convert Serial commmand into playing some specific audio file.*/
                if(Serial.available() > 0){
                    String   inputString                                        = Serial.readString                         ();
                    if      ((inputString == "PLAY_EXHIBITION")                 && (justDonePlayingAudioFileBoolean == false))  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileExhibitionCharArray)                          ; justDonePlayingAudioFileBoolean = true; }
                    if      ((inputString == "PLAY_EXHIBITION_VISITED")         && (justDonePlayingAudioFileBoolean == false))  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (AudioIndexCharArray(exhibitionReceiveIndexSaveInt))    ; justDonePlayingAudioFileBoolean = true; exhibitionReceiveIndexSaveInt = -1; }
                    if      ((inputString == "PLAY_EXPLANATION")                && (justDonePlayingAudioFileBoolean == false))  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileExplanationCharArray)                         ; justDonePlayingAudioFileBoolean = true; }
                    if      ((inputString == "PLAY_OR")                         && (justDonePlayingAudioFileBoolean == false))  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileOrCharArray)                                  ; justDonePlayingAudioFileBoolean = true; }
                    if      ((inputString == "PLAY_PLEASE_VISIT_AND_TAP")       && (justDonePlayingAudioFileBoolean == false))  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFilePleaseVisitAndTapCharArray)                   ; justDonePlayingAudioFileBoolean = true; }
                    if      ((inputString == "PLAY_WELCOME")                    && (justDonePlayingAudioFileBoolean == false))  { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (audioFileWelcomeToCharArray)                           ; justDonePlayingAudioFileBoolean = true; }
                    for     (int i = audioFileIndexMinInt; i <= audioFileIndexMaxInt; i ++){
                                if(inputString == String(i))                    { vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile   (AudioIndexCharArray(i))                                ; justDonePlayingAudioFileBoolean = true; }
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

