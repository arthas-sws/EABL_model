clc;clear all;
data= xlsread('94组合.xlsx','Sheet3');
DCT = data(1:5,:);
YUV =data(6,:);
mosxielv =data(7,:);
TQP = [26 32 38 44 50];
% 创建图像
 for i =1:5
figure(i);
scatter(DCT(i,:), YUV, 'filled'); % 使用scatter绘制散点图
xlabel('TE', 'FontSize', 20); % 添加x轴标签
ylabel('CD', 'FontSize', 20); % 添加y轴标签
set(gca, 'FontSize', 20);
% title('TQP = 'TQP{I}''); % 添加图像标题
 print(['TQP' num2str(TQP(i))], '-dpng');
PLCC = corrcoef(DCT(i,:), YUV);
plcc(i)=PLCC(1,2);
 end
% 添加数据点标签
% for i = 1:numel(DCT)
%     text(DCT(1,:)(i), YUV(i), num2str(i), 'HorizontalAlignment', 'left', 'VerticalAlignment', 'bottom');
% end
PLCC2 = corrcoef(YUV,mosxielv);