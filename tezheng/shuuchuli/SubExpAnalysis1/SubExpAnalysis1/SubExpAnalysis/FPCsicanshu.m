clc;clear all;
% data = xlsread('四个库总数据.xlsx','vsense1');
% data = xlsread('四个库总数据.xlsx','vsense2');
% data = xlsread('四个库总数据.xlsx','MPCCD');
data = xlsread('四个库总数据.xlsx','WPC2');
MOS =  data(:,1);
EABL = data(:,2);
% 将数据分为多个块，每个块包含 25 个元素
block_size = 25;%vsense1 16 vsense2 10 MPCCD 5
num_blocks = 16;%vsense1 2 vsense2 4 MPCCD 8


block_averages = zeros(num_blocks, 1);


for i = 1:num_blocks %B遍历所有的块
    block_start = (i - 1) * block_size + 1;%第一个块索引
    block_end = i * block_size;%结尾索引

    [plcc, mae, rms, srcc, krcc] = IQA_eval(MOS(block_start:block_end), EABL(block_start:block_end), 'regress_logistic');
    PLCC(i) = plcc;
    SRCC(i) = srcc;
    MAE(i) = mae;
    RMSE(i) = rms;
    KRCC(i) = krcc;
end
[plccall, maeall, rmsall, srccall, krccall] = IQA_eval(MOS, EABL, 'regress_logistic');
