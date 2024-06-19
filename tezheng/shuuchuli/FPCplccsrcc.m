clc;clear all;
WPC2data = xlsread('�ĸ���������.xlsx','MPCCD');
data = WPC2data(:,1);
MOS =  WPC2data(:,1);
% �����ݷ�Ϊ����飬ÿ������� 25 ��Ԫ��
block_size = 5;
num_blocks = 8;


block_averages = zeros(num_blocks, 1);
for j=1:55
    jisuan = WPC2data(:,j+1);

for i = 1:num_blocks %B�������еĿ�
    jisuan = (jisuan - min(jisuan)) / (max(jisuan) - min(jisuan));
    jisuan = jisuan*5;
    MOS = (MOS - min(MOS)) / (max(MOS) - min(MOS));
    MOS = MOS*5;
    block_start = (i - 1) * block_size + 1;%��һ��������
    block_end = i * block_size;%��β����
    plcc = corrcoef(jisuan(block_start:block_end),MOS(block_start:block_end));
    PLCC(i,j) = plcc(1,2);
    SRCC(i,j) = corr(jisuan(block_start:block_end),MOS(block_start:block_end), 'Type', 'Spearman');
    MSE(i,j) = sqrt(mean((jisuan(block_start:block_end) - MOS(block_start:block_end)).^2));
    
end

end