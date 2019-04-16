clear;
clc;
close all;

% syms o z y
o = 4;
z = 442;
y = 44;

R = ([
    cos(o)*cos(y)                       cos(o)*sin(y)                       -1*sin(o);
    sin(z)*sin(o)*cos(y)-cos(z)*sin(y)  sin(z)*sin(o)*sin(y)+cos(z)*cos(y)  sin(z)*cos(o);
    cos(z)*sin(o)*cos(y)+sin(z)*sin(y)  cos(z)*sin(o)*sin(y)-sin(z)*cos(y)  cos(z)*cos(o)
    ]);

Rtrans = transpose(R);
Rident = R * Rtrans

% m = [
%     1 0 2; 
%     1 1 1; 
%     1 0 1
%     ];
% 
% pitch = -1 * asin(R(1,3));           %theta
% roll = asin(R(2,3) / cos(pitch));    %zero
% yaw = asin(R(1,2) / cos(pitch));     %Y



% test1 = [0.5257, 0.8507 ; 0.8507, -0.5257];
% test2 = transpose(test1)
% test3 = test1*test2;
% test4 = inv(test1)