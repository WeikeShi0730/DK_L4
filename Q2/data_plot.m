clear all;
Table = csvread('Q2_5.csv',1);  % skips the first three rows of data

chan_size = 15;
loop_size = 3;

%experiment
arrival_rate = Table(1:chan_size, 3);

hold on
for i = 1 : loop_size
    %arrival_rate = Table(chan_size*(i-1)+1:chan_size*i, 13);
    delay = Table(chan_size*(i-1)+1:chan_size*i, 20);
    tpt = Table(chan_size*(i-1)+1:chan_size*i, 18);
    %plot(arrival_rate, delay, 'LineWidth',5);
    plot(arrival_rate, delay, 'LineWidth',3);
end
legend('Backduration = 5', 'Backduration = 10', 'Backduration = 20');
hold off
grid on
title('Arrival Rate vs. Mean Delay (Stations 10)');
%set(gca, 'XScale', 'log');

xlabel('Arrival rate');
ylabel('Mean delay');
% arrival_rate_5 = Table(1:6, 3);
% arrival_rate_10 = Table(11:18, 3);
% arrival_rate_20 = Table(21:30, 3);
% delay_5 = Table(1:6, 8);
% delay_10 = Table(11:18, 8);
% delay_20 = Table(21:30, 8);
% 
% plot(arrival_rate_5, delay_5, 'LineWidth',3);
% hold on
% plot(arrival_rate_10, delay_10, 'LineWidth',3);
% plot(arrival_rate_20, delay_20, 'LineWidth',3);
% legend('Backduration = 3', 'Backduration = 5', 'Backduration = 7');
% hold off
% 
% grid on
% title('Arrival Rate vs. Mean Delay (Stations 5)');
% 
% xlabel('Arrival rate');
% ylabel('Mean delay');