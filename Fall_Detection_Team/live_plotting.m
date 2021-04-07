%% Data for testing the live algorithm
% Version 1
clear all;
close all;
clc;

participantNum = 6;
movement = "longForward";

data = csvread("Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_" + movement + ".csv");
% time =(data(:,1) - data(1,1)) / 1000;

e = data(:,1:3);
falls = data(:,4);

plot1 = figure(1);
plot(e(:,2:3));
hold on;

for i = 1:length(falls)
    % forward
    if falls(i) == 1
        plot(i, e(i,3), 'ro')
    end
    
    % backwards
    if falls(i) == 2
        plot(i, e(i,3), 'bo')
    end
    
    % left 
    if falls(i) == 4
        plot(i, e(i,3), 'mo')
    end
    
    % right
    if falls(i) == 8
        plot(i, e(i,3), 'go')
    end
end