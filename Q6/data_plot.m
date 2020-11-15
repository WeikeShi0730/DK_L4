clear all;
%Table = csvread('back_off.csv',1);  % skips the first three rows of data
%Table = csvread('data_duration.csv',1);  % it affects 
%Table = csvread('slot_num.csv',1);  % skips the first three rows of data
%Table = csvread('station.csv',1);  % it affects
Table = csvread('slot_duration.csv',1);  % it affects

chan_size = 10;
loop_size = 5;


figure(1)
%option to show multi curve diff station
for i = 1 : loop_size
    arrival_rate(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 3);
    D(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 5);
    tpt(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 7);
end

hold on
for i = 1 : loop_size
    D_vs_tpt_p(i) = plot(tpt(:,i), D(:,i), 'LineWidth',1);
end

legend('Backduration = 5', 'Backduration = 10');
hold off

figure(2)
hold on
for i = 1 : loop_size
    D_vs_lamda_p(i) = plot(arrival_rate(:,i), D(:,i), 'LineWidth',1);
end
grid on
title('Mean Delay vs. Throughput');
%set(gca, 'XScale', 'log');
%ylim([0 15])

xlabel('Throughput');
ylabel('Mean delay');
