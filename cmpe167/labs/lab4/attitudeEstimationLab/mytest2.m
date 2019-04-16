clear all;
clc;
close all;

Ro=eye(3);


accelReading=[0.1; 0.2; 1.1];
magReading=[-0.1; 0.2; 0.3];

accelInertial=[0;0;-1];
magInertial=[1;0;0];

R = DCMfromTriad(magReading,accelReading,magInertial,accelInertial)
