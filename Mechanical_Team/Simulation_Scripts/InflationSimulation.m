close all;
clear all;
clc;

%% rectangular full simulation
%all dimensions listed in meters
%Front Mid
Lpipe = 0.05;
Rpipe = 0.02;
Lpocket = 9 * 0.0254;
Hpocket = 12 * 0.0254;
Dpocket = 45.07/1000;  
Vpocket = 0.5*pi*(Hpocket/2)*Dpocket * Lpocket;

i = 1;

for Qpump = 50:25:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i)
%     Qpump
    i = i+1;
end

figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 50:25:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

%Front Chest
Lpipe = 0.05;
Rpipe = 0.02;
Lpocket = 9 * 0.0254;
Hpocket = 8 * 0.0254;
Dpocket = 36.64/1000;  
Vpocket = 0.5*pi*(Hpocket/2)*Dpocket * Lpocket;

i = 1;

for Qpump = 50:25:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i);
%     Qpump;
    i = i+1;
end

% figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 50:25:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

%Front Hip
Lpipe = 0.05;
Rpipe = 0.02;
Lpocket = 9 * 0.0254;
Hpocket = 6 * 0.0254;
Dpocket = 28.12/1000;  
Vpocket = 0.5*pi*(Hpocket/2)*Dpocket * Lpocket;

i = 1;

for Qpump = 50:25:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i)
%     Qpump
    i = i+1;
end

% figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 50:25:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

%Back Top
Lpipe = 0.05;
Rpipe = 0.02;
Lpocket = 18 * 0.0254;
Hpocket = 8 * 0.0254;
Dpocket = 54.71/1000;  
Vpocket = 0.5*pi*(Hpocket/2)*Dpocket * Lpocket;

i = 1;

for Qpump = 50:25:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i)
%     Qpump
    i = i+1;
end

% figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 50:25:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

%Back Mid
Lpipe = 0.05;
Rpipe = 0.02;
Lpocket = 18 * 0.0254;
Hpocket = 12 * 0.0254;
Dpocket = 83.13/1000;  
Vpocket = 0.5*pi*(Hpocket/2)*Dpocket * Lpocket;

i = 1;

for Qpump = 50:25:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i)
%     Qpump
    i = i+1;
end

% figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 50:25:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

%Back Hip
Lpipe = 0.05;
Rpipe = 0.02;
Lpocket = 18 * 0.0254;
Hpocket = 6 * 0.0254;
Dpocket = 39.07/1000;  
Vpocket = 0.5*pi*(Hpocket/2)*Dpocket * Lpocket;

i = 1;

for Qpump = 50:25:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i)
%     Qpump
    i = i+1;
end

% figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 50:25:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
ylim([0 0.1])
legend('Front Middle','80 ms Target', 'Front Chest','80 ms Target', 'Front Hip', '80 ms Target','Back Top', '80 ms Target','Back Middle', '80 ms Target', 'Back Hip', '80 ms Target')
hold off

%% circular design
% all dimensions listed in meters
% 15 kPa
Lpipe = 0.05;
Rpipe = 0.02;
r = 1.5 * 0.0254; %radius of bubble
Dpocket = 7.185/1000;  
n = 1; %number of bubbles

Vpocket = 0.5*(4/3)*pi*r*r*Dpocket*n;

i = 1;

for Qpump = 5:20:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
    i = i+1;
end

figure('Name', 'Air Pump Pressure vs Inflation Time')
Qpump = 5:20:500;
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

%12 kPa
Lpipe = 0.05;
Rpipe = 0.02;
r = 1.5 * 0.0254;
Dpocket = 7.185/1000;  
n = 1; %number of bubbles

Vpocket = 0.5*(4/3)*pi*r*r*Dpocket*n;

i = 1;

for Qpump = 5:20:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i);
%     Qpump;
    i = i+1;
end

Qpump = 5:20:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

% 10 kPa
Lpipe = 0.05;
Rpipe = 0.02;
r = 1.5 * 0.0254;
Dpocket = 6.648/1000;  
n = 1; %number of bubbles

Vpocket = 0.5*(4/3)*pi*r*r*Dpocket*n;

i = 1;

for Qpump = 5:20:500
    Tpipe(i) = (pi * Rpipe^2 *Lpipe)/Qpump;
    Tpocket(i) = 60000*Vpocket/Qpump;
    Ttotal(i) = Tpocket(i) + Tpipe(i);
%     Ttotal(i)
%     Qpump
    i = i+1;
end

Qpump = 5:20:500
plot(Qpump, Ttotal)
hold on
plot((60000*Vpocket/0.08),0.08,'o','MarkerSize',10)
title('Air Pump Flow Rate vs Inflation Time')
xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
% hold off

xlabel('Flow Rate (L/min)')
ylabel('Time (sec)')
ylim([0 0.1])
legend('15kPa applied','80 ms Target @ 15kPa', '12kPa applied','80 ms Target @ 12kPa', '10kPa applied','80 ms Target @ 10kPa')
hold off
