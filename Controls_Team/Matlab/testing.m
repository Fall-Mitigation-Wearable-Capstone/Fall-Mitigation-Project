%%
clear all;
close all;
clc;

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\David\Raw\testing1_walk.csv");

yaw = data(:, 1);
pitch = data(:, 2);
roll = data(:, 3);

figure(1)
plot(pitch)
hold on
plot(roll)
legend("pitch", "roll")

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\David\Raw\testing1_forward_fall.csv");

yaw = data(:, 1);
pitch = data(:, 2);
roll = data(:, 3);

figure(1)
plot(pitch)
hold on
plot(roll)
legend("pitch", "roll")
