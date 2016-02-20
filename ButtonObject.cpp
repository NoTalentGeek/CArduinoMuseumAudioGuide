#include "ButtonObject.h"





bool buttonStatePressedBool = false;    /*Whether this button is currently being pressed or not.*/
int buttonHighLow = -1;                 /*State button LOW or HIGH.*/
int buttonPinInt = -1;                  /*Button pin location.*/





/*==================================================
This is basically a class to add a control for a physical
    button.*/
ButtonObject::ButtonObject(int _buttonPinInt){

    /*Assign button pin as a local variable.*/
    buttonPinInt = _buttonPinInt;
    /*Set input mode.*/
    pinMode(buttonPinInt, INPUT);

}
/*==================================================*/





/*==================================================
Function to set a control whether a button is pressed.*/
boolean ButtonObject::ButtonPressedBoolean(){

    buttonHighLow = digitalRead(buttonPinInt);

    if(
        (buttonHighLow == HIGH) &&
        (buttonStatePressedBool == false)
    ){ buttonStatePressedBool = true; }
    else if((buttonHighLow == LOW)){ buttonStatePressedBool = false; }

    return buttonStatePressedBool;

}
/*==================================================*/