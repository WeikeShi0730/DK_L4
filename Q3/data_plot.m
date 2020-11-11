clear all;
Table = csvread('Q3.csv',1);  % skips the first three rows of data

chan_size = 10;
loop_size = 1;

%experiment
mean_delay = Table(1:chan_size, 7);

hold on
for i = 1 : loop_size
    delay = Table(chan_size*(i-1)+1:chan_size*i, 6);
    plot(mean_delay, delay, 'LineWidth',5);
end
legend('Backduration = 5', 'Backduration = 10', 'Backduration = 20');
hold off
grid on
title('Arrival Rate vs. Mean Delay');
%set(gca, 'XScale', 'log');

xlabel('Arrival rate');
ylabel('Mean delay');