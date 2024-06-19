%留一法 

clc;clear all;
%%
%留一法
% 创建一个16x16的矩阵
matrix = ones(16, 16);

% 为矩阵的每一列赋值
for i = 1:16
    matrix(:, i) = i;
end
for i = 1:16
    matrix(i, i) = 0;
end
matrix = matrix - diag(diag(matrix)); % 删除对角线上的0

% 生成一个16x15的新矩阵，删除了所有0
liuyifajuzhen = reshape(matrix(matrix ~= 0), 16, 15);
% 获取矩阵的行数
num_rows = size(liuyifajuzhen, 1);

% 重新排列矩阵
liuyifajuzhen = flipud(liuyifajuzhen);

%留一法矩阵第一行没有1，第二行没有2...第十六行没有16
%%
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

for liuyifashiyan = 1:16
    TQPC = tqp(:,liuyifajuzhen(liuyifashiyan,:));
    GQPC = gqp(:,liuyifajuzhen(liuyifashiyan,:));
    MOSC = mos(:,liuyifajuzhen(liuyifashiyan,:));
    DCTC = dct(:,liuyifajuzhen(liuyifashiyan,:));
    MOSXLC = MOSXL(liuyifajuzhen(liuyifashiyan,:),1)';
    YUVC = YUV(liuyifajuzhen(liuyifashiyan,:),1)';
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
%     coeffxielv = polyfit(TQPNH, xielv, 1);
    xielva = coeffxielv(1);
    xielvb = coeffxielv(2);
    xielvc = coeffxielv(3);

    coeffjieju = polyfit(TQPNH, jieju, 2);
    jiejua = coeffjieju(1);
    jiejub = coeffjieju(2);
    jiejuc = coeffjieju(3);
    %现在有了二次拟合的各项数据 可以写出YUV = xielv*DCT(i)+jieju
%     X(i) =  xielva*TQP(i)*TQP(i)-xielvb*TQP(i)+xielvc;
%     J(i) = jiejua*TQP(i)*TQP(i)-jiejub*TQP(i)+jiejuc;
%     YUVp(i) = X(i)*DCT(i)+J(i);
%
%%
%使用原始点云YUV和原始MOS斜率拟合MOS斜率
    yuvmos = polyfit(YUVC,MOSXLC,1);
   %%
    %验证集
    TQPY = tqp(:,liuyifashiyan);
    GQPY = gqp(:,liuyifashiyan);
    MOSY = mos(:,liuyifashiyan);
    DCTY = dct(:,liuyifashiyan);
    TQPY = reshape(TQPY,25,1);
    GQPY = reshape(GQPY,25,1);
    MOSY = reshape(MOSY,25,1);
    DCTY = reshape(DCTY,25,1);
   f1 =  0.6106;
    f2 = 45.1778;
    f3 = -3.9346;
    f4 =  0.3916;
    TQS = 2.^((TQPY-4)./6);
    for yanzheng = 1:25
%     k = mod(yanzheng-1, 5) + 1;
    X(yanzheng) =  xielva*TQPY(yanzheng)*TQPY(yanzheng)+xielvb*TQPY(yanzheng)+xielvc;
%      X(yanzheng) =  xielva*TQPY(yanzheng)+xielvb;
    J(yanzheng) = jiejua*TQPY(yanzheng)*TQPY(yanzheng)+jiejub*TQPY(yanzheng)+jiejuc;
    YUVp(yanzheng) = X(yanzheng)*DCTY(yanzheng)+J(yanzheng);
    mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %斜率作为Y X为原始YUV数据
    CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+90;
    DG(yanzheng) = f4+f1./(1+exp(1).^((-GQPY(yanzheng)+f2)./f3));
    FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
    end
    PLCC = corrcoef(FMOS,MOSY);
    SRCC(liuyifashiyan) = corr(FMOS',MOSY, 'Type', 'Spearman');
    RMSE(liuyifashiyan) = sqrt(mean((FMOS' - MOSY).^2));
    PLCC2 = corrcoef(CMOS,MOSY);
    PLCC3 = corrcoef(DG,MOSY);
    plcc(liuyifashiyan) = PLCC(1,2);
    plcc2(liuyifashiyan) = PLCC2(1,2);
    plcc3(liuyifashiyan) = PLCC3(1,2);
    srcc(liuyifashiyan) = corr(FMOS',MOSY, 'Type', 'Spearman');
    rmse(liuyifashiyan) = sqrt(mean((FMOS' - MOSY).^2));
end
std(plcc)
std(srcc)
std(rmse)

