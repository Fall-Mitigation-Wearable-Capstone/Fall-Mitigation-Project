%% plot the simulated data

clear all;
close all;
clc;


sim_data = csvread('trial4.csv')
x = sim_data(800:1000, :)
time = 0:1/200:(length(x) - 1)/200

% plot(sim_data)

data1 = sim_data(800:1000, 1)
plot(time, data1*180/pi)
hold on
data2 = sim_data(800:1000, 2)
plot(time, data2*180/pi)
data3 = sim_data(800:1000, 3)
plot(time, data3*180/pi)
legend("x", "y", "z")