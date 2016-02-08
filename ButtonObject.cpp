#include    "ButtonObject.h"
bool        buttonStatePressedBool                  = false;
int         buttonHighLow                           ;
int         buttonPinInt                            ;

            ButtonObject::ButtonObject              (int _buttonPinInt){
                buttonPinInt                        = _buttonPinInt;
                pinMode                             (buttonPinInt, INPUT);
}

boolean 	ButtonObject::ButtonPressedBoolean 		(){
				buttonHighLow                       =  digitalRead(buttonPinInt);
 				if      ((buttonHighLow             == HIGH) && (buttonStatePressedBool == false    )){ buttonStatePressedBool = true ; }
				else if ((buttonHighLow             == LOW                                          )){ buttonStatePressedBool = false; }
				return 	buttonStatePressedBool;
}

