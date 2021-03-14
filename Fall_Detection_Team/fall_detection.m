%% Data for testing
clear all;
close all;
clc;

participantNum = 4;
movement = "right";

data = csvread("Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_" + movement + ".csv");
% time2 = 0:30/length(data2):(length(data2)-1)*30/length(data2);
time =(data(:,1) - data(1,1)) / 1000;
a = data(:,5:7);
g = data(:,2:4);
e = data(:,8:10);

figure(1)
% subplot(2,1,1)
plot(time, e(:,2:3));
legend("pitch", "roll")
hold on;
figure(2)
plot(e(:,2:3))
% figure(3)
% plot(a)
% legend("x", "y", "z")

% title("Euler")
% subplot(2,1,2)
% plot(range, e(range,2:3))

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

out1 = sprintf("%d %s", participantNum, movement);
disp(out1)

% range = 1:length(e);
range = 2662:2848;

figure(3)
plot(time(range), e(range, 2:3))
% hold on
% plot(time(range(1)+31), e(range(1)+31, 2:3), 'x')

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

figure(2)
% subplot(2,1,1)
plot(time, e(:,2:3));
hold on;

out1 = sprintf("testing: %d %s", participantNum, movement);
disp(out1);

%current reporitng frequency at 155hz

%fall flags for debouncing
forward_flag = 0;
right_flag = 0;
left_flag = 0;
back_flag = 0;

%buffers for data
circBuff_roll = zeros(31);
circBuff_pitch = zeros(31);
circBuff_gyrox = zeros(31);
circBuff_gyroy = zeros(31);
% circBuff_gyroz = zeros(31);

data_ticks = 1;

for i = 1:31
    circBuff_roll(i) = roll(i);
    circBuff_pitch(i) = pitch(i);
    circBuff_gyrox(i) = gx(i);
    circBuff_gyroy(i) = gy(i);
%     circBuff_gyroz(i) = gz(i);
    data_ticks = data_ticks + 1;
end

for i = 31:length(e)
    diffRoll = roll(i) - circBuff_roll(mod(i, 31) + 1);
    diffPitch = pitch(i) - circBuff_pitch(mod(i, 31) + 1);
    diffGyroX = gx(i) - circBuff_gyrox(mod(i, 31) + 1);
    diffGyroY = gy(i) - circBuff_gyroy(mod(i, 31) + 1); 
%     diffGyroZ = gz(i) - circBuff_gyroz(mod(i, 31) + 1);

%     if(diffRoll <= -6 && diffGyroX < -10 && gx(i) < - 20) % && diffGyroX <= -15 && gx(i) < -30 )
%         x = sprintf("%d forward fall: %0.2f %0.2f %0.2f", data_ticks, diffRoll, diffGyroX, gx(i)); 
%         disp(x)
%         
%         if(forward_flag < 16)
%             forward_flag = forward_flag + 1;
%         end
%     else
%         if(forward_flag > 0)
%             forward_flag = forward_flag - 1;
%         end
%     end

    if(diffRoll >= 7)
        back_flag = back_flag + 1;
    else
        back_flag = 0;
    end
    
    if(diffPitch < -6)
        x = sprintf("%0.2f left fall: %0.2f %0.2f", time(data_ticks), diffPitch, diffGyroY); 
        disp(x)
        
        if(left_flag < 16)
            left_flag = left_flag + 1;
        end
    else
        if(left_flag > 0)
            left_flag = left_flag - 1;
        end
    end
    
    if(diffPitch > 6)
        
        if(right_flag < 16)
            right_flag = right_flag + 1;
        end
    else
        if(right_flag > 0)
            right_flag = right_flag - 1;
        end
    end
    
%     if forward_flag >= 16
% %         x = sprintf("%0.2f forward fall: %0.2f %0.2f %0.2f", time(data_ticks), diffRoll, diffGyroX, roll(i)); 
% %         disp(x)
% %         
%         
%         plot(time(data_ticks), roll(data_ticks), 'co')
%         
% %         forward_flag = 0;
%     end
    
    if left_flag >= 16
        
        plot(time(data_ticks), pitch(data_ticks), 'ro')
%         left_flag = 0;
    end
 
    if right_flag >= 16
        x = sprintf("%0.2f right fall: %0.2f %0.2f", time(data_ticks), diffPitch, diffGyroY); 
        disp(x)
        
        plot(time(data_ticks), pitch(data_ticks), 'mo')
%         right_flag = 0;
    end
%     
%     if back_flag >= 5
%         x = sprintf("%0.2f back fall: %0.2f %0.2f", time(data_ticks), diffRoll, diffGyroX); 
%         disp(x)
%         
%         plot(time(data_ticks), roll(data_ticks), 'go')
%         back_flag = 0;
%     end
    
    circBuff_roll(mod(i, 31) + 1) = roll(i);
    circBuff_pitch(mod(i, 31) + 1) = pitch(i);
    circBuff_gyrox(mod(i, 31) + 1) = gx(i);
    circBuff_gyroy(mod(i, 31) + 1) = gy(i);
    data_ticks = data_ticks + 1;
end

legend("pitch", "roll")
















