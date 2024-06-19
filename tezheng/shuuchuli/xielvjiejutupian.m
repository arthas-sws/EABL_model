%б�ʺ�TQP �ؾ��TQP ����ͼ
clc;clear all;
data= xlsread('��������.xlsx');
datayuv = xlsread('DCT.xlsx','ԭʼ����YUV����');
TQP = data(:,1);
GQP = data(:,5);
MOS = data(:,2);
DCT = data(:,3);
TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
MOSXL =datayuv(2:end,6);
%����ֻѡȡ��32��YUV����
YUV = datayuv(2:end,3);
tqp = reshape(TQP,25,16);
gqp = reshape(GQP,25,16);
mos = reshape(MOS,25,16);
dct = reshape(DCT,25,16);%һ����һ�ֵ���


    TQPC = tqp(:,[2,9,16,13,12,6,14,11]);
    GQPC = gqp(:,[2,9,16,13,12,6,14,11]);
    MOSC = mos(:,[2,9,16,13,12,6,14,11]);
    DCTC = dct(:,[2,9,16,13,12,6,14,11]);
    MOSXLC = MOSXL([2,9,16,13,12,6,14,11],1)';
    YUVC = YUV([2,9,16,13,12,6,14,11],1)';
    %%
    %DCTԤ��YUV����
    for tqpnihe = 1:5
    %�̶�gqpΪ26 ������Ϊ��1:5��������ͬ��tqpniheΪ��ͬ��TQPȡֵ
    
    
    coefficients = polyfit(DCTC(tqpnihe,:), YUVC, 1);
    xielv(tqpnihe) = coefficients(1);
    jieju(tqpnihe) = coefficients(2);
    %��tqpniheΪ1ʱ�õ���DCTC��YUV��ϳ���TQP26����µ�б�ʺͽؾ�
    %ʹ��TQP��
    end
    
    TQPNH = [26 32 38 44 50];
    coeffxielv = polyfit(TQPNH, xielv, 2);
    xielva = coeffxielv(1);
    xielvb = coeffxielv(2);
    xielvc = coeffxielv(3);

    coeffjieju = polyfit(TQPNH, jieju, 2);
    jiejua = coeffjieju(1);
    jiejub = coeffjieju(2);
    jiejuc = coeffjieju(3);
   %%
 figure();
 scatter(TQPNH,xielv, 'filled'); % ��������ͼ  
 xlabel('TQP', 'FontSize', 20); % x���ǩ
 ylabel('s(TQP)', 'FontSize', 20); % y���ǩ
 % ���� x �᷶Χ�����磬��20��60��
set(gca, 'FontSize', 20);
xlim([20, 56]);
% ���� y �᷶Χ�����磬��0��1��
ylim([0.05, 0.11]);  
print('б�ʺ�TQP', '-dpng');

figure();

scatter(TQPNH,jieju,'filled');
xlabel('TQP', 'FontSize', 20); % x���ǩ
ylabel('i(TQP)', 'FontSize', 20); % y���ǩ
 % ���� x �᷶Χ�����磬��20��60��
 set(gca, 'FontSize', 20);
xlim([20, 56]);
% ���� y �᷶Χ�����磬��0��1��
ylim([3.8, 5]);    
print('�ؾ��TQP', '-dpng');