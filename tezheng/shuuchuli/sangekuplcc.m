clc;clear all;
 %data= xlsread('三个库的DCT.xlsx','MPCCD');
 % data= xlsread('三个库的DCT.xlsx','vsense_vvqdb');
data= xlsread('三个库的DCT.xlsx','vsenseVVDB2');
% allintra = [1 2 3 4 5 11 12 13 14 15 21 22 23 24 25 31 32 33 34 35];
% randomAc = [6 7 8 9 10 16 17 18 19 20 26 27 28 29 30 36 37 38 39 40];
% data = data(allintra,:);
% data = data(randomAc,:);
TQP = data(:,1);
MOSreal =data(:,3);
DCT = data(:,4);
% DCTT = reshape(DCT,25,16);
% Gcanshu = MPCCD(:,4);
GQP = data(:,1);
% Gcanshugai=max(Gcanshu, 0.45);
% TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
TQS = 2.^((TQP-4)./6);
%%
f1 =  0.6106;
f2 = 45.1778;
f3 = -3.9346;
f4 =  0.3916;
DG = f4+f1./(1+exp(1).^((-GQP+f2)./f3));
name = {'bag','banana','biscuits','cake','cauliflower','flowerpot','house','litchi','mushroom','ping-pong_bat','puer_tea','pumpkin','ship','statue','stone','tool_box'};
%%
for i = 1:length(MOSreal)
% k = mod(i-1, 5) + 1;
X(i) =  2.922e-05*TQP(i)*TQP(i)-0.001929*TQP(i)+0.1219;
J(i) = 0.08526*TQP(i)+0.05937;
YUVp(i) = X(i).*DCT(i)+J(i);
xielv(i) = 0.01265*YUVp(i)-0.4051;%斜率作为Y X为原始YUV数据
CMOS(i) = xielv(i)*TQS(i)+90;
FMOS(i) = CMOS(i)*DG(i);
end
N1 = corrcoef(MOSreal,FMOS);
N2 = corrcoef(MOSreal,DG);
N3 = corrcoef(MOSreal,CMOS);
%%
%绘制散点图
% scatter(FMOS,MOSreal, 'filled'); % 绘制散点图 
%  xlabel('FMOS'); % x轴标签
%  ylabel('MOSreal'); % y轴标签
%  title('vsenseVVDB2');
% print('vsenseVVDB2', '-dpng');

%%
% %MPCCD 
% block_size =6;
% 
% % 缩放到0到5的范围
% scaledMOS = (MOSreal - min(MOSreal)) / (max(MOSreal) - min(MOSreal));
% scaledFMOS = (FMOS - min(FMOS)) / (max(FMOS) - min(FMOS));
% scaledMOS =scaledMOS*5;
% scaledFMOS = scaledFMOS'*5;
% 
% for i=1:8
%     
%     block_start = (i - 1) * block_size + 1;%第一个块索引
%     block_end = i * block_size;%结尾索引
%     plcc = corrcoef(scaledMOS(block_start:block_end),scaledFMOS(block_start:block_end));
%     PLCC(i) = plcc(1,2);
%     SRCC(i) = corr(scaledMOS(block_start:block_end),scaledFMOS(block_start:block_end), 'Type', 'Spearman');
%     MSE(i) = sqrt(mean((scaledMOS(block_start:block_end) - scaledFMOS(block_start:block_end)).^2));
%     
% end
% MPCCDPLCC = mean(abs(PLCC));
% MPCCDSRCC = mean(abs(SRCC));
% MPCCDMSE = mean(abs(MSE));

%%

% % vsense_vvqdb
% block_size =4;
% 
% % 缩放到0到100的范围
% scaledMOS = (MOSreal - min(MOSreal)) / (max(MOSreal) - min(MOSreal));
% scaledFMOS = (FMOS - min(FMOS)) / (max(FMOS) - min(FMOS));
% scaledMOS =scaledMOS*100;
% scaledFMOS = scaledFMOS'*100;
% 
% for i=1:8
%     
%     block_start = (i - 1) * block_size + 1;%第一个块索引
%     block_end = i * block_size;%结尾索引
%     plcc = corrcoef(scaledMOS(block_start:block_end),scaledFMOS(block_start:block_end));
%     PLCC(i) = plcc(1,2);
%     SRCC(i) = corr(scaledMOS(block_start:block_end),scaledFMOS(block_start:block_end), 'Type', 'Spearman');
%     MSE(i) = sqrt(mean((scaledMOS(block_start:block_end) - scaledFMOS(block_start:block_end)).^2));
%     
% end
% MPCCDPLCC = mean(abs(PLCC));
% MPCCDSRCC = mean(abs(SRCC));
% MPCCDMSE = mean(abs(MSE));

%%
% % 
% %vsense_vvqdb2
block_size =5;

% 缩放到0到50的范围
scaledMOS = (MOSreal - min(MOSreal)) / (max(MOSreal) - min(MOSreal));
scaledFMOS = (FMOS - min(FMOS)) / (max(FMOS) - min(FMOS));
scaledMOS =scaledMOS*50;
scaledFMOS = scaledFMOS'*50;

for i=1:8
    
    block_start = (i - 1) * block_size + 1;%第一个块索引
    block_end = i * block_size;%结尾索引
    plcc = corrcoef(scaledMOS(block_start:block_end),scaledFMOS(block_start:block_end));
    PLCC(i) = plcc(1,2);
    SRCC(i) = corr(scaledMOS(block_start:block_end),scaledFMOS(block_start:block_end), 'Type', 'Spearman');
    MSE(i) = sqrt(mean((scaledMOS(block_start:block_end) - scaledFMOS(block_start:block_end)).^2));
    
end
MPCCDPLCC = mean(abs(PLCC));
MPCCDSRCC = mean(abs(SRCC));
MPCCDMSE = mean(abs(MSE));

