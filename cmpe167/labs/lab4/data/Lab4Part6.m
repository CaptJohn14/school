clear all;
clc;
close all;

A_data = table2array(readtable('lab4.xlsx','Sheet', 'part6', 'Range','A1:C301'))';
M_data = table2array(readtable('lab4.xlsx','Sheet', 'part6', 'Range','E1:G301'))';

[Rmis, Pbody] = AlignMasterSlave(A_data, M_data, [0;0;-1], [1;0;0], eye(3));

% Sb = Rmis'*M_data
% Sb = Sb'
% x = Sb(:,1);
% y = Sb(:,2);
% z = Sb(:,3);

% CalibrateEllipsoidData3D(x, y, z, 1, 1);