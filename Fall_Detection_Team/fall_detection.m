%% Data for testing
clear all;
close all;
clc;

participantNum = 2;

data = csvread("Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_sit2up.csv");
% time2 = 0:30/length(data2):(length(data2)-1)*30/length(data2);
g = data(:,1:3);
e = data(:,7:9);

% range = 2397:2517; %forward 1
% range = 2533:2615; %forward 2
% range = 2651:2712; %right 2
% range = 1:length(data); %walking (both)
% range = 2508:2819; %bending over 1
% range = 2820:3222; %straighting out 1
% range = 2379:2731; %bending over 2
% range = 2732:3052; %straighting out 2
range = 2400:3000;

figure(1)
subplot(2,1,1)
plot(e(:,2:3))
legend("pitch", "roll")
title("Euler")
subplot(2,1,2)
plot(range, e(range,2:3))

% figure(2)
% title("Gyro")
% plot(g)

% range = 1:length(e);

pitch = e(:,2);
roll = e(:,3);
gx = g(:,1);
gy = g(:,2);
gz = g(:,3);

%% Finding thresh

data_ticks = 1;
circBuff_roll = zeros(31);
circBuff_pitch = zeros(31);
circBuff_gyrox = zeros(31);
circBuff_gyroy = zeros(31);
circBuff_gyroz = zeros(31);

START = range(1);
END = range(length(range));

for i = START:START+31
    circBuff_roll(i-START+1) = roll(i);
    circBuff_pitch(i-START+1) = pitch(i);
    circBuff_gyrox(i-START+1) = gx(i);
    circBuff_gyroy(i-START+1) = gy(i);
    circBuff_gyroz(i-START+1) = gz(i);
    data_ticks = data_ticks + 1;
end

minRoll = 1000;
maxRoll = -1000;

minPitch = 1000;
maxPitch = -1000;

minGx = 1000;
maxGx = -1000;

minGy = 1000;
maxGy = -1000;

minGz = 1000;
maxGz = -1000;

minTick = 0;
maxTick = 0;

for i = START+32:END
    diffRoll = roll(i) - circBuff_roll(mod(i-START-31, 31) + 1);
    diffPitch = pitch(i) - circBuff_pitch(mod(i-START-31, 31) + 1);
    diffgx = gx(i) - circBuff_gyrox(mod(i-START-31, 31) + 1);
    diffgy = gy(i) - circBuff_gyroy(mod(i-START-31, 31) + 1);
    diffgz = gz(i) - circBuff_gyroz(mod(i-START-31, 31) + 1);
    
    if minRoll > diffRoll
        minRoll = diffRoll;
    end
    if maxRoll < diffRoll
        maxRoll = diffRoll;
    end
    
    if minPitch > diffPitch
        minPitch = diffPitch;
    end
    if maxPitch < diffPitch
        maxPitch = diffPitch;
    end
    
    if minGx > diffgx
        minGx = diffgx;
    end
    if maxGx < diffgx
        maxGx = diffgx;
    end
    
    if minGy > diffgy
        minGy = diffgy;
    end
    if maxGy < diffgy
        maxGy = diffgy;
    end
    
    if minGz > diffgz
        minGz = diffgz;
    end
    if maxGz < diffgz
        maxGz = diffgz;
    end
    
    circBuff_roll(mod(i-START-31, 40) + 1) = roll(i);
    circBuff_pitch(mod(i-START-31, 40) + 1) = pitch(i);
    circBuff_gyrox(mod(i-START-31, 40) + 1) = gx(i);
    circBuff_gyroy(mod(i-START-31, 40) + 1) = gy(i);
    circBuff_gyroz(mod(i-START-31, 40) + 1) = gz(i);
    data_ticks = data_ticks + 1;
end

out1 = sprintf("Pitch min: %0.2f", minPitch);
out2 = sprintf("Pitch max: %0.2f", maxPitch);
disp(out1)
disp(out2)

out1 = sprintf("Roll min: %0.2f", minRoll);
out2 = sprintf("Roll max: %0.2f", maxRoll);
disp(out1)
disp(out2)

out1 = sprintf("Gx min: %0.2f", minGx);
out2 = sprintf("Gx max: %0.2f", maxGx);
disp(out1)
disp(out2)

out1 = sprintf("Gy min: %0.2f", minGy);
out2 = sprintf("Gy max: %0.2f", maxGy);
disp(out1)
disp(out2)

out1 = sprintf("Gz min: %0.2f", minGz);
out2 = sprintf("Gz max: %0.2f", maxGz);
disp(out1)
disp(out2)

%% Algorithm

%current reporitng frequency at 155hz

%fall flags for debouncing
foward_flag = 0;

%buffers for data
circBuff_roll = zeros(31);
circBuff_pitch = zeros(31);
circBuff_gyrox = zeros(31);
circBuff_gyroy = zeros(31);
circBuff_gyroz = zeros(31);

data_ticks = 1;


for i = 1:31
    circBuff_roll(i) = roll(i);
    circBuff_pitch(i) = pitch(i);
%     circBuff_gyrox(i) = gx(i);
%     circBuff_gyroy(i) = gy(i);
%     circBuff_gyroz(i) = gz(i);
    data_ticks = data_ticks + 1;
end

for i = 31:length(e)
    diffRoll = roll(i) - circBuff_roll(mod(i, 31) + 1);
    diffPitch = pitch(i) - circBuff_pitch(mod(i, 31) + 1);
%     diffGyroX = gx(i) - circBuff_gyrox(mod(i, 40) + 1);
%     diffGyroY = gy(i) - circBuff_gyroy(mod(i, 40) + 1); 
%     diffGyroZ = gz(i) - circBuff_gyroz(mod(i, 40) + 1);

%     if(diffRoll >= 2.61 && diffRoll <= 16.6)
%         x = sprintf("%d forward fall", data_ticks); 
%         disp(x)
%     end

    if(diffPitch >= -38.7 && diffPitch <= -10.3)
        x = sprintf("%d right fall", data_ticks); 
        disp(x)
    end
    
    circBuff_roll(mod(i, 40) + 1) = roll(i);
    circBuff_pitch(mod(i, 40) + 1) = pitch(i);
    data_ticks = data_ticks + 1;
end




















