%分数统计
zuigao = 20000;
data= xlsread('奖学金综测统计(1).xlsx');
D = data(:,9);
shuru = 5000;
%20030 25
%5000 12.4906
%4330 11.6237
%3750 10.8172
%3500 10.4504
%3250 10.0703
%2000 7.8998
%百分
baifenzhi = 100*shuru/zuigao;
keyanchengji = sqrt(baifenzhi)*10*0.25;
B = 100.*D./zuigao;
K = sqrt(B)*10;
%%
% zuigao2 = 5000;
% shuru = 