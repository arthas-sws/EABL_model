
%%��ͼ ����NMOS �� MOS 16��ͼ ��ϳ��˶��η���GQP
% Linear model Poly2:
%      f(x) = p1*x^2 + p2*x + p3
% Coefficients (with 95% confidence bounds):
%        p1 =   -0.000998  (-0.001376, -0.0006199)
%        p2 =     0.05628  (0.02742, 0.08514)
%        p3 =      0.2081  (-0.3211, 0.7372)
clc;clear all;
data= xlsread('��������.xlsx','Sheet1');
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
shuju = reshape(SHIYAN,5,5);%��һ��ΪGQP26 ��һ��ΪTQP26
markers = {'o-', 'x-', 's-', '^-', 'v-'};
figure; % ����һ���µ�ͼ�δ���
hold on; % ����ͼ�δ����Ա���ƶ��ͼ��
for i = 1:5
    plot(TQP, shuju(:, i), markers{i},'linewidth' ,1.5,'DisplayName', ['GQP =  ' num2str(TQP(i))]); % ��������ͼ
end

% ��ӱ�ǩ��ͼ��
xlabel('TQP', 'FontSize', 20); % x���ǩ
ylabel('MOS', 'FontSize', 20); % y���ǩ

lgd = legend('Location', 'southwest');
% ����ͼ�������С
set(lgd, 'FontSize', 20); % ������Ҫ���������С
set(gca, 'FontSize', 20);
grid off; % ��ʾ����

hold off; % ������ͼ����
filename = [name{j}, '.png'];
print(filename, '-dpng');
end