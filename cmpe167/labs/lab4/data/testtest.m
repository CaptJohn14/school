clear all;
clc;
close all;

ax = table2array(readtable('lab4.xlsx','Sheet', 'part6', 'Range','A1:A301'));
ay = table2array(readtable('lab4.xlsx','Sheet', 'part6', 'Range','B1:B301'));
az = table2array(readtable('lab4.xlsx','Sheet', 'part6', 'Range','C1:C301'));

[at, bt] = CalibrateEllipsoidData3D(ax, ay , az, 1, 1);

