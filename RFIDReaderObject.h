#ifndef RFIDReaderObject_H
#define RFIDReaderObject_H





#include "Arduino.h"
#include <SoftwareSerial.h>






class RFIDReaderObject{

    public:
        RFIDReaderObject();
        int GetLastMatchedIndexInt();
        int LoopInt(bool _playingBool);
        String GetExhibitionReceivedNameAltString();
        void SetupVoid();

};





#endif