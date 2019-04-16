
% clear all
close all
clc

AX = table2array(readtable('Data.xlsx','Sheet', 'part6_accel', 'Range','A1:A5940'));
AY = table2array(readtable('Data.xlsx','Sheet', 'part6_accel', 'Range','B1:B5940'));
AZ = table2array(readtable('Data.xlsx','Sheet', 'part6_accel', 'Range','C1:C5940'));

MX = table2array(readtable('Data.xlsx','Sheet', 'part6_mag', 'Range','A1:A5940'));
MY = table2array(readtable('Data.xlsx','Sheet', 'part6_mag', 'Range','B1:B5940'));
MZ = table2array(readtable('Data.xlsx','Sheet', 'part6_mag', 'Range','C1:C5940'));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%NORMALIZE
rawAnorm = sqrt(AX.^2 + AY.^2 + AZ.^2);
rawMnorm = sqrt(MX.^2 + MY.^2 + MZ.^2);

ax = (AX*1.0021)-0.0275
ay = (AY*0.999) -0.0061
az = (AZ*0.99)  -0.0211

mx = (MX*1.1517)-13.5
my = (MY*1.0833)-42.5
mz = (MZ*1.1253)+214.5 

% calAnorm = sqrt(ax.^2 + ay.^2 + az.^2);
% calMnorm = sqrt(mx.^2 + my.^2 + mz.^2);
% 
% figure('Name','Raw Normalize Acceleration','NumberTitle','off');
% plot(rawAnorm)
% title('Raw Normalize Acceleration');
% 
% figure('Name','Raw Magnetometer Acceleration','NumberTitle','off');
% plot(rawMnorm)
% title('Raw Normalize Magnetometer');
% 
% figure('Name','Cal Normalize Acceleration','NumberTitle','off');
% plot(calAnorm)
% title('Cal Normalize Acceleration');
% 
% figure('Name','Cal Magnetometer Acceleration','NumberTitle','off');
% plot(calMnorm)
% title('Cal Normalize Magnetometer');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('Name','part6 Accelerometer Ellipsoid','NumberTitle','off');
% [aa,ab] = CalibrateEllipsoidData3D(AX, AY, AZ, 2, 1);
% [axc, ayc, azc] = CorrectEllipsoidData3D(AX, AY, AZ, aa, ab);

% figure(1)
% [aa,ab] = CalibrateEllipsoidData3D(ax, ay, az, 1, 1);
% [axc, ayc, azc] = CorrectEllipsoidData3D(ax, ay, az, aa, ab);
% figure(2)
CalibrateEllipsoidData3D(axc', ayc', azc', 1, 1);

% figure('Name','part6 Accelerometer','NumberTitle','off');
% scatter3(axc, ayc, azc);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% figure('Name','part6 Magnetometer Ellipsoid','NumberTitle','off');
% [ma,mb] = CalibrateEllipsoidData3D(MX, MY, MZ, 1, 1);
% [mxc, myc, mzc] = CorrectEllipsoidData3D(MX, MY, MZ, ma, mb);
% 
% figure('Name','part6 Magnetometer','NumberTitle','off');
% scatter3(mxc, myc, mzc);

% x = (MX*1.1517)-13.5
% y = (MY*1.0833)-42.5
% z = (MZ*1.1253)+214.5 
% 
% figure('Name','part6 Magnetometer Ellipsoid','NumberTitle','off');
% [ma,mb] = CalibrateEllipsoidData3D(x, y, z, 5, 1);
% [mxc, myc, mzc] = CorrectEllipsoidData3D(x, y, z, ma, mb);
% 
% figure('Name','part6 Magnetometer','NumberTitle','off');
% scatter3(mxc, myc, mzc);
