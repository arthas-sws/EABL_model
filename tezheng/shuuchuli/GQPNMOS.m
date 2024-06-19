
%%画图 画了NMOS 和 MOS 16个图 拟合出了二次方的GQP
% Linear model Poly2:
%      f(x) = p1*x^2 + p2*x + p3
% Coefficients (with 95% confidence bounds):
%        p1 =   -0.000998  (-0.001376, -0.0006199)
%        p2 =     0.05628  (0.02742, 0.08514)
%        p3 =      0.2081  (-0.3211, 0.7372)
clc;clear all;
data= xlsread('最终数据.xlsx','Sheet1');
TQP = data(:,1);
MOS = data(:,2);
GQP = data(:,5);
dianyunMOS = reshape(MOS,25,16);
NMOS = [];
index = [0 1 2 3 4];
pcdex = [1 5 10 15 20];
TQP =[26 32 38 44 50];
for i = [1 6 11 16 21]
    for j = [0 1 2 3 4]
        NMOS(i+j,1:16)=dianyunMOS(i+j,1:16)./dianyunMOS(i,1:16);
    end

end
TQP=[26 32 38 44 50];
DG=[0.9976 0.9815 0.9174 0.7423 0.5302];
name = {'bag','banana','biscuits','cake','cauliflower','flowerpot','house','litchi','mushroom','ping-pong_bat','puer_tea','pumpkin','ship','statue','stone','tool_box'};
%%
for j = 1:16
% SHIYAN = NMOS(:,j);
SHIYAN = dianyunMOS(:,j);
shuju = reshape(SHIYAN,5,5);%第一列为GQP26 第一行为TQP26
markers = {'o-', 'x-', 's-', '^-', 'v-'};
figure; % 创建一个新的图形窗口
hold on; % 保持图形窗口以便绘制多个图形
for i = 1:5
    plot(TQP, shuju(:, i), markers{i},'linewidth' ,1.5,'DisplayName', ['GQP =  ' num2str(TQP(i))]); % 绘制折线图
end

% 添加标签和图例
xlabel('TQP', 'FontSize', 20); % x轴标签
ylabel('MOS', 'FontSize', 20); % y轴标签

lgd = legend('Location', 'southwest');
% 调整图例字体大小
set(lgd, 'FontSize', 20); % 根据需要调整字体大小
set(gca, 'FontSize', 20);
grid off; % 显示网格

hold off; % 结束多图绘制
filename = [name{j}, '.png'];
print(filename, '-dpng');
end