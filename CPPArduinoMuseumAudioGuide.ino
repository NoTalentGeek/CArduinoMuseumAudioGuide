/*Arduino program for museum guide.*/
#include "ButtonObject.h"
#include "QueueList.h"
#include "RFIDReaderObject.h"
#include "VS1053Object.h"





#define PIN_REPEAT          22
#define PIN_VOLUME_DOWN     23 /*Device volume down button pin.*/
#define PIN_VOLUME_UP       24 /*Device volume up button pin.*/





boolean             initialBoolean                                      = true;
boolean             justDonePlayingAudioFileBoolean                     = false; /*Whether or not the device just finished playing an audio file.*/
boolean             stillCapturingBoolean                               = false;
ButtonObject        repeatButtonObject                                  = ButtonObject(PIN_REPEAT);
ButtonObject        volumeDownButtonObject                              = ButtonObject(PIN_VOLUME_DOWN);    /*Device volume down button object.*/
ButtonObject        volumeUpButtonObject                                = ButtonObject(PIN_VOLUME_UP);      /*Device volume up button object.*/
char                audioFileAnyCharArray               [10]            = "92.mp3"; /*Reference to audio file that plays any.mp3.*/
char                audioFileExhibitionCharArray        [10]            = "93.mp3"; /*Reference to audio file that plays exhibition.mp3.*/
char                audioFileExplanationCharArray       [10]            = "94.mp3"; /*Reference to audio file that plays explanation.mp3.*/
char                audioFileIndexCharArray             [10]            = "1.mp3";  /*A temporary char array that will contain a converted string.*/
char                audioFileMuseumCharArray            [10]            = "95.mp3"; /*Reference to audio file that plays museum.mp3.*/
char                audioFileOrCharArray                [10]            = "96.mp3"; /*Reference to audio file that plays or.mp3.*/
char                audioFilePleaseVisitAndTapCharArray [10]            = "97.mp3"; /*Reference to audio file that plays pleaseTapAndVisit.mp3.*/
char                audioFileSampleCharArray            [10]            = "98.mp3"; /*Reference to audio file that plays sample.mp3.*/
char                audioFileWelcomeToCharArray         [10]            = "99.mp3"; /*Reference to audio file that plays welcomeTo.mp3.*/
char                exhibitionCurrentIndexCharArray     [10]            ;
char                exhibitionExplanationIndexCharArray [10]            ;
char                exhibitionTargetIndex1CharArray     [10]            ;
char                exhibitionTargetIndex2CharArray     [10]            ;
char                exhibitionTargetIndex3CharArray     [10]            ;
char*               repeatInstructionCharPointerArray   [14]            ;
int                 audioFileIndexInt                                   = 0; /*PROTOTYPE: Current audio index.*/
int                 audioFileIndexMaxInt                                = 8; /*The latest exhibition index in this device.*/
int                 audioFileIndexMinInt                                = 1; /*The least exhibition index in this device.*/
int                 exhibitionCurrentIndexInt                           ;
int                 exhibitionExplanationIndexInt                       ;
int                 exhibitionReceiveIndexInt                           = -1; /*The receive index from RFID.*/
int                 exhibitionReceiveIndexSaveInt                       = -1; /*The receive index from RFID (non null).*/
int                 exhibitionTargetFillInCounterInt                    = 0;
int                 exhibitionTargetIndex1Int                           ;
int                 exhibitionTargetIndex2Int                           ;
int                 exhibitionTargetIndex3Int                           ;
int                 playerIndexInt                                      = 1; /*The player index of this device.*/
QueueList<char*>    repeatInstructionCharPointerQueueList               ;
RFIDReaderObject    rfidReaderObject                                    = RFIDReaderObject();
String              exhibitionCurrentIndexString                        ;
String              exhibitionExplanationIndexString                    ;
String              exhibitionTargetIndex1String                        ;
String              exhibitionTargetIndex2String                        ;
String              exhibitionTargetIndex3String                        ;
VS1053Object        vs1053Object                                        = VS1053Object();





/*==================================================*/
void setup(){

    Serial.begin(9600);             /*Initiate Sereial connection.*/
    rfidReaderObject.SetupVoid();   /*Setup the RFID component.*/
    vs1053Object.SetupVoid();       /*Setup the VS1053 audio player.*/

    /*Function does not working anymore since I am using the HaXe version
        of the museum manager.*/
    //EstablishProcessingConnection();

    /*Instead I am using this Serial.println("HANDSHAKE"); to establish
        one way connection to HaXe museum manager.*/
    Serial.println("HANDSHAKE");

    /*Play the file and welcome the visitor for the first time he/she enter the museum.*/
    vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileWelcomeToCharArray);
    vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileSampleCharArray);
    vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileMuseumCharArray);
    vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFilePleaseVisitAndTapCharArray);
    vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileAnyCharArray);
    vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileExhibitionCharArray);

}
/*==================================================*/




/*==================================================*/
void loop(){

    exhibitionReceiveIndexInt = rfidReaderObject.LoopInt(vs1053Object.GetPlayingMusicBool());          /*Always request exhibition index value from the RFID reader.*/
    if(exhibitionReceiveIndexInt != -1){ exhibitionReceiveIndexSaveInt = exhibitionReceiveIndexInt; }   /*The the exhibition index value received from the RFID is -1 then do not process the value.*/
    MoveExhibitionVoid(); /*If the exhibition index value received is legit (not -1) then proceed with moving the player into new exhibition.*/

    if(repeatButtonObject.ButtonPressedBoolean()  == true && stillCapturingBoolean == false){ RepeatInstructionVoid(initialBoolean);  }
    if(volumeDownButtonObject.ButtonPressedBoolean()    == true && stillCapturingBoolean == false){ vs1053Object.VolumeDownVoid();          }
    if(volumeUpButtonObject.ButtonPressedBoolean()      == true && stillCapturingBoolean == false){ vs1053Object.VolumeUpVoid();            }

    /*Send instruction to software controller that this device has finished playing an audio file.*/
    if(justDonePlayingAudioFileBoolean == true ){

        /*Send string "O" to the serial port to indicate the HaXe interface that
            this device has finished playing an audio file.*/
        Serial.println("O");

        justDonePlayingAudioFileBoolean = false;

    }

    /*Convert Serial commmand into playing some specific audio file.*/
    if(Serial.available() > 0){

        String inputString = Serial.readString();

        /*These are variables that I need to check before I give permission
            for this device to play an audio file.
        inputString is a variable that capture serial input from
            the serial monitor.
        justDonePlayingBoolean is a variable to indicates whether an
            audio file is or not currently played.
        stillCapturingBoolean is a variable to indicates whether this
            device is waiting FULL input from the interface or not.*/

        if(
            /*Play "Exhibition".*/
            (inputString == "Q")                        &&
            (justDonePlayingAudioFileBoolean == false)  &&
            (stillCapturingBoolean == true)
        ){

            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileExhibitionCharArray);
            repeatInstructionCharPointerQueueList.push(audioFileExhibitionCharArray);
            justDonePlayingAudioFileBoolean = true;

        }
        if(
            /*Play the index number of visitor's recent visited exhibition.*/
            (inputString == "W") &&
            (justDonePlayingAudioFileBoolean == false) &&
            (stillCapturingBoolean == true)
        ){

            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(AudioIndexCharArray(exhibitionReceiveIndexSaveInt + 1));
            repeatInstructionCharPointerQueueList.push(exhibitionCurrentIndexCharArray);
            justDonePlayingAudioFileBoolean = true;

            /*PENDING: Give explanation on exhibitionReceiveIndexSaveInt.*/
            exhibitionCurrentIndexInt = exhibitionReceiveIndexSaveInt + 1;
            exhibitionCurrentIndexString = String(exhibitionCurrentIndexInt) + ".mp3";
            exhibitionCurrentIndexString.toCharArray(exhibitionCurrentIndexCharArray, 10);
            exhibitionReceiveIndexSaveInt = -1;

        }
        if(
            /*Play audio file that says "Explanation.".*/
            (inputString == "E") &&
            (justDonePlayingAudioFileBoolean == false) &&
            (stillCapturingBoolean == true)
        ){

            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileExplanationCharArray);
            repeatInstructionCharPointerQueueList.push(audioFileExplanationCharArray);
            justDonePlayingAudioFileBoolean = true;

        }
        if(
            /*Play audio file that says "Or.".*/
            (inputString == "R") &&
            (justDonePlayingAudioFileBoolean == false) &&
            (stillCapturingBoolean == true)
        ){

            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileOrCharArray);
            repeatInstructionCharPointerQueueList.push(audioFileOrCharArray);
            justDonePlayingAudioFileBoolean = true;

        }
        if(
            /*Play audio file that says "Please visit and tap into.".*/
            (inputString == "T") &&
            (justDonePlayingAudioFileBoolean == false) &&
            (stillCapturingBoolean == true)
        ){

            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFilePleaseVisitAndTapCharArray);
            repeatInstructionCharPointerQueueList.push(audioFilePleaseVisitAndTapCharArray);
            justDonePlayingAudioFileBoolean = true;

        }
        if(
            /*Play audio file that says "Welcome.".*/
            (inputString == "Y") &&
            (justDonePlayingAudioFileBoolean == false) &&
            (stillCapturingBoolean == true)
        ){

            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileWelcomeToCharArray);
            repeatInstructionCharPointerQueueList.push(audioFileWelcomeToCharArray);
            justDonePlayingAudioFileBoolean = true;

        }

        if(inputString == "U"){

            /*When there is no capturing necessary again from the HaXe simulator
                program "U." will be received in serial monitor, hence this Arduino
                device will not capture any command anymore.*/
            justDonePlayingAudioFileBoolean = true;
            stillCapturingBoolean = false;

            /*Create entries for repeat button.*/
            int indexInt = 0;
            while(repeatInstructionCharPointerQueueList.isEmpty() == false) {

                /*CAUTION: So here is the most important codes for the repeat button.
                Basically what I did here is to switch (juggle) the value from
                    repeatInstructionCharPointerArray to repeatInstructionCharPointerQueueList.
                With this method then I can repeat any instruction any amount I want.*/
                repeatInstructionCharPointerArray[indexInt] = repeatInstructionCharPointerQueueList.pop();
                indexInt ++;

            }

        }

        if(inputString == "I"){

            /*"I." in serial monitor is to telling this Arduino device to start
                listening to set of instruction.*/
            stillCapturingBoolean = true;
            justDonePlayingAudioFileBoolean = true;

            /*PENDING: What is this initialBoolean used for?*/
            initialBoolean = false;
            /*PENDING: What is this variable below used for?*/
            exhibitionTargetFillInCounterInt = 0;

            /*Emptying queue for the repeat button.
            So that the repeat button will get a fresh new instruction to repeat.*/
            while(repeatInstructionCharPointerQueueList.isEmpty() == false){ repeatInstructionCharPointerQueueList.pop(); }

        }

        for(int i = audioFileIndexMinInt; i <= audioFileIndexMaxInt; i ++){

            if(
                (inputString == String(i)) &&
                (justDonePlayingAudioFileBoolean == false) &&
                (stillCapturingBoolean == true)
            ){

                vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(AudioIndexCharArray(i));


                if(exhibitionTargetFillInCounterInt == 0){

                    exhibitionExplanationIndexInt = i;
                    exhibitionExplanationIndexString = String(exhibitionExplanationIndexInt) + ".mp3";
                    exhibitionExplanationIndexString.toCharArray(exhibitionExplanationIndexCharArray, 10);
                    repeatInstructionCharPointerQueueList.push(exhibitionExplanationIndexCharArray);

                }
                else if(exhibitionTargetFillInCounterInt == 1){

                    exhibitionTargetIndex1Int = i;
                    exhibitionTargetIndex1String = String(exhibitionTargetIndex1Int) + ".mp3";
                    exhibitionTargetIndex1String.toCharArray(exhibitionTargetIndex1CharArray, 10);
                    repeatInstructionCharPointerQueueList.push(exhibitionTargetIndex1CharArray);

                }
                else if(exhibitionTargetFillInCounterInt == 2){

                    exhibitionTargetIndex2Int = i;
                    exhibitionTargetIndex2String = String(exhibitionTargetIndex2Int) + ".mp3";
                    exhibitionTargetIndex2String.toCharArray(exhibitionTargetIndex2CharArray, 10);
                    repeatInstructionCharPointerQueueList.push(exhibitionTargetIndex2CharArray);

                }
                else if(exhibitionTargetFillInCounterInt == 3){

                    exhibitionTargetIndex3Int = i;
                    exhibitionTargetIndex3String = String(exhibitionTargetIndex3Int) + ".mp3";
                    exhibitionTargetIndex3String.toCharArray(exhibitionTargetIndex3CharArray, 10);
                    repeatInstructionCharPointerQueueList.push(exhibitionTargetIndex3CharArray);

                }

                justDonePlayingAudioFileBoolean = true;
                exhibitionTargetFillInCounterInt ++;

            }

        }

    }

}
/*==================================================*/





/*==================================================
Establishing connection within the museum manager program.
Right now it is now working for the HaXe museum manager program.*/
void EstablishProcessingConnection(){ while(Serial.available() <= 0){ Serial.println("HANDSHAKE"); delay(300); } }
/*==================================================*/





/*==================================================
Create a function that send CharArray to a connection between Arduino and the Processing sketch.
This function is taking the destination exhibition straight from the RFID reader.
Hence, there is no need to have any parameters in this function.*/
void MoveExhibitionVoid(){

    if(exhibitionReceiveIndexInt != -1){

        Serial.println(rfidReaderObject.GetExhibitionReceivedNameAltString());
        delay(300);

        /*These codes below is working only for the Processing museum manager.
        These are not working anymore in HaXe museum manager.
        Serial.println("SENT_PLAYER_IND_XXX=" + String(playerIndexInt));
        delay(300);
        Serial.println("SENT_PLAYER_EXH_NXT=" + rfidReaderObject.GetExhibitionReceivedNameAltString());
        delay(300);
        */

    }

}
/*==================================================*/





/*==================================================
This is an obselete funtion yo play the next audio file in the micro SD card directory.
This is not used anymore because now this Arduino device is all controlled from the HaXe museum manager.*/
void PlayNextVoid(bool _playNoInterruptionBool){

    audioFileIndexInt ++;
    if( audioFileIndexInt > audioFileIndexMaxInt){ audioFileIndexInt = audioFileIndexMinInt; }
    if(_playNoInterruptionBool == true){ vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(AudioIndexCharArray(audioFileIndexInt)); }
    else if(_playNoInterruptionBool == false){ vs1053Object.GetAdafruitVS1053FilePlayer().startPlayingFile(AudioIndexCharArray(audioFileIndexInt)); }

}
/*==================================================*/





/*==================================================
This is an obselete funtion yo play the previous audio file in the micro SD card directory.
This is not used anymore because now this Arduino device is all controlled from the HaXe museum manager.*/
void PlayPreviousVoid(bool _playNoInterruptionBool){

    audioFileIndexInt --;
    if( audioFileIndexInt < audioFileIndexMinInt){ audioFileIndexInt = audioFileIndexMaxInt; }
    if(_playNoInterruptionBool == true){ vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(AudioIndexCharArray(audioFileIndexInt)); }
    else if(_playNoInterruptionBool == false){ vs1053Object.GetAdafruitVS1053FilePlayer().startPlayingFile(AudioIndexCharArray(audioFileIndexInt)); }

}
/*==================================================*/





/*==================================================
Button function to repeat instruction the parameter is only whether the visitor repeats
    the initial instruction when they start visiting the museum or t=instruction after they finish visited
    one exhibition.*/
void RepeatInstructionVoid(boolean _initialBoolean){

    /*Create a temporary quueue to be used within this function.*/
    QueueList<char*> tempRepeatInstruction1CharPointerStackList = repeatInstructionCharPointerQueueList;

    if(_initialBoolean == true){

        vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileWelcomeToCharArray);
        vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileSampleCharArray);
        vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileMuseumCharArray);
        vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFilePleaseVisitAndTapCharArray);
        vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileAnyCharArray);
        vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(audioFileExhibitionCharArray);

    }
    else if(_initialBoolean == false){

        /*PENDING: Create a simple flow chart drawing for repeat button functionality.*/
        for(int i = 0; i < sizeof(repeatInstructionCharPointerArray); i ++){
            vs1053Object.GetAdafruitVS1053FilePlayer().playFullFile(repeatInstructionCharPointerArray[i]);
        }

    }

}
/*==================================================*/




/*==================================================
Basically a function to convert any string to a char array that can be processed
    by the Arduino device to get to play a new audio file.*/
char* AudioIndexCharArray(int  _audioFileIndexInt){

    audioFileIndexInt = _audioFileIndexInt;
    String audioFileIndexString = String(audioFileIndexInt) + ".mp3";
    audioFileIndexString.toCharArray(audioFileIndexCharArray, 10);
    return audioFileIndexCharArray;

}
/*==================================================*/