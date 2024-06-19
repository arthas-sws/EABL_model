clc;
clear all;

% data = xlsread('显著性实验挑选.xlsx','vsense挑选');
%  data = xlsread('显著性实验挑选.xlsx','vsense2挑选');
 data = xlsread('显著性实验挑选.xlsx','MPCCD挑选');
% data = xlsread('显著性实验挑选.xlsx','WPC2挑选');
data = data(15:24,:);
data(isnan(data)) = -1; 
 

% 创建一个新的矩阵，用于添加白色分隔
data_with_borders = zeros(size(data) +1)+1; % 
data_with_borders(1:end-1, 1:end-1) = data; % 填充数据

% 绘制数据矩阵
figure;
pcolor(data_with_borders);
colormap([0.5 0.5 0.5; 1 1 1; 0 0 0]); % 设置颜色映射，灰色（0）、白色（1）和黑色（2）
 axis equal tight; % 设置轴的比例相等，并去掉多余的空间
xticks(1+0.5:size(data_with_borders,2)+0.5);
yticks(1+0.5:size(data_with_borders,1)+0.5);
set(gca, 'FontSize', 18);
% 设置刻度标签
xticklabels({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'});
yticklabels({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'});
saveas(gcf, 'MPCCD.png');
% 额外的美化设置（如果需要）
% set(gca, 'XAxisLocation', 'top'); % 将X轴刻度移动到顶部