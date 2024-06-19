clc;
clear all;
% data = xlsread('显著性实验挑选.xlsx','vsense挑选');
%  data = xlsread('显著性实验挑选.xlsx','vsense2挑选');
%  data = xlsread('显著性实验挑选.xlsx','MPCCD挑选');
 data = xlsread('显著性实验挑选.xlsx','WPC2挑选');


% 将 NaN 值替换为 -1

% 选取一部分数据进行展示

data(isnan(data)) = -1;

% 插入横轴和竖轴的间隔宽度
gapWidthX = 2;
gapWidthY = 2;

% 计算插入横轴和竖轴的间隔位置
gapPositionsX = linspace(0, size(data, 2), size(data, 2) + 1);
gapPositionsY = linspace(0, size(data, 1), size(data, 1) + 1);

% 创建图形窗口
figure();

% 使用 imagesc 代替 imshow
imagesc(data);
axis off;%关闭坐标显示
% 设置颜色映射
% 黑色表示优质算法 白色表示不如 灰色表示相似无法比较
colormap([0.5 0.5 0.5; 1 1 1; 0 0 0;]);

% 在横轴和竖轴的间隔位置处添加分割线
hold on;

for i = 1:numel(gapPositionsX) - 1
    midX = mean(gapPositionsX(i:i + 1));
    midY = mean(gapPositionsY(i:i + 1));
    plot([midX, midX], [0.5, size(data, 1) + 0.5], '--k', 'LineWidth', 1);
    plot([0.5, size(data, 2) + 0.5], [midY, midY], '--k', 'LineWidth', 1);
end

% 在图的两边增加算法名称
%'DCTP','GraphSIM','PCMRR','mcolor','μADcolor','mseF(p2point)','mseF,PSNR (p2point)','h(p2plane)','h.,PSNR(p2plane)','c[0],PSNRF' 

algorithmNames = {'DCTP','GraphSIM','PCMRR','mcolor','μADcolor','mseF(p2point)','mseF,PSNR (p2point)','h(p2plane)','h.,PSNR(p2plane)','c[0],PSNRF'};
for i = 1:numel(algorithmNames)
    text(0.5, i, algorithmNames{i}, 'HorizontalAlignment', 'right');
%     text(size(data, 2) + 1, i, algorithmNames{i}, 'HorizontalAlignment', 'left');
end

hold off;

% 添加标题和标签
% title('显著性实验结果图');
xlabel('算法编号');
ylabel('实验编号');
% saveas(gcf, '显著性实验结果图WPC2挑选.png');