backoffLock = textread('BackoffLockTimeCosts.txt');
TTASLock = textread('TTASLockTimeCosts.txt');
TASLock = textread('TASLockTimeCosts.txt');
CLHLock = textread('CLHLockTimeCosts.txt');

figure;
hold on;
x = 1 : 2 : length(CLHLock)*2 - 1;
plot(x,CLHLock,'-+');
plot(x,TTASLock,'-o');
plot(x,TASLock,'-.');
plot(x,backoffLock);

xlabel('线程数量');
ylabel('执行时间/ms');
title('实践对比');
legend('CLHLock','TTASLock','TASLock','backoffLock');
%valatile