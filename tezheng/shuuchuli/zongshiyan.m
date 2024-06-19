clc;clear all;
MOSread = xlsread('WPC2.0_MOS.xlsx');
DCT = xlsread('DCT.xlsx');
 MOS = DCT(:,1);

% MOS = MOSread(:,3);
TQP = MOSread(:,2);

for k = 2:9
    DCT = xlsread('DCT.xlsx');
    DCT = DCT(:,k);
 
    MOSt = zeros(400, 1);
for i = 1:400
yuvstd(i) = 0.0376*DCT(i)+2.1943;
% yuvstd(i) = 5.175e+09*DCT(i)^(-1.08)+62.84;
MOSxielv(i) = 0.0275*yuvstd(i)-0.415;



TQS(i) = 2^((TQP(i)-4)/6);

MOSt(i) = MOSxielv(i)*TQS(i)+90;

end;


plcc = corrcoef(MOSt, MOS);
DSTmse1(k) = sqrt(mean((MOS - MOSt).^2));
plcc_value(k) = plcc(1, 2);
srcc1(k) = corr(MOS, MOSt, 'Type', 'Spearman');
end;
%%
for k = 2:9
    DCT = xlsread('DCT.xlsx','sheet2');
    DCT = DCT(:,k);
    MOS = DCT(:,1);
    MOSt = zeros(200, 1);
for i = 1:200
yuvstd(i) = 0.0376*DCT(i)+2.1943;
% yuvstd(i) = 5.175e+09*DCT(i)^(-1.08)+62.84;
MOSxielv(i) = 0.0275*yuvstd(i)-0.415;



TQS(i) = 2^((TQP(i)-4)/6);

MOSt(i) = MOSxielv(i)*TQS(i)+90;

end;

% MOSt = MOSt';
plcc = corrcoef(MOSt, MOS);
plcc_value2(k) = plcc(1, 2);
DSTmse2(k) = sqrt(mean((MOS - MOSt).^2));
srcc2(k) = corr(MOS, MOSt, 'Type', 'Spearman');
end;
