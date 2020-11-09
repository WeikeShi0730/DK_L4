Table = csvread('Q2.csv',1);  % skips the first three rows of data

chan_size = 5;
loop_size = 1;

%experiment
arrival_rate = Table(1:chan_size, 3);

hold on
for i = 1 : loop_size
    delay = Table(chan_size*(i-1)+1:chan_size*i, 6);
    plot(arrival_rate, delay);
end

hold off
grid on
title('Arrival Rate vs. Mean Delay');
set(gca, 'XScale', 'log');

xlabel('Arrival rate');
ylabel('Mean delay');