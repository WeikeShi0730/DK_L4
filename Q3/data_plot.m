clear all;
Table = csvread('Q3_data.csv',1);  % skips the first three rows of data

chan_size = 11;
loop_size = 2;

%experiment
%mean_delay = Table(1:chan_size, 15);

hold on
for i = 1 : loop_size
    mean_delay = Table(chan_size*(i-1)+1:chan_size*i, 9);
    tpt = Table(chan_size*(i-1)+1:chan_size*i, 8);
    plot(tpt, mean_delay, 'LineWidth',3);
end
legend('#station = 5', '#station = 10');
hold off
grid on
title('Throughput vs. Mean Delay');
%set(gca, 'XScale', 'log');

xlabel('Throughput');
ylabel('Mean delay');