/*

  IBT.h - Library for ease use of IBT2 BTS7960 motor driver.
  Created by Al Mahir Ahmed, October 6th 2023.
  Released into the public domain.
  Contact: mahirgored@gmail.com; Github: Vulcan758

*/

#ifndef IBT_h
#define IBT_h

#include "Arduino.h"

class IBT{
    public:
        void setSpeedLevel(int level);
        void setRawSpeed(int val);
        void stop_();
        IBT(int LPWM, int RPWM, int LEN = 0, int REN = 0); // Short R_EN and L_EN to 5V
    private:
        int L_PWM;
        int R_PWM;
        int L_EN;
        int R_EN;
        void checkEnable(int switchType);
};

#endif