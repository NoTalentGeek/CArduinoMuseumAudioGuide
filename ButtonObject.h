#ifndef     ButtonObject_h
#define     ButtonObject_h





#include    "Arduino.h"
#include    "VS1053Object.h"





class ButtonObject{

    public:
        ButtonObject(int _buttonPinInt);
        boolean ButtonPressedBoolean();

    private:
        bool buttonStatePressedBool;
        int buttonHighLow;
        int buttonPinInt;

};





#endif