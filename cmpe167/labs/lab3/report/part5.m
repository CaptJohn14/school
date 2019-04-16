clear all
close all
clc

x = readtable('Data.xlsx','Sheet', 'part5_data', 'Range','B1:B100');
y = readtable('Data.xlsx','Sheet', 'part5_data', 'Range','D1:D100');
z = readtable('Data.xlsx','Sheet', 'part5_data', 'Range','F1:F100');

X = table2array(x);
Y = table2array(y);
Z = table2array(z);

Gx = cumtrapz(X);
Gy = cumtrapz(Y);
Gz = cumtrapz(Z);

figure('Name','part5 X','NumberTitle','off');
plot(X);
title('Part5 X');
ylabel('angular rate');

figure('Name','part5 Y','NumberTitle','off');
plot(Y);
title('Part5 Y');
ylabel('angular rate');

figure('Name','part5 Z','NumberTitle','off');
plot(Z);
title('Part5 Z');
ylabel('angular rate');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% figure('Name','X Drift','NumberTitle','off');
% plot(Gx);
% title('X Angular Rate');
% ylabel('angle(degrees)');
% 
% figure('Name','Y Drift','NumberTitle','off');
% plot(Gy);
% title('Y Angular Rate');
% ylabel('angle(degrees)');
% 
% figure('Name','Z Drift','NumberTitle','off');
% plot(Gz);
% title('Z Angular Rate');
% ylabel('angle(degrees)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure('Name','X Drift','NumberTitle','off');
plot(Gx/8.8); %8.75555
title('X Angles');
ylabel('angle(degrees)');

figure('Name','Y Drift','NumberTitle','off');
plot(Gy/9.6); %9.53333
title('Y Angles');
ylabel('angle(degrees)');

figure('Name','Z Drift','NumberTitle','off');
plot(Gz/8.2); %8.1444
title('Z Angles');
ylabel('angle(degrees)');
