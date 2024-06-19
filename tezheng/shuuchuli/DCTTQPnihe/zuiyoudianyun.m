clc;clear all;
%最优线性计算
n = 16; % 总数
k = 8;  % 要选择的数量
% 生成所有可能的组合
combinations = nchoosek(1:n, k);
%%
%首先找出原始YUV 和MOS斜率 相关性较高的
%找出原始YUV数据和失真DCT数据较好的（只计算DCT26
data= xlsread('DCT.xlsx','Sheet2');
DCT = data(2:end,9);%读取DCT系数
DCT=reshape(DCT,5,16);%DCT矩阵变换成5*16 这样第一行就是T26


data2= xlsread('DCT.xlsx','原始点云YUV数据');
YUV = data2(2:17,1:5)';%所有原始YUV数据

MOSxielv = data2(2:17,6)';%原始点云MOS斜率
%%
for zuhe = 1:12870   
DCTchoose = DCT(:,combinations(zuhe,:));%提取出不同点云组合的DCT系数矩阵
YUVchoose = YUV(:,combinations(zuhe,:));%提取出不同点云组合的YUV系数矩阵
MOSchoose = MOSxielv(:,combinations(zuhe,:));
    for i = 1:5
         
        % 提取matrix1和matrix2的每一行数据
        row1 = DCTchoose(1, :);%这里的i 表示不同的QP 这里直接选择26先查看情况
        row2 = YUVchoose(i, :);%这里的i 表示不同的步长YUV 
        row3 = MOSchoose(1, :);%唯一的MOS斜率
        % 计算PLCC
        plcc1 = corrcoef(row1, row2);
        plcc2 = corrcoef(row2, row3);
        % 存储PLCC的值
        plcc_matrix1(i) = plcc1(1, 2);%不同步长平均
        plcc_matrix2(i) = plcc2(1, 2);%不同步长平均
    end
        mean_value1(zuhe) = mean(plcc_matrix1, 'all');
        mean_value2(zuhe) = mean(plcc_matrix2, 'all');
        absmean_value1(zuhe) = abs(mean_value1(zuhe));
        absmean_value2(zuhe) = abs(mean_value2(zuhe));
end
xiangcheng = absmean_value2.*absmean_value1;
xiangjia   = absmean_value2+absmean_value1;
%  max(absmean_value);
 
% 获取矩阵中最大的前十个值及其索引
num_values = 10; % 要获取的最大值的数量
[sorted_values, sorted_indices] = sort(xiangcheng, 'descend'); % 降序排序

% 获取最大的前十个值和对应的索引
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

% 打印最大的前十个值及其对应的索引
for i = 1:num_values
    fprintf('相乘最大值 %d: 值 = %f, 索引 = %d\n', i, top_values(i), top_indices(i));
end
%%
% 获取矩阵中最大的前十个值及其索引
num_values = 10; % 要获取的最大值的数量
[sorted_values, sorted_indices] = sort(xiangjia, 'descend'); % 降序排序

% 获取最大的前十个值和对应的索引
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

% 打印最大的前十个值及其对应的索引
for i = 1:num_values
    fprintf('相加最大值 %d: 值 = %f, 索引 = %d\n', i, top_values(i), top_indices(i));
end
%%
% absmean_value2(12304);
% absmean_value1(12304);
DCTc = DCT(:,combinations(12022,:));
YUVc = YUV(:,combinations(12022,:));
MOSc = MOSxielv(:,combinations(12022,:));

for i = 1:5
    
    for k = 1:5
        plccsuoyou = corrcoef(DCTc(i,:), YUVc(k,:));
        PLCCjuzhen(i,k) = plccsuoyou(1,2); %还是第三列最好
    end
end



