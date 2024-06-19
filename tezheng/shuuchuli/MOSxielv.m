clc;clear all;
data= xlsread('DCT.xlsx','9.13��������');
data1 = data(2:end,15);
data2 = data(2:end,14);
%���ŵ�ͬһ�߶�

% ����1��������Сֵ�����ֵ
min1 = min(data1);
max1 = max(data1);
min2 = min(data2);
max2 = max(data2);

% ����2�Ͳ���3�����Ա任ӳ�䵽0��1�ķ�Χ
scaled_data1 = (data1 - min1) / (max1 - min1);
scaled_data2 = (data2 - min2) / (max2 - min2);

         coefficients = polyfit(scaled_data2, scaled_data1, 1);
         
         plcc1 = corrcoef(data2, data1);
         plcc2 = corrcoef(scaled_data2, scaled_data1);