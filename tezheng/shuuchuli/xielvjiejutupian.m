%斜率和TQP 截距和TQP 的作图
clc;clear all;
data= xlsread('最终数据.xlsx');
datayuv = xlsread('DCT.xlsx','原始点云YUV数据');
TQP = data(:,1);
GQP = data(:,5);
MOS = data(:,2);
DCT = data(:,3);
TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
MOSXL =datayuv(2:end,6);
%这里只选取了32的YUV数据
YUV = datayuv(2:end,3);
tqp = reshape(TQP,25,16);
gqp = reshape(GQP,25,16);
mos = reshape(MOS,25,16);
dct = reshape(DCT,25,16);%一列是一种点云


    TQPC = tqp(:,[2,9,16,13,12,6,14,11]);
    GQPC = gqp(:,[2,9,16,13,12,6,14,11]);
    MOSC = mos(:,[2,9,16,13,12,6,14,11]);
    DCTC = dct(:,[2,9,16,13,12,6,14,11]);
    MOSXLC = MOSXL([2,9,16,13,12,6,14,11],1)';
    YUVC = YUV([2,9,16,13,12,6,14,11],1)';
    %%
    %DCT预测YUV数据
    for tqpnihe = 1:5
    %固定gqp为26 即索引为（1:5，：）不同的tqpnihe为不同的TQP取值
    
    
    coefficients = polyfit(DCTC(tqpnihe,:), YUVC, 1);
    xielv(tqpnihe) = coefficients(1);
    jieju(tqpnihe) = coefficients(2);
    %当tqpnihe为1时得到了DCTC和YUV拟合出的TQP26情况下的斜率和截距
    %使用TQP作
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
 scatter(TQPNH,xielv, 'filled'); % 绘制折线图  
 xlabel('TQP', 'FontSize', 20); % x轴标签
 ylabel('s(TQP)', 'FontSize', 20); % y轴标签
 % 设置 x 轴范围（例如，从20到60）
set(gca, 'FontSize', 20);
xlim([20, 56]);
% 设置 y 轴范围（例如，从0到1）
ylim([0.05, 0.11]);  
print('斜率和TQP', '-dpng');

figure();

scatter(TQPNH,jieju,'filled');
xlabel('TQP', 'FontSize', 20); % x轴标签
ylabel('i(TQP)', 'FontSize', 20); % y轴标签
 % 设置 x 轴范围（例如，从20到60）
 set(gca, 'FontSize', 20);
xlim([20, 56]);
% 设置 y 轴范围（例如，从0到1）
ylim([3.8, 5]);    
print('截距和TQP', '-dpng');