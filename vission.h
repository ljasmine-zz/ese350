#ifndef vission_H
#define vission_H

#include "mbed.h"
void i2c_write2(int addr, char a, char b);
void clock_init();
void cam_init(); 
void initVission(); 
int getIx1(); 
int getIx2();
