%using magdwick filter on mag data
%%%--------------------------------------------------------------------
clc
clear all
close all

t = csvread("");

a = t(:,1:3);
m = t(:,4:6);
g = t(:,7:9);

a(:, 3) = -a(:,3);
m(:, 3) = -m(:,3);
g(:, 3) = -g(:,3);

% figure(1)
% subplot(3,1,1)
% plot(a)
% subplot(3,1,2)
% plot(m)
% subplot(3,1,3)
% plot(g)

% %%
% load("ExampleData.mat");
% 
% figure(2)
% subplot(3,1,1)
% plot(Accelerometer)
% legend('X', 'Y', 'Z')
% subplot(3,1,2)
% plot(Magnetometer)
% legend('X', 'Y', 'Z')
% subplot(3,1,3)
% plot(Gyroscope * pi / 180)
% legend('X', 'Y', 'Z')
% 
% %%



q = [1 0 0 0];
e = a;

for i = 1:length(a)
    gyros = g(i, :);                   %first row of gyros
    mags = m(i, :);                    %first row of mags
    accels = a(i, :);                     %first row of accels
    
    q = madgwickFilter(q, accels, mags, gyros, 1/500);
    e(i, :) = quaternion2Euler(quaternionConjugate(q)) * 180 / pi; %quaternionConjugate(q)
end

time = 0:1/500:(length(a) - 1) * 1/500;

figure
subplot(3,1,1)
plot(time, e(:,1))
subplot(3,1,2)
plot(time, e(:,2))
subplot(3,1,3)
plot(time, e(:,3))

%% mag calibration

clc
clear all
close all

mag = csvread("m.csv");

% mag(:,:) = mag(:,:) / 100;

mx = mag(:,1);
my = mag(:,2);
mz = mag(:,3);

% figure(1)
% plot(mx, my)
% title('X vs Y')
% figure(2)
% plot(mx, mz)
% title('X vs Z')
% figure(3)
% plot(my, mz)
% title('Y vs Z')
% figure(4)
% plot3(mx, my, mz)
% xlabel('x')
% ylabel('y')
% zlabel('z')

mx_max = 31.34;
mx_min = -54.13;
mx_avg = (mx_max + mx_min) / 2;

my_max = 9.44;
my_min = -70.7;
my_avg = (my_max + my_min) / 2;

mz_max = 28.66;
mz_min = -49.77;
mz_avg = (mz_max + mz_min) / 2;


% figure(1)
% plot(mx, my)
% hold on
% plot( mx(:,1) - mx_avg, my(:,1) - my_avg)
% legend('uncalibrated', 'calibrated')
% 
% figure(2)
% plot(mx, mz)
% hold on
% plot( mx(:,1) - mx_avg, mz(:,1) - mz_avg)
% legend('uncalibrated', 'calibrated')
% 
% figure(3)
% plot(mz, my)
% hold on
% plot( mz(:,1) - mz_avg, my(:,1) - my_avg)
% legend('uncalibrated', 'calibrated')
% 
% figure(4)
% plot3(mx(:,1) - mx_avg, my(:,1) - my_avg, mz(:,1) - mz_avg)

figure(1)
plot(mx / 100, my / 100)
hold on
plot( (mx(:,1) - mx_avg) / 100, (my(:,1) - my_avg)/100)
legend('uncalibrated', 'calibrated')

figure(2)
plot(mx/100, mz/100)
hold on
plot( (mx(:,1) - mx_avg)/100, (mz(:,1) - mz_avg)/100)
legend('uncalibrated', 'calibrated')

figure(3)
plot(mz/100, my/100)
hold on
plot( (mz(:,1) - mz_avg)/100, (my(:,1) - my_avg)/100)
legend('uncalibrated', 'calibrated')

figure(4)
plot3((mx(:,1) - mx_avg)/100, (my(:,1) - my_avg)/100, (mz(:,1) - mz_avg)/100)


%% acc calibration

clc
clear all
close all

acc = csvread("acc.csv");

ax = acc(:,1);
ay = acc(:,2);
az = acc(:,3);

figure(1)
plot(ax, ay)
title('X vs Y')
figure(2)
plot(ax, az)
title('X vs Z')
figure(3)
plot(ay, az)
title('Y vs Z')
figure(4)
plot3(ax, ay, az)
xlabel('x')
ylabel('y')
zlabel('z')

%%

mx_max = 31.34;
mx_min = -54.13;
mx_avg = (mx_max + mx_min) / 2;

my_max = 9.44;
my_min = -70.7;
my_avg = (my_max + my_min) / 2;

mz_max = 28.66;
mz_min = -49.77;
mz_avg = (mz_max + mz_min) / 2;


% figure(1)
% plot(mx, my)
% hold on
% plot( mx(:,1) - mx_avg, my(:,1) - my_avg)
% legend('uncalibrated', 'calibrated')
% 
% figure(2)
% plot(mx, mz)
% hold on
% plot( mx(:,1) - mx_avg, mz(:,1) - mz_avg)
% legend('uncalibrated', 'calibrated')
% 
% figure(3)
% plot(mz, my)
% hold on
% plot( mz(:,1) - mz_avg, my(:,1) - my_avg)
% legend('uncalibrated', 'calibrated')
% 
% figure(4)
% plot3(mx(:,1) - mx_avg, my(:,1) - my_avg, mz(:,1) - mz_avg)

figure(1)
plot(mx / 100, my / 100)
hold on
plot( (mx(:,1) - mx_avg) / 100, (my(:,1) - my_avg)/100)
legend('uncalibrated', 'calibrated')

figure(2)
plot(mx/100, mz/100)
hold on
plot( (mx(:,1) - mx_avg)/100, (mz(:,1) - mz_avg)/100)
legend('uncalibrated', 'calibrated')

figure(3)
plot(mz/100, my/100)
hold on
plot( (mz(:,1) - mz_avg)/100, (my(:,1) - my_avg)/100)
legend('uncalibrated', 'calibrated')

figure(4)
plot3((mx(:,1) - mx_avg)/100, (my(:,1) - my_avg)/100, (mz(:,1) - mz_avg)/100)