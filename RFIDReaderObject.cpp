#include "Arduino.h"
#include "RFIDReaderObject.h"
#include <SoftwareSerial.h>




const int byteNettoInt = 12;
const int tagAmountInt = 4;
const int byteBruttoInt = byteNettoInt + 4;





bool            tagRegisteredBool;
char            tagUnknownCharArray[byteNettoInt];
int             lastMatchedIndexInt;
int             registeredTagIntArray[tagAmountInt][byteBruttoInt] = {
    {54, 65, 48, 48, 52, 65, 48, 52, 48, 54, 50, 50},
    {54, 65, 48, 48, 52, 65, 48, 66, 54, 57, 52, 50},
    {56, 53, 48, 48, 48, 68, 54, 57, 55, 69, 57, 70},
    {56, 53, 48, 48, 51, 48, 68, 52, 68, 57, 66, 56}
};
int             tagUnknownIntArray[byteNettoInt];
SoftwareSerial  softwareSerialObject = SoftwareSerial(13, 12)    ;
String          exhibitionReceivedNameAltString;





/*==================================================
No need for construction here.*/
RFIDReaderObject::RFIDReaderObject(){}
/*==================================================*/





/*==================================================
This setup function is just to start software serial connection.*/
void RFIDReaderObject::SetupVoid(){ softwareSerialObject.begin(9600); }
/*==================================================*/





/*==================================================
Function to handle basic loop of this object.*/
int RFIDReaderObject::LoopInt(bool _playingBool){

    bool tagReadBool        = false;    /*Whether this object need to read RFID card or not.*/
    int tagByteCounterInt   = 0;        /*Index within byte array read from the RFID card.*/
    int tagByteReadInt;                 /*This is the byte got from the RFID card.*/

    /*Flush any serial monitor entry while this device is playing an audio file.*/
    if(_playingBool == true ){ while(softwareSerialObject.available() > 0){ softwareSerialObject.read(); } }
    /*If this device is not playing audio file then start listening for possible RFID card entry.*/
    else if(_playingBool == false){

        /*If there is an amount of serial inputed in software serial that determine that
            is from RFID card set tagRead to true.
        Then prepare the device to read RFID tag content.*/
        if(softwareSerialObject.available() == byteBruttoInt){ tagReadBool = true; }

        if(tagReadBool == true){

            while(softwareSerialObject.available() > 0){

                tagByteReadInt = softwareSerialObject.read();

                if(
                    tagByteReadInt != 2     &&
                    tagByteReadInt != 3     &&
                    tagByteReadInt != 10    &&
                    tagByteReadInt != 13
                ){

                    /*Feed the tagUnknownCharArray and tagUnknownIntArray with byte received from
                        the RFID tag.*/
                    tagUnknownCharArray[tagByteCounterInt] = tagByteReadInt;
                    tagUnknownIntArray[tagByteCounterInt] = tagByteReadInt;
                    tagByteCounterInt ++;

                    //Serial.print(tagByteReadInt);
                    //if(tagByteCounterInt != byteNettoInt){ Serial.print(" "); }

                }

                /*If tag read byte int returned 3 then it means to stop transmission/reading the
                    RFID tag.*/
                if(tagByteReadInt == 3){

                    tagReadBool = false;
                    //Serial.println(".");

                }

            }

        }

    }

    /*If there is no RFID card inputed then return -1 and reset the exhibitionReceivedNameAltString.*/
    if(strlen(tagUnknownCharArray) == 0){ exhibitionReceivedNameAltString = ""; return -1; }
    else{

        int byteCounterInt      = 0; /*The bytes from current iteration of tagCounterInt.*/
        int byteMatchCounterInt = 0; /*How many byte is matched with current iteration of the inputed RFID card.*/
        int tagCounterInt       = 0; /*The RFID tag that is registered within this device.*/

        /*This is while loop to check whethe the inputed RFID is registered RFID card or not.*/
        while(1){

            if(tagUnknownIntArray[byteCounterInt] == registeredTagIntArray[tagCounterInt][byteCounterInt]){

                byteCounterInt          ++;
                byteMatchCounterInt     ++;

            }
            else if(tagUnknownIntArray[byteCounterInt] != registeredTagIntArray[tagCounterInt][byteCounterInt]){

                byteCounterInt          = 0;
                byteMatchCounterInt     = 0;
                tagCounterInt           ++;

            }

            /*If the amount of bytes match is equal to the netto byte of an RFID card
                then the inputed RFID card is registered in this device.*/
            if(byteMatchCounterInt == byteNettoInt){

                tagRegisteredBool       = true;
                break;

            }
            /*Otherwise then the RFID tag is not yet registered.*/
            else if(tagCounterInt >= tagAmountInt){

                tagRegisteredBool       = false;
                break;

            }

        }

        /*Either way reset the buffer! So that we can capture another RFID card.*/
        for(int i = 0; i < byteNettoInt; i ++){ tagUnknownCharArray[i] = 0; }

        if(tagRegisteredBool == true){

            /*Here is a list of exhibition target that currently supported by this device
                according to the registeredTagIntArray.*/
            lastMatchedIndexInt = tagCounterInt;
            if(lastMatchedIndexInt == 0){ exhibitionReceivedNameAltString = "EXH_001"; }
            else if(lastMatchedIndexInt == 1){ exhibitionReceivedNameAltString = "EXH_002"; }
            else if(lastMatchedIndexInt == 2){ exhibitionReceivedNameAltString = "EXH_003"; }
            else if(lastMatchedIndexInt == 3){ exhibitionReceivedNameAltString = "EXH_004"; }

            //Serial.print("THIS CARD INDEX IS ");
            //Serial.print(tagCounterInt);
            //Serial.println(".");

            return lastMatchedIndexInt;

        }
        /*In case the tag is not registered.*/
        else if(tagRegisteredBool == false){

            exhibitionReceivedNameAltString = "";
            //Serial.println("THIS CARD INDEX IS NOT REGISTERED.");
            return -1;

        }

    }

}
/*==================================================*/





/*==================================================*/
int RFIDReaderObject::GetLastMatchedIndexInt(){ return lastMatchedIndexInt; }
/*==================================================*/





/*==================================================*/
String RFIDReaderObject::GetExhibitionReceivedNameAltString(){ return exhibitionReceivedNameAltString; }
/*==================================================*/
