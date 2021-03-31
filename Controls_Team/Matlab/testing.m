%%
clear all;
close all;
clc;

participantNum = 3;

%%
clc;
time = 0:30/length(data):(length(data)-1)*30/length(data);
% plot(data(2154:3000,8:9));
plot(time, data(:,8:9));
legend("pitch", "roll");

min(data(:,8))
max(data(:,8))
mean(data(:,8))
min(data(:,9))
max(data(:,9))
mean(data(:,9))

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_walk.csv");

% f1 = figure(1);
% sgtitle("Participant "+ participantNum + ": Walking Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f1, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_walk.png");

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_sit.csv");

% f2 = figure(2);
% sgtitle("Participant "+ participantNum + ": Standing-to-Sitting Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f2, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_sit.png");

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_sit2up.csv");

% f3 = figure(3);
% sgtitle("Participant "+ participantNum + ": Sitting-to-Standing Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f3, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_sit2up.png");

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_pickup.csv");

% f4 = figure(4);
% sgtitle("Participant "+ participantNum + ": Pick-Up Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f4, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_pickup.png");

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_forward.csv");

% f5 = figure(5);
% sgtitle("Participant "+ participantNum + ": Forward Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f5, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_forward.png");

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_right.csv");

% f6 = figure(6);
% sgtitle("Participant "+ participantNum + ": Right Fall Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f6, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_right.png");

%%
data = csvread("..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_collapse.csv");

% f7 = figure(7);
% sgtitle("Participant "+ participantNum + ": Collapse Test")
% 
% subplot(3,1,1)
% hold on;
% plot(data(:,1:3));
% legend("X", "Y", "Z")
% title("Gyroscope Data")
% ylabel("Degress / Sec")
% 
% subplot(3,1,2)
% hold on;
% plot(data(:,4:6));
% legend("X", "Y", "Z")
% title("Accelerometer Data")
% ylabel("Gs")
% 
% subplot(3,1,3)
% hold on;
% plot(data(:,7:9));
% legend("Yaw", "Pitch", "Roll")
% title("Euler Angles")
% ylabel("Degrees")

% saveas(f7, "..\..\Fall_Detection_Team\Participant-Data\Participant " + participantNum + "\Plots\testing" + participantNum + "_collapse.png");

