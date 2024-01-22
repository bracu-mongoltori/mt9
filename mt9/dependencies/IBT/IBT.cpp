/*

  IBT.cpp - Library for ease use of IBT2 BTS7960 motor driver.
  Created by Al Mahir Ahmed, October 6th 2023.
  Released into the public domain.
  Contact: mahirgored@gmail.com; Github: Vulcan758

*/

#include "Arduino.h"
#include "IBT.h"

IBT::IBT(int LPWM, int RPWM, int LEN, int REN){
    L_PWM = LPWM;
    R_PWM = RPWM;
    L_EN = LEN;
    R_EN = REN;
    pinMode(L_PWM, OUTPUT);
    pinMode(R_PWM, OUTPUT);

    if (L_EN != 0 && R_EN != 0){
        pinMode(L_EN, OUTPUT);
        pinMode(R_EN, OUTPUT);

        digitalWrite(L_EN, HIGH);
        digitalWrite(R_EN, HIGH);  
    } 
}

void IBT::setSpeedLevel(int level){
    //this->checkEnable(1);
    int val = map(constrain(abs(level), 0, 10), 0, 10, 0, 255);
    if (level < 0){
        analogWrite(L_PWM, val);
        analogWrite(R_PWM, 0);

    }
    else{
        analogWrite(R_PWM, val);
        analogWrite(L_PWM, 0);

    }
}

void IBT::setRawSpeed(int val){
    //this->checkEnable(1);
    int raw_val = constrain(abs(val), 0, 255);
    if (val < 0){
        analogWrite(L_PWM, raw_val);
        analogWrite(R_PWM, 0);
    }
    else{
        analogWrite(R_PWM, raw_val);
        analogWrite(L_PWM, 0);
    }
}

void IBT::stop_(){
    analogWrite(L_PWM, 0);
    analogWrite(R_PWM, 0);
    //this->checkEnable(0);
}

void IBT::checkEnable(int switchType){
    if (L_EN != 0 && R_EN != 0){
        if (switchType == 0){ // Want to turn off the enable pins
            digitalWrite(L_EN, LOW);
            digitalWrite(R_EN, LOW);
        }
        else if (switchType == 1){ // Want to turn off the enable pins
            if (digitalRead(L_EN) == LOW or digitalRead(R_EN) == LOW){
                digitalWrite(L_EN, LOW);
                digitalWrite(R_EN, LOW);
                digitalWrite(L_EN, HIGH);
                digitalWrite(R_EN, HIGH);
            }
        } 

    }
}