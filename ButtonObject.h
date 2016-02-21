#ifndef     ButtonObject_h
#define     ButtonObject_h





#include    "Arduino.h"
#include    "VS1053Object.h"





class ButtonObject{

    public:
        boolean ButtonPressedBoolean();
        ButtonObject(int _buttonPinInt);

    private:
        bool buttonStatePressedBool;
        int buttonHighLow;
        int buttonPinInt;

};





#endif