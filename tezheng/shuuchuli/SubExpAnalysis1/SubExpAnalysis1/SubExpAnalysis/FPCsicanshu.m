clc;clear all;
% data = xlsread('�ĸ���������.xlsx','vsense1');
% data = xlsread('�ĸ���������.xlsx','vsense2');
% data = xlsread('�ĸ���������.xlsx','MPCCD');
data = xlsread('�ĸ���������.xlsx','WPC2');
MOS =  data(:,1);
EABL = data(:,2);
% �����ݷ�Ϊ����飬ÿ������� 25 ��Ԫ��
block_size = 25;%vsense1 16 vsense2 10 MPCCD 5
num_blocks = 16;%vsense1 2 vsense2 4 MPCCD 8


block_averages = zeros(num_blocks, 1);


for i = 1:num_blocks %B�������еĿ�
    block_start = (i - 1) * block_size + 1;%��һ��������
    block_end = i * block_size;%��β����

    [plcc, mae, rms, srcc, krcc] = IQA_eval(MOS(block_start:block_end), EABL(block_start:block_end), 'regress_logistic');
    PLCC(i) = plcc;
    SRCC(i) = srcc;
    MAE(i) = mae;
    RMSE(i) = rms;
    KRCC(i) = krcc;
end
[plccall, maeall, rmsall, srccall, krccall] = IQA_eval(MOS, EABL, 'regress_logistic');
