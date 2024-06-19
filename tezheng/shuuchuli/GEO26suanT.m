clc;clear all;
data= xlsread('DCT.xlsx','GEO26');
data = data(2:end,:);
DCTshuju = data(:,2:9);
YUVshuju = data(:,10:14);
for a =1:8
   for b = 1:5
       
DCT=reshape(DCTshuju(:,a),5,16);
YUV = reshape(YUVshuju(:,b),5,16);
    for i = 1:5
    for j = 1:5
        % 提取matrix1和matrix2的每一行数据
        row1 = DCT(i, :);
        row2 = YUV(j, :);
        
        % 计算PLCC
        plcc = corrcoef(row1, row2);
        
        % 存储PLCC的值
        plcc_matrix{a,b}(i, j) = plcc(1, 2);
    end
end


 end
end


% 初始化一个8x5的矩阵来存储均值结果
mean_matrix = cell(8, 5);

% 计算每个cell元素中矩阵的均值
for i = 1:8
    for j = 1:5
        % 获取当前cell元素中的矩阵
        current_matrix = plcc_matrix{i, j};
        
        % 计算矩阵的均值
        mean_value = mean(current_matrix, 'all');
        
        % 将均值存储到新的矩阵中
        mean_matrix{i, j} = mean_value;
    end
end

%最优线性计算
n = 16; % 总数
k = 8;  % 要选择的数量

% 生成所有可能的组合
combinations = nchoosek(1:n, k);