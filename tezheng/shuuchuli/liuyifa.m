%��һ�� 

clc;clear all;
%%
%��һ��
% ����һ��16x16�ľ���
matrix = ones(16, 16);

% Ϊ�����ÿһ�и�ֵ
for i = 1:16
    matrix(:, i) = i;
end
for i = 1:16
    matrix(i, i) = 0;
end
matrix = matrix - diag(diag(matrix)); % ɾ���Խ����ϵ�0

% ����һ��16x15���¾���ɾ��������0
liuyifajuzhen = reshape(matrix(matrix ~= 0), 16, 15);
% ��ȡ���������
num_rows = size(liuyifajuzhen, 1);

% �������о���
liuyifajuzhen = flipud(liuyifajuzhen);

%��һ�������һ��û��1���ڶ���û��2...��ʮ����û��16
%%
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

for liuyifashiyan = 1:16
    TQPC = tqp(:,liuyifajuzhen(liuyifashiyan,:));
    GQPC = gqp(:,liuyifajuzhen(liuyifashiyan,:));
    MOSC = mos(:,liuyifajuzhen(liuyifashiyan,:));
    DCTC = dct(:,liuyifajuzhen(liuyifashiyan,:));
    MOSXLC = MOSXL(liuyifajuzhen(liuyifashiyan,:),1)';
    YUVC = YUV(liuyifajuzhen(liuyifashiyan,:),1)';
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
%     coeffxielv = polyfit(TQPNH, xielv, 1);
    xielva = coeffxielv(1);
    xielvb = coeffxielv(2);
    xielvc = coeffxielv(3);

    coeffjieju = polyfit(TQPNH, jieju, 2);
    jiejua = coeffjieju(1);
    jiejub = coeffjieju(2);
    jiejuc = coeffjieju(3);
    %�������˶�����ϵĸ������� ����д��YUV = xielv*DCT(i)+jieju
%     X(i) =  xielva*TQP(i)*TQP(i)-xielvb*TQP(i)+xielvc;
%     J(i) = jiejua*TQP(i)*TQP(i)-jiejub*TQP(i)+jiejuc;
%     YUVp(i) = X(i)*DCT(i)+J(i);
%
%%
%ʹ��ԭʼ����YUV��ԭʼMOSб�����MOSб��
    yuvmos = polyfit(YUVC,MOSXLC,1);
   %%
    %��֤��
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
    mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %б����ΪY XΪԭʼYUV����
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

