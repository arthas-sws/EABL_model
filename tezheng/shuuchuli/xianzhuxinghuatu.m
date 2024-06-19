clc;
clear all;
% data = xlsread('������ʵ����ѡ.xlsx','vsense��ѡ');
%  data = xlsread('������ʵ����ѡ.xlsx','vsense2��ѡ');
%  data = xlsread('������ʵ����ѡ.xlsx','MPCCD��ѡ');
 data = xlsread('������ʵ����ѡ.xlsx','WPC2��ѡ');


% �� NaN ֵ�滻Ϊ -1

% ѡȡһ�������ݽ���չʾ

data(isnan(data)) = -1;

% ������������ļ�����
gapWidthX = 2;
gapWidthY = 2;

% ���������������ļ��λ��
gapPositionsX = linspace(0, size(data, 2), size(data, 2) + 1);
gapPositionsY = linspace(0, size(data, 1), size(data, 1) + 1);

% ����ͼ�δ���
figure();

% ʹ�� imagesc ���� imshow
imagesc(data);
axis off;%�ر�������ʾ
% ������ɫӳ��
% ��ɫ��ʾ�����㷨 ��ɫ��ʾ���� ��ɫ��ʾ�����޷��Ƚ�
colormap([0.5 0.5 0.5; 1 1 1; 0 0 0;]);

% �ں��������ļ��λ�ô���ӷָ���
hold on;

for i = 1:numel(gapPositionsX) - 1
    midX = mean(gapPositionsX(i:i + 1));
    midY = mean(gapPositionsY(i:i + 1));
    plot([midX, midX], [0.5, size(data, 1) + 0.5], '--k', 'LineWidth', 1);
    plot([0.5, size(data, 2) + 0.5], [midY, midY], '--k', 'LineWidth', 1);
end

% ��ͼ�����������㷨����
%'DCTP','GraphSIM','PCMRR','mcolor','��ADcolor','mseF(p2point)','mseF,PSNR (p2point)','h(p2plane)','h.,PSNR(p2plane)','c[0],PSNRF' 

algorithmNames = {'DCTP','GraphSIM','PCMRR','mcolor','��ADcolor','mseF(p2point)','mseF,PSNR (p2point)','h(p2plane)','h.,PSNR(p2plane)','c[0],PSNRF'};
for i = 1:numel(algorithmNames)
    text(0.5, i, algorithmNames{i}, 'HorizontalAlignment', 'right');
%     text(size(data, 2) + 1, i, algorithmNames{i}, 'HorizontalAlignment', 'left');
end

hold off;

% ��ӱ���ͱ�ǩ
% title('������ʵ����ͼ');
xlabel('�㷨���');
ylabel('ʵ����');
% saveas(gcf, '������ʵ����ͼWPC2��ѡ.png');