#ifndef     ButtonObject_h
#define     ButtonObject_h
#include    "Arduino.h"
#include    "VS1053Object.h"
class       ButtonObject{
            public:
                        ButtonObject                (int            _buttonPinInt);
                void    ButtonVolumeDownLoopVoid    (VS1053Object   _vs1053);
                void    ButtonVolumeUpLoopVoid      (VS1053Object   _vs1053);
            private:
                bool    buttonStatePressedBool  ;
                int     buttonHighLow           ;
                int     buttonPinInt            ;
};
#endif

