clear all;
%Table = csvread('two_curve.csv',1);  % skips the first three rows of data
Table = csvread('prove_scatter.csv',1);  % skips the first three rows of data

chan_size = 11;
loop_size = 2;

arrival_rate = Table(1:chan_size, 3);

%option to show multi curve diff station
%for i = 1 : loop_size
    %D(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 5);
    %tpt(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 6);
%end
%
%hold on
%for i = 1 : loop_size
    %D_vs_tpt_p(i) = plot(tpt(:,i), D(:,i), 'LineWidth',5);
%end

%option to show one curve
D = Table(:, 5);
tpt = Table(:, 6);

D_vs_tpt_scat = scatter(tpt, D);


%break

legend('Backduration = 5', 'Backduration = 10');
hold off
grid on
title('Mean Delay vs. Throughput');
%set(gca, 'XScale', 'log');
%ylim([0 15])

xlabel('Throughput');
ylabel('Mean delay');
