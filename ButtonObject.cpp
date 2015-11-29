#include    "ButtonObject.h"
bool        buttonStatePressedBool                  = false;
int         buttonHighLow                           ;
int         buttonPinInt                            ;

            ButtonObject::ButtonObject              (int _buttonPinInt){
                buttonPinInt                        = _buttonPinInt;
                pinMode                             (buttonPinInt, INPUT);
}

void        ButtonObject::ButtonVolumeDownLoopVoid  (VS1053Object _vs1053){
                buttonHighLow                       =  digitalRead(buttonPinInt);
                if      ((buttonHighLow             == HIGH) && (buttonStatePressedBool == false    )){ buttonStatePressedBool = true ; _vs1053.VolumeDownVoid(); }
                else if ((buttonHighLow             == LOW                                          )){ buttonStatePressedBool = false; }
}

void        ButtonObject::ButtonVolumeUpLoopVoid    (VS1053Object _vs1053){
                buttonHighLow                       =  digitalRead(buttonPinInt);
                if      ((buttonHighLow             == HIGH) && (buttonStatePressedBool == false    )){ buttonStatePressedBool = true ; _vs1053.VolumeUpVoid(); }
                else if ((buttonHighLow             == LOW                                          )){ buttonStatePressedBool = false; }
}
