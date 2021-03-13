%%
clear all;
close all;
clc;

participantNum = 3;

%%

data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_walk.csv");

time = data(:,1) - data(1,1);
g = data(:,2:4);
e = data(:,8:10);

%plot(time, e(:,2:3));
plot(time, e(:,2:3));
legend("pitch", "roll");


%%