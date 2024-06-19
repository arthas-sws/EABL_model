clc;clear all;
%%

data= xlsread('DCT.xlsx','9.16最优数据');
DCT = data(1:5,2:9);
DCT = reshape(DCT,40,1);
% YUV = data(2:end,14);
TQP = [26 32 38 44 50];
TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
% DCT = reshape(DCT,40,1);
MOSp1 = zeros(40,1);
YUVp = zeros(40,1);
for i = 1:40
k = mod(i-1, 5) + 1;
X(i) =  2.922e-05*TQP(k)*TQP(k)-0.001929*TQP(k)+0.1219;
J(i) = 0.08526*TQP(k)+0.05937;
YUVp(i) = X(i)*DCT(i)+J(i);
xielv(i) = 0.01265*YUVp(i)-0.4051;%斜率作为Y X为原始YUV数据
MOSp1(i) = xielv(i)*TQP(k)+90;
end
MOSreal = xlsread('DCT.xlsx','Sheet6');
MOSreal = MOSreal(:,1);
corrcoef(MOSreal, MOSp1)
% yuv = YUVp';
% plcc = corrcoef(YUV, yuv);
% MOSp = MOSp';
% MOSp = reshape(MOSp,5,16);