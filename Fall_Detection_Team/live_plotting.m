%% Data for testing the live algorithm
% Version 1
clear all;
close all;
clc;

participantNum = 4;
movement = "adlOnly";

data = csvread("Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_" + movement + ".csv");
time =(data(:,1) - data(1,1)) / 1000;
g = data(:,2:4);
e = data(:,5:7);
falls = data(:,8);

plot1 = figure(1);
plot(time, e(:,2:3));
hold on;

for i = 1:length(falls)
    % forward
    if falls(i) == 1
        plot(time(i), e(i,3), 'ro')
    end
    
    % backwards
    if falls(i) == 2
        plot(time(i), e(i,3), 'bo')
    end
    
    % left 
    if falls(i) == 4
        plot(time(i), e(i,2), 'mo')
    end
    
    % right
    if falls(i) == 8
        plot(time(i), e(i,2), 'go')
    end
end