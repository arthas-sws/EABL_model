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
    TQPY = tqp(:,[1 3 4 5 7 8 10 15]);
    GQPY = gqp(:,[1 3 4 5 7 8 10 15]);
    MOSY = mos(:,[1 3 4 5 7 8 10 15]);
    DCTY = dct(:,[1 3 4 5 7 8 10 15]);
    TQPT = reshape(TQPY,200,1);
    GQPY = reshape(GQPY,200,1);
    MOSY = reshape(MOSY,200,1);
    MOSMAX = 85;
    DCTY = reshape(DCTY,200,1);
   f1 =  0.6106;
    f2 = 45.1778;
    f3 = -3.9346;
    f4 =  0.3916;
    TQS = 2.^((TQP-4)./6);
%     for yanzheng = 1:200
% %     k = mod(yanzheng-1, 5) + 1;
%     X(yanzheng) =  xielva*TQP(yanzheng)*TQP(yanzheng)+xielvb*TQP(yanzheng)+xielvc;
%     J(yanzheng) = jiejua*TQP(yanzheng)*TQP(yanzheng)+jiejub*TQP(yanzheng)+jiejuc;
%     YUVp(yanzheng) = X(yanzheng)*DCTY(yanzheng)+J(yanzheng);
%     mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %斜率作为Y X为原始YUV数据
%     CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+90;
%     DG(yanzheng) = f4+f1./(1+exp(1).^((-GQPY(yanzheng)+f2)./f3));
%     FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
%     end
%     PLCC = corrcoef(FMOS,MOSY);

    for yanzheng = 1:400
%     k = mod(yanzheng-1, 5) + 1;
    X(yanzheng) =  xielva*TQP(yanzheng)*TQP(yanzheng)+xielvb*TQP(yanzheng)+xielvc;
    J(yanzheng) = jiejua*TQP(yanzheng)*TQP(yanzheng)+jiejub*TQP(yanzheng)+jiejuc;
    YUVp(yanzheng) = X(yanzheng)*DCT(yanzheng)+J(yanzheng);
    mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %斜率作为Y X为原始YUV数据
    CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+MOSMAX;
    CMOS2(yanzheng) = (-0.2502)*TQS(yanzheng)+MOSMAX;
    DG(yanzheng) = f4+f1./(1+exp(1).^((-GQP(yanzheng)+f2)./f3));
    FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
    end
    
    [fplcc, fmae, frms, fsrcc, fkrcc] = IQA_eval(MOS, FMOS', 'regress_logistic');
    [tqpplcc, tqpfmae, tqpfrms, tqpfsrcc, tqpfkrcc] = IQA_eval(MOS, TQP, 'regress_logistic');
    [teplcc, temae,terms, tesrcc, tekrcc] = IQA_eval(MOS, CMOS', 'regress_logistic');
    [dfplcc, dfmae, dfrms, dsrcc, dkrcc] = IQA_eval(MOS, DG', 'regress_logistic');
     
%     PLCC1 = corrcoef(FMOS,MOS);
%     PLCC2 = corrcoef(CMOS,MOS);
%     PLCC3 = corrcoef(DG,MOS);
%     PLCC4 = corrcoef(TQP,MOS);
%     srcc1 = corr(FMOS',MOS, 'Type', 'Spearman');
%     rmse1 = sqrt(mean((FMOS - MOS').^2));
%     srcc2 = corr(CMOS',MOS, 'Type', 'Spearman');
%     rmse2 = sqrt(mean((CMOS - MOS').^2));
%     srcc3 = corr(DG',MOS, 'Type', 'Spearman');
%     rmse3 = sqrt(mean((DG - MOS').^2));
%     PLCC5 = corrcoef(CMOS2,MOS);
%     srcc4 = corr(CMOS2',MOS, 'Type', 'Spearman');
%     rmse4 = sqrt(mean((CMOS2 - MOS').^2));
%     srcc4 = corr(TQP,MOS, 'Type', 'Spearman');
%     rmse4 = sqrt(mean((TQP - MOS).^2));    
    
    
    %%
%     FMOSPC = reshape(FMOS,25,16);
%     YUVPC = reshape(YUVp,25,16);
%     DCTPC = reshape(DCT,25,16);
%     CMOSPC = reshape(CMOS,25,16);
%     MOSPC = reshape(MOS,25,16);
    
    %%