clear all;
%Table = csvread('back_off.csv',1);  % skips the first three rows of data
%Table = csvread('data_duration.csv',1);  % it affects 
%Table = csvread('slot_num.csv',1);  % skips the first three rows of data
Table = csvread('station.csv',1);  % it affects
%Table = csvread('slot_duration.csv',1);  % it affects

chan_size = 10;
loop_size = 5;

for i = 1 : loop_size
    arrival_rate(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 3);
    D(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 5);
    tpt(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 7);
end

figure(1)
%option to D vs tpt

hold on
for i = 1 : loop_size
   D_vs_tpt_p(i) = plot(tpt(:,i), D(:,i), 'LineWidth',1);
end

%legend('Backduration = 5', 'Backduration = 10');
%legend('X/X_r = 5/5', 'X/X_r = 10/5', 'X/X_r = 15/5', 'X/X_r = 20/5', 'X/X_r = 25/5');
legend('X/X_r = 5', 'X/X_r = 5/2', 'X/X_r = 5/3', 'X/X_r = 5/4', 'X/X_r = 5/5');
legend('# of station = 5', '# of station = 10', '# of station = 15', '# of station = 20', '# of station = 25');
hold off

grid on
title('Mean Delay vs. Throughput');
%set(gca, 'XScale', 'log');
%ylim([0 15])

xlabel('Throughput');
ylabel('Mean delay');

%break

%%option to tpt vs lamda
%for i = 1 : loop_size
    %arrival_rate(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 3);
    %D(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 5);
    %tpt(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 7);
%end
%
%hold on
%for i = 1 : loop_size
    %tpt_vs_lamda_p(i) = plot(arrival_rate(:,i), tpt(:,i), 'LineWidth',1);
%end

%%legend('Backduration = 5', 'Backduration = 10');
%%legend('X/X_r = 5', 'X/X_r = 5/2', 'X/X_r = 5/3', 'X/X_r = 5/4', 'X/X_r = 5/5');
%legend('X/X_r = 5/5', 'X/X_r = 10/5', 'X/X_r = 15/5', 'X/X_r = 20/5', 'X/X_r = 25/5');
%hold off
%
%grid on
%title('throughput vs. arrival rate');
%%set(gca, 'XScale', 'log');
%%ylim([0 15])
%
%xlabel('arrival rate');
%ylabel('throughput');

