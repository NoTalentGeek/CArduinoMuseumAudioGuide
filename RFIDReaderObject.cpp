#include                "Arduino.h"
#include                "RFIDReaderObject.h"
#include                <SoftwareSerial.h>
        bool            tagRegisteredBool                                       ;
        int             lastMatchedIndexInt                                     ;
const   int             byteNettoInt                                            = 12;
const   int             byteBruttoInt                                           = byteNettoInt + 4;
const   int             tagAmountInt                                            = 4;
        char            tagUnknownCharArray[byteNettoInt]                       ;
        int             registeredTagIntArray[tagAmountInt][byteBruttoInt]      = {
                            {54, 65, 48, 48, 52, 65, 48, 52, 48, 54, 50, 50},
                            {54, 65, 48, 48, 52, 65, 48, 66, 54, 57, 52, 50},
                            {56, 53, 48, 48, 48, 68, 54, 57, 55, 69, 57, 70},
                            {56, 53, 48, 48, 51, 48, 68, 52, 68, 57, 66, 56}
};
        int             tagUnknownIntArray[byteNettoInt]                        ;
        SoftwareSerial  softwareSerialObject                                    = SoftwareSerial(13, 12);
        String          exhibitionReceivedNameAltString                         ;

                RFIDReaderObject::RFIDReaderObject                              (){}
void            RFIDReaderObject::SetupVoid                                     (){ softwareSerialObject.begin(9600); }
int             RFIDReaderObject::LoopVoid                                      (bool _playingBool){

                bool    tagReadBool                 =  false;
                int     tagByteCounterInt           =  0;
                int     tagByteReadInt              ;
                     if (_playingBool               == true )                   { while(softwareSerialObject.available() > 0){ softwareSerialObject.read(); } }
                else if (_playingBool               == false)                   {
                        if (softwareSerialObject.available() == byteBruttoInt)  { tagReadBool = true; }
                        if (tagReadBool             == true)                    {
                        while(softwareSerialObject.available() > 0)             {
                            tagByteReadInt          =  softwareSerialObject.read();
                            if(
                                tagByteReadInt      != 2   &&
                                tagByteReadInt      != 3   &&
                                tagByteReadInt      != 10  &&
                                tagByteReadInt      != 13
                            ){
                                tagUnknownCharArray     [tagByteCounterInt] = tagByteReadInt;
                                tagUnknownIntArray      [tagByteCounterInt] = tagByteReadInt;
                                tagByteCounterInt                           ++;
                                Serial.print                              (tagByteReadInt);
                                if(tagByteCounterInt != byteNettoInt)     { Serial.print(" "); }
                            }
                            if(
                                tagByteReadInt      == 3
                            ){
                                tagReadBool         =  false;
                                Serial.println    (".");
                            }
                        }
                        }
                }
                if  (strlen(tagUnknownCharArray)    == 0)   { exhibitionReceivedNameAltString = ""; return -1; }
                else                                        {
                    int byteCounterInt              = 0;
                    int byteMatchCounterInt         = 0;
                    int tagCounterInt               = 0;
                    while(1)                        {
                             if(tagUnknownIntArray[byteCounterInt] == registeredTagIntArray[tagCounterInt][byteCounterInt]){
                                byteCounterInt      ++;
                                byteMatchCounterInt ++;
                        }
                        else if(tagUnknownIntArray[byteCounterInt] != registeredTagIntArray[tagCounterInt][byteCounterInt]){
                                byteCounterInt      = 0;
                                byteMatchCounterInt = 0;
                                tagCounterInt       ++;
                        }
                             if(byteMatchCounterInt == byteNettoInt){
                                tagRegisteredBool   =  true;
                                break;
                        }
                        else if(tagCounterInt       >= tagAmountInt){
                                tagRegisteredBool   =  false;
                                break;
                        }
                    }

                    for(int i = 0; i < byteNettoInt; i ++)                      { tagUnknownCharArray[i] = 0; }

                         if(tagRegisteredBool                                   == true ){
                            //Serial.print                                      ("THIS CARD INDEX IS ");
                            //Serial.print                                      (tagCounterInt);
                            //Serial.println                                    (".");
                            lastMatchedIndexInt                                 =  tagCounterInt;
                                 if(lastMatchedIndexInt == 0)                   { exhibitionReceivedNameAltString = "EXH_ZOD"; }
                            else if(lastMatchedIndexInt == 1)                   { exhibitionReceivedNameAltString = "EXH_BOD"; }
                            else if(lastMatchedIndexInt == 2)                   { exhibitionReceivedNameAltString = "EXH_SPH"; }
                            else if(lastMatchedIndexInt == 3)                   { exhibitionReceivedNameAltString = "EXH_USH"; }
                            return                                              lastMatchedIndexInt;
                    }
                    else if(tagRegisteredBool                                   == false){
                            //Serial.println                                    ("THIS CARD INDEX IS NOT REGISTERED.");
                            exhibitionReceivedNameAltString                     = "";
                            return                                              -1;
                    }

                }

}
String          RFIDReaderObject::GetExhibitionReceivedNameAltString            (){ return exhibitionReceivedNameAltString; }
