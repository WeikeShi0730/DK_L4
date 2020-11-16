clear all;
Table = csvread('Q4.csv',1);  % skips the first three rows of data

chan_size = 4;
loop_size = 1;

%experiment
arrivalrate = Table(1:chan_size, 3);

hold on
for i = 1 : loop_size
    delay = Table(chan_size*(i-1)+1:chan_size*i, 6);
    delay1 = Table(chan_size*(i-1)+1:chan_size*i, 7);
    plot(arrivalrate, delay, 'LineWidth',3);
    plot(arrivalrate, delay1, 'LineWidth',3);
end
legend('Other station delay', 'station 1 delay');
hold off
grid on
title('Arrival Rate vs. Mean Delay (number of station = 5)');
%set(gca, 'XScale', 'log');

xlabel('Arrival Rate');
ylabel('Mean delay');