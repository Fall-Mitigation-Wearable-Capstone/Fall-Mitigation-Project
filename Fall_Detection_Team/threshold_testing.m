%% Data for testing the live algorithm
% Version 1
clear all;
close all;
clc;

participantNum = 3;
movement = "liveLeft";

data = csvread("Participant-Data\Participant " + participantNum + "\Raw\testing" + participantNum + "_" + movement + ".csv");
time =(data(:,1) - data(1,1)) / 1000;
g = data(:,2:4);
e = data(:,5:7);
falls = data(:,8);

figure(1)
subplot(2,1,1);
plot(time, g(:,1:2));
legend('gX', 'gY');

subplot(2,1,2);
plot(time, e(:,2:3));
legend('Pitch', 'Roll');


%% Plotting falls

figure(2)
% subplot(2,1,1)
plot(time, e(:,2:3));
hold on;

gx = g(:,1);
gy = g(:,2);
pitch = e(:,2);
roll = e(:,3);

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
%     circBuff_gyrDEDoz(i) = gz(i);
    data_ticks = data_ticks + 1;
end

for i = 31:length(e)
    diffRoll = roll(i) - circBuff_roll(mod(i, 31) + 1);
    diffPitch = pitch(i) - circBuff_pitch(mod(i, 31) + 1);
    diffGyroX = gx(i) - circBuff_gyrox(mod(i, 31) + 1);
    diffGyroY = gy(i) - circBuff_gyroy(mod(i, 31) + 1); 
%     diffGyroZ = gz(i) - circBuff_gyroz(mod(i, 31) + 1);
    
    % dR = -6.5, gx = -40, dgx = -10
%     if(diffRoll <= -5 && gx(i) <= -60) % diffRoll <= -6 && gx(i) < -90
%         x = sprintf("%0.2f forward fall: %0.2f %0.2f %0.2f", time(data_ticks), diffRoll, gx(i), diffGyroX); 
%         %x = sprintf("%d %0.2f forward: %0.2f %0.2f %0.2f", data_ticks, time(data_ticks), a(i, 1), a(i, 2), a(i, 3));
%         %x = sprintf("%d %0.2f forward: %0.2f", data_ticks, time(data_ticks), diffGyroX);
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

%     % current: 75, 8, 23
%     if(roll(i) > 60 && diffRoll > 8 && gx(i) > 23)
%         x = sprintf("%0.2f back fall: %0.2f %0.2f %0.2f", time(data_ticks), roll(i), diffRoll, gx(i)); 
%         disp(x)
%         if(back_flag < 16)
%             back_flag = back_flag + 1;
%         end
%     else
%         if(back_flag > 0)
%             back_flag = back_flag - 1;
%         end
%     end
    
    % dp = -6, gy = -50
    if(diffPitch < -7)
        x = sprintf("%0.2f left fall: %0.2f %0.2f %0.2f", time(data_ticks), pitch(i), diffPitch, gy(i)); 
        disp(x)
        
        if(left_flag < 16)
            left_flag = left_flag + 1;
        end
    else
        if(left_flag > 0)
            left_flag = left_flag - 1;
        end
    end
    
%     if(diffPitch > 6 && gy(i) > 55)
%         x = sprintf("%0.2f right fall: %0.2f %0.2f %0.2f", time(data_ticks), pitch(i), diffPitch, gy(i)); 
%         disp(x)
%         if(right_flag < 16)
%             right_flag = right_flag + 1;
%         end
%     else
%         if(right_flag > 0)
%             right_flag = right_flag - 1;
%         end
%     end
    
    if forward_flag >= 16
        plot(time(data_ticks), roll(data_ticks), 'bx')
    end
  
    if left_flag >= 16
        plot(time(data_ticks), pitch(data_ticks), 'ro')
    end
 
    if right_flag >= 16
        plot(time(data_ticks), pitch(data_ticks), 'mo')
    end
    
    if back_flag >= 16
        plot(time(data_ticks), roll(data_ticks), 'go')
    end
    
    circBuff_roll(mod(i, 31) + 1) = roll(i);
    circBuff_pitch(mod(i, 31) + 1) = pitch(i);
    circBuff_gyrox(mod(i, 31) + 1) = gx(i);
    circBuff_gyroy(mod(i, 31) + 1) = gy(i);
    data_ticks = data_ticks + 1;
end

% x = [0.6 0.51];
% y = [0.8 0.775];
% annotation('textarrow', x, y, 'String', 'Start of Fall (17.28s)')
% 
% x = [0.6 0.52];
% y = [0.7 0.67];
% annotation('textarrow', x, y, 'String', 'First Detected Fall (17.47s)')
% 
% title('Participant 1: Forward Fall Trial')

legend("Pitch Angle", "Right Fall Flag")%"roll")
ylabel("Degrees")
xlabel("Time (Seconds)")
% title(out1)
% title("Repeated Right Fall Trial")