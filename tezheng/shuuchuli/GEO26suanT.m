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
        % ��ȡmatrix1��matrix2��ÿһ������
        row1 = DCT(i, :);
        row2 = YUV(j, :);
        
        % ����PLCC
        plcc = corrcoef(row1, row2);
        
        % �洢PLCC��ֵ
        plcc_matrix{a,b}(i, j) = plcc(1, 2);
    end
end


 end
end


% ��ʼ��һ��8x5�ľ������洢��ֵ���
mean_matrix = cell(8, 5);

% ����ÿ��cellԪ���о���ľ�ֵ
for i = 1:8
    for j = 1:5
        % ��ȡ��ǰcellԪ���еľ���
        current_matrix = plcc_matrix{i, j};
        
        % �������ľ�ֵ
        mean_value = mean(current_matrix, 'all');
        
        % ����ֵ�洢���µľ�����
        mean_matrix{i, j} = mean_value;
    end
end

%�������Լ���
n = 16; % ����
k = 8;  % Ҫѡ�������

% �������п��ܵ����
combinations = nchoosek(1:n, k);