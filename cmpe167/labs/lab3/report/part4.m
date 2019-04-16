
clear all
close all
clc

t = readtable('Data.xlsx','Sheet', '1hrX', 'Range','A1:A7200');
x = readtable('Data.xlsx','Sheet', '1hrX', 'Range','D1:D7200');
y = readtable('Data.xlsx','Sheet', '1hrY', 'Range','A1:D7200');
z = readtable('Data.xlsx','Sheet', '1hrZ', 'Range','A1:D7200');

time = table2array(t);
X = table2array(x);
Y = table2array(y);
Z = table2array(z);

Gx = cumtrapz(time,X);
Gy = cumtrapz(time,Y);
Gz = cumtrapz(time,Z);

figure('Name','X Drift','NumberTitle','off');
plot(time, Gx);
title('X Angular Rate');
xlabel('time(s)') ;
ylabel('angle(degrees)');

figure('Name','Y Drift','NumberTitle','off');
plot(time, Gy);
title('Y Angular Rate');
xlabel('time(s)');
ylabel('angle(degrees)');

figure('Name','Z Drift','NumberTitle','off');
plot(time, Gz);
title('Z Angular Rate');
xlabel('time(s)');
ylabel('angle(degrees)');