clc;clear all;
data= xlsread('��������.xlsx');
TQP = data(:,1);
MOSreal =data(:,2);
DCT = data(:,3);
DCTT = reshape(DCT,25,16);
Gcanshu = data(:,4);
GQP = data(:,5);
Gcanshugai=max(Gcanshu, 0.45);
TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
%%
f1 =  0.6106;
f2 = 45.1778;
f3 = -3.9346;
f4 =  0.3916;
DG = f4+f1./(1+exp(1).^((-GQP+f2)./f3));
name = {'bag','banana','biscuits','cake','cauliflower','flowerpot','house','litchi','mushroom','ping-pong_bat','puer_tea','pumpkin','ship','statue','stone','tool_box'};
%%
for i = 1:400
k = mod(i-1, 5) + 1;
X(i) =  2.922e-05*TQP(i)*TQP(i)-0.001929*TQP(i)+0.1219;
J(i) = 0.08526*TQP(i)+0.05937;
YUVp(i) = X(i)*DCT(i)+J(i);
xielv(i) = 0.01265*YUVp(i)-0.4051;%б����ΪY XΪԭʼYUV����
CMOS(i) = xielv(i)*TQS(k)+90;
end
CMOS = CMOS';
FMOS = CMOS.*Gcanshu;
FMOS2 = CMOS.*Gcanshugai;
%%
fenleiCMOS = reshape(CMOS,25,16);%�����Ʒֳ�ÿ��Ϊһ������
fenleiMOSreal = reshape(MOSreal,25,16);
fenleiFMOS = reshape(FMOS,25,16);
fenleiFMOS2 = reshape(FMOS2,25,16);
%%
Cplcc = corrcoef(MOSreal,CMOS);
Fplcc = corrcoef(MOSreal,FMOS);
F2plcc = corrcoef(MOSreal,FMOS2);
%%
for i =1:16;
    dianyunCMOS = fenleiCMOS(:,i);
    dianyunFMOS = fenleiFMOS(:,i);
    dianyunFMOS2 = fenleiFMOS2(:,i);
    dianyunMOSreal = fenleiMOSreal(:,i);
    A = corrcoef(dianyunCMOS,dianyunMOSreal);
    B = corrcoef(dianyunFMOS,dianyunMOSreal);
    C = corrcoef(dianyunFMOS2,dianyunMOSreal);
    CMOSPLCC(i) = A(1,2);
    FMOSPLCC(i) = B(1,2);
    F2MOSPLCC(i) = C(1,2);
end
CPLCCmean = mean(CMOSPLCC);
FPLCCmean = mean(FMOSPLCC);
F2PLCCmean = mean(F2MOSPLCC);
%%
%���
num_values = 16; % Ҫ��ȡ�����ֵ������
[sorted_values, sorted_indices] = sort(CMOSPLCC, 'descend'); % ��������

% ��ȡ����ǰʮ��ֵ�Ͷ�Ӧ������
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

for i = 1:num_values
    fprintf('ֻ��CQPģ��MOS��������ֵ %d: ֵ = %f, ���� = %d', i, top_values(i), top_indices(i));
    disp(name(top_indices(i)))
end
% fprintf('ֻ��CQPģ��MOS��������ֵ ����Ϊ= ');
% disp(name(top_indices))
num_values = 16; % Ҫ��ȡ�����ֵ������
[sorted_values, sorted_indices] = sort(FMOSPLCC, 'descend'); % ��������

% ��ȡ����ǰʮ��ֵ�Ͷ�Ӧ������
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

for i = 1:num_values
    fprintf('����MOS��������ֵ %d: ֵ = %f, ���� = %d', i, top_values(i), top_indices(i));
    disp(name(top_indices(i)))
end
% fprintf('����ģ��QPģ��MOS��������ֵ ����Ϊ= ');
% disp(name(top_indices))
num_values = 16; % Ҫ��ȡ�����ֵ������
[sorted_values, sorted_indices] = sort(F2MOSPLCC, 'descend'); % ��������

% ��ȡ����ǰʮ��ֵ�Ͷ�Ӧ������
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

for i = 1:num_values
    fprintf('����MOS2��������ֵ %d: ֵ = %f, ���� = %d', i, top_values(i), top_indices(i));
    disp(name(top_indices(i)))
end
% fprintf('����MOS2ģ��MOS��������ֵ ����Ϊ= ');
% disp(name(top_indices))
%%
%��һ��̽�������ڵĹ�ϵ
vector = (1:25)';
indices = reshape(vector,5,5);
for i = 1:16
    %����׼��
    dianyunCMOS = fenleiCMOS(:,i);
    dianyunFMOS = fenleiFMOS(:,i);
    dianyunFMOS2 = fenleiFMOS2(:,i);
    dianyunMOSreal = fenleiMOSreal(:,i);
    dianyunCMOS = reshape(dianyunCMOS,5,5);
    dianyunFMOS = reshape(dianyunFMOS,5,5);
    dianyunFMOS2 = reshape(dianyunFMOS2,5,5);
    dianyunMOSreal = reshape(dianyunMOSreal,5,5);
    for k = 1:5
    A1 = corrcoef(dianyunCMOS(:,k),dianyunMOSreal(:,k));%��֮�������ԣ������Ԫ��Ϊ��ͬGQP ��ͬCQP 
    A2 = corrcoef(dianyunCMOS(k,:),dianyunMOSreal(k,:));%��֮�������ԣ������Ԫ��Ϊ��ͬGQP ��ͬCQP
    B1 = corrcoef(dianyunFMOS(:,k),dianyunMOSreal(:,k));%��֮�������ԣ������Ԫ��Ϊ��ͬGQP ��ͬCQP 
    B2 = corrcoef(dianyunFMOS(k,:),dianyunMOSreal(k,:));%��֮�������ԣ������Ԫ��Ϊ��ͬGQP ��ͬCQP
    C1 = corrcoef(dianyunFMOS2(:,k),dianyunMOSreal(:,k));%��֮�������ԣ������Ԫ��Ϊ��ͬGQP ��ͬCQP 
    C2 = corrcoef(dianyunFMOS2(k,:),dianyunMOSreal(k,:));%��֮�������ԣ������Ԫ��Ϊ��ͬGQP ��ͬCQP
    DYC(i,k) =  A1(1,2);%i�����˲�ͬ�ĵ��� k�����˲�ͬ��QP һ��������ͬGQP ��ͬCQP  ����ʮ�ǲ�ͬGQP ��ͬCQP
    DYC(i,k+5) = A2(1,2);
    DYF(i,k) = B1(1,2);
    DYF(i,k+5) = B2(1,2);
    DYF2(i,k) = C1(1,2);
    DYF2(i,k+5) = C2(1,2);
    end
end
FGMOS = DG .*CMOS;
N1 = corrcoef(MOSreal,CMOS)
N2 = corrcoef(MOSreal,FMOS)
N3 = corrcoef(MOSreal,FMOS2)
N4 = corrcoef(MOSreal,Gcanshu)
N5 = corrcoef(MOSreal,Gcanshugai)
N6 = corrcoef(MOSreal,FGMOS)
N7 = corrcoef(MOSreal,DG)
