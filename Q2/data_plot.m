clear all;
Table = csvread('Q2_10.csv',1);  % skips the first three rows of data
% 
% chan_size = 10;
% loop_size = 3;
% 
% %experiment
% arrival_rate = Table(1:chan_size, 3);
% 
% hold on
% for i = 1 : loop_size
%     delay = Table(chan_size*(i-1)+1:chan_size*i, 8);
%     plot(arrival_rate, delay, 'LineWidth',5);
% end
% legend('Backduration = 5', 'Backduration = 10', 'Backduration = 20');
% hold off
% grid on
% title('Arrival Rate vs. Mean Delay (Stations 10)');
% %set(gca, 'XScale', 'log');
% 
% xlabel('Arrival rate');
% ylabel('Mean delay');
arrival_rate_5 = Table(1:3, 3);
arrival_rate_10 = Table(11:16, 3);
arrival_rate_20 = Table(21:28, 3);
delay_5 = Table(1:3, 8);
delay_10 = Table(11:16, 8);
delay_20 = Table(21:28, 8);

plot(arrival_rate_5, delay_5, 'LineWidth',5);
hold on
plot(arrival_rate_10, delay_10, 'LineWidth',5);
plot(arrival_rate_20, delay_20, 'LineWidth',5);
legend('Backduration = 5', 'Backduration = 10', 'Backduration = 20');
hold off

grid on
title('Arrival Rate vs. Mean Delay (Stations 10)');

xlabel('Arrival rate');
ylabel('Mean delay');