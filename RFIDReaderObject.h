#ifndef     RFIDReaderObject_H
#define     RFIDReaderObject_H
#include    "Arduino.h"
#include    <SoftwareSerial.h>

class       RFIDReaderObject{

            public:
                        RFIDReaderObject                    ();
                void    SetupVoid                           ();
                int     LoopInt                            (bool _playingBool);
                int     GetLastMatchedIndexInt              ();
                String  GetExhibitionReceivedNameAltString  ();

};
#endif