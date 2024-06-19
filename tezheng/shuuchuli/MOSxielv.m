clc;clear all;
data= xlsread('DCT.xlsx','9.13最优数据');
data1 = data(2:end,15);
data2 = data(2:end,14);
%缩放到同一尺度

% 步骤1：计算最小值和最大值
min1 = min(data1);
max1 = max(data1);
min2 = min(data2);
max2 = max(data2);

% 步骤2和步骤3：线性变换映射到0到1的范围
scaled_data1 = (data1 - min1) / (max1 - min1);
scaled_data2 = (data2 - min2) / (max2 - min2);

         coefficients = polyfit(scaled_data2, scaled_data1, 1);
         
         plcc1 = corrcoef(data2, data1);
         plcc2 = corrcoef(scaled_data2, scaled_data1);