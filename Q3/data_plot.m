clear all;
Table = csvread('Q3_data.csv',1);  % skips the first three rows of data

chan_size = 18;
loop_size = 2;

%experiment
%mean_delay = Table(1:chan_size, 15);

hold on
for i = 1 : loop_size
    mean_delay = Table(chan_size*(i-1)+1:chan_size*i, 9);
    tpt = Table(chan_size*(i-1)+1:chan_size*i, 7);
    plot(tpt, mean_delay, 'LineWidth',3);
end
%legend('5S BEB', '10S BEB');

%%%%%%%%%%%%compare Q1
Table = csvread('Q2_5.csv',1);  % skips the first three rows of data

chan_size = 12;
loop_size = 6;

%experiment
%arrival_rate = Table(1:chan_size, 3);

hold on
for i = 1 : loop_size
    %arrival_rate = Table(chan_size*(i-1)+1:chan_size*i, 13);
    delay = Table(chan_size*(i-1)+1:chan_size*i, 10);
    tpt = Table(chan_size*(i-1)+1:chan_size*i, 8);
    %plot(arrival_rate, delay, 'LineWidth',5);
    plot(tpt, delay, 'LineWidth',3);
end
legend('5S BEB', '10S BEB', '5S 3backoff', '5S 5backoff', '5S 7backoff','10S 5backoff', '10S 10backoff', '10S 20backoff');
hold off
grid on
title('Throughput vs. Mean Delay');
%set(gca, 'XScale', 'log');

xlabel('Throughput');
ylabel('Mean delay');