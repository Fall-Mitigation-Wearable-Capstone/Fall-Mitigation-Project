%% MuJoCo Fall Detection Algorithm
% Detect falls and determine different falls depending on Euler angles and
% gyroscope data received from MuJoCo simulations.

clear all; close all; clc;

% Open data files
sim_data = csvread('trial5.csv');
eul_data = csvread('eulers_trial5.csv');  % Place holder file name

% Extract relevant data points
gyro_x = sim_data(:, 1) * 180/pi;
gyro_y = sim_data(:, 2) * 180/pi;
gyro_z = sim_data(:, 3) * 180/pi;
pitch = eul_data(:, 2);
roll = eul_data(:, 3);

%% Pseudocode 1: Pure thresholds
% Check every point for fitting the fall parameters
% Raise a different flags for which fall gets noticed
% If the flag doesn't get flagged the again, turn flag off
% At 40 ticks, output if fall is sensed
forward_flag = 0;
slip_flag = 0;
faint_flag = 0;
left_flag = 0;
time_ticks = 0;
fall_flag = 0;
debounce_count = 0;
% data_ticks = 1;

for data_ticks = 1:length(sim_data)
    roll_curr = roll(data_ticks);
    pitch_curr = pitch(data_ticks);
    xrate_curr = gyro_x(data_ticks);
    yrate_curr = gyro_y(data_ticks);
    zrate_curr = gyro_z(data_ticks);
    
    if ((roll_curr <= -0.26) && (pitch_curr >= -1.28) && (abs(xrate_curr) >= 10.88) && (abs(zrate_curr) >= 86))
        forward_flag = forward_flag + 1;
    elseif ((roll_curr >= 11.44) && (pitch_curr <= 38.5) && (xrate_curr <= 152.19) && (yrate_curr <= -49.8) && (zrate_curr >= 74.4))
        faint_flag = faint_flag + 1;
    elseif ((roll_curr <= -111.6) && (pitch_curr >= -71.6) && (xrate_curr >= -77.8) && (yrate_curr <= -29.5) && (zrate_curr <= -6.7))
        slip_flag = slip_flag + 1;
    elseif ((roll_curr >= 35.5) && (pitch_curr >= -47.4) && (xrate_curr <= 25.6) && (yrate_curr <= 183.1) && (zrate_curr >= -107.1))
        left_flag = left_flag + 1;
    else
        forward_flag = 0;
        faint_flag = 0;
        slip_flag = 0;
        left_flag = 0;
    end
    
    if forward_flag >= 15
        forward_flag = 0;
        out = sprintf("%d Quick forward fall detected", data_ticks);
        disp(out)
    end
    if faint_flag >= 15
        faint_flag = 0;
        out = sprintf("%d Quick Faint fall detected", data_ticks);
        disp(out)
    end
    if slip_flag >= 15
        slip_flag = 0;
        out = sprintf("%d Quick Slip Fall detected", data_ticks);
        disp(out)
    end
    if left_flag >= 15
        left_flag = 0;
        out = sprintf("%d Quick Left Fall detected", data_ticks);
        disp(out)
    end
    
    time_ticks = time_ticks + 1;
    if time_ticks == 40
        if forward_flag >= 5
            forward_flag = 0;
            out = sprintf("%d Forward fall detected", data_ticks);
            disp(out)
        elseif faint_flag >= 5
            faint_flag = 0;
            out = sprintf("%d Faint fall detected", data_ticks);
            disp(out)
%             disp("Faint fall detected")
        elseif slip_flag >= 5
            slip_flag = 0;
            out = sprintf("%d Slip fall detected", data_ticks);
            disp(out)
        elseif left_flag >= 5
            left_flag = 0;
            out = sprintf("%d Left fall detected", data_ticks);
            disp(out)
        end
        time_ticks = 0;
    end
end

%% Pseudocode 2: 

% init: 
% buffer of 40 values (each euler)

% loop:
% get new data
% compare difference with oldest value
% update oldest
clear all; close all; clc;

% Open data files
sim_data = csvread('trial5.csv');
eul_data = csvread('eulers_trial5.csv');  % Place holder file name

% Extract relevant data points
gyro_x = sim_data(:, 1) * 180/pi;
gyro_y = sim_data(:, 2) * 180/pi;
gyro_z = sim_data(:, 3) * 180/pi;
pitch = eul_data(:, 2);
roll = eul_data(:, 3);

circBuff_roll = zeros(40);
circBuff_pitch = zeros(40);
circBuff_gyrox = zeros(40);
circBuff_gyroy = zeros(40);
circBuff_gyroz = zeros(40);
data_ticks = 1;

for i = 1:40
    circBuff_roll(i) = roll(i);
    circBuff_pitch(i) = pitch(i);
    circBuff_gyrox(i) = gyro_x(i);
    circBuff_gyroy(i) = gyro_y(i);
    circBuff_gyroz(i) = gyro_z(i);
    data_ticks = data_ticks + 1;
end

for i = 40:length(roll)
    diffRoll = roll(i) - circBuff_roll(mod(i, 40) + 1);
    diffPitch = pitch(i) - circBuff_pitch(mod(i, 40) + 1);
    diffGyroX = gyro_x(i) - circBuff_gyrox(mod(i, 40) + 1);
    diffGyroY = gyro_y(i) - circBuff_gyroy(mod(i, 40) + 1); 
    diffGyroZ = gyro_z(i) - circBuff_gyroz(mod(i, 40) + 1);
    
    if i >= 1500 && i <= 1560
        out = sprintf("%d %d %d %d", data_ticks, diffGyroX, diffGyroY, diffGyroZ);
        disp(out)
    end
%     
    if (diffPitch >= 61 && abs(diffRoll) <= 1.63 && abs(gyro_x(i)) <= 44.16 && gyro_y(i) >= 221.9 && abs(gyro_z(i)) <= 122.7)
%         out = sprintf("%d Forward detected", data_ticks);
%         disp(out)
            
    elseif (diffPitch <= 0 && diffPitch >= -28.97 && diffRoll <= 80 && diffRoll >= 23.14 && diffGyroX >= -130.127 && diffGyroX <= 0 && diffGyroY <= 56.275 && diffGyroY >= -150 && diffGyroZ <= 100 && diffGyroZ >= 0) %&& abs(gyro_x(i)) <= 13.7 && gyro_y(i) <= -146.7 && gyro_z(i) <= -89.47)
%         out = sprintf("%d Slip fall detected", data_ticks);
%         disp(out)
    
    elseif (diffPitch <= 0 && diffPitch >= -22.316 && diffRoll <= 98.025 && diffRoll >= 0 && diffGyroX <= 146.222 && diffGyroY <= -284.67 && diffGyroZ <= 235.84) % && gyro_x(i) >= 106.5 && gyro_y(i) >= -279.656)% && diffGyroZ <= 31.754)
        out = sprintf("%d Faint fall detected", data_ticks);
        disp(out)
            
%     elseif (diffPitch >= -20 && diffPitch <= -12.986 && diffRoll >= -3) % && diffGyroX <= 44.948 && diffGyroY <= -7.5601 && diffGyroZ >= -111.4029)
%         out = sprintf("%d Left fall detected", data_ticks);
%         disp(out)
    end
    
    circBuff_roll(mod(i, 40) + 1) = roll(i);
    circBuff_pitch(mod(i, 40) + 1) = pitch(i);
    circBuff_gyrox(mod(i, 40) + 1) = gyro_x(i);
    circBuff_gyroy(mod(i, 40) + 1) = gyro_y(i); 
    circBuff_gyroz(mod(i, 40) + 1) = gyro_z(i);
    data_ticks = data_ticks + 1;
end





