clc;
clear all;

% data = xlsread('������ʵ����ѡ.xlsx','vsense��ѡ');
%  data = xlsread('������ʵ����ѡ.xlsx','vsense2��ѡ');
 data = xlsread('������ʵ����ѡ.xlsx','MPCCD��ѡ');
% data = xlsread('������ʵ����ѡ.xlsx','WPC2��ѡ');
data = data(15:24,:);
data(isnan(data)) = -1; 
 

% ����һ���µľ���������Ӱ�ɫ�ָ�
data_with_borders = zeros(size(data) +1)+1; % 
data_with_borders(1:end-1, 1:end-1) = data; % �������

% �������ݾ���
figure;
pcolor(data_with_borders);
colormap([0.5 0.5 0.5; 1 1 1; 0 0 0]); % ������ɫӳ�䣬��ɫ��0������ɫ��1���ͺ�ɫ��2��
 axis equal tight; % ������ı�����ȣ���ȥ������Ŀռ�
xticks(1+0.5:size(data_with_borders,2)+0.5);
yticks(1+0.5:size(data_with_borders,1)+0.5);
set(gca, 'FontSize', 18);
% ���ÿ̶ȱ�ǩ
xticklabels({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'});
yticklabels({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'});
saveas(gcf, 'MPCCD.png');
% ������������ã������Ҫ��
% set(gca, 'XAxisLocation', 'top'); % ��X��̶��ƶ�������