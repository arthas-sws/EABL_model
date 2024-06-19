clc;clear all;
%�������Լ���
n = 16; % ����
k = 8;  % Ҫѡ�������
% �������п��ܵ����
combinations = nchoosek(1:n, k);
%%
%�����ҳ�ԭʼYUV ��MOSб�� ����Խϸߵ�
%�ҳ�ԭʼYUV���ݺ�ʧ��DCT���ݽϺõģ�ֻ����DCT26
data= xlsread('DCT.xlsx','Sheet2');
DCT = data(2:end,9);%��ȡDCTϵ��
DCT=reshape(DCT,5,16);%DCT����任��5*16 ������һ�о���T26


data2= xlsread('DCT.xlsx','ԭʼ����YUV����');
YUV = data2(2:17,1:5)';%����ԭʼYUV����

MOSxielv = data2(2:17,6)';%ԭʼ����MOSб��
%%
for zuhe = 1:12870   
DCTchoose = DCT(:,combinations(zuhe,:));%��ȡ����ͬ������ϵ�DCTϵ������
YUVchoose = YUV(:,combinations(zuhe,:));%��ȡ����ͬ������ϵ�YUVϵ������
MOSchoose = MOSxielv(:,combinations(zuhe,:));
    for i = 1:5
         
        % ��ȡmatrix1��matrix2��ÿһ������
        row1 = DCTchoose(1, :);%�����i ��ʾ��ͬ��QP ����ֱ��ѡ��26�Ȳ鿴���
        row2 = YUVchoose(i, :);%�����i ��ʾ��ͬ�Ĳ���YUV 
        row3 = MOSchoose(1, :);%Ψһ��MOSб��
        % ����PLCC
        plcc1 = corrcoef(row1, row2);
        plcc2 = corrcoef(row2, row3);
        % �洢PLCC��ֵ
        plcc_matrix1(i) = plcc1(1, 2);%��ͬ����ƽ��
        plcc_matrix2(i) = plcc2(1, 2);%��ͬ����ƽ��
    end
        mean_value1(zuhe) = mean(plcc_matrix1, 'all');
        mean_value2(zuhe) = mean(plcc_matrix2, 'all');
        absmean_value1(zuhe) = abs(mean_value1(zuhe));
        absmean_value2(zuhe) = abs(mean_value2(zuhe));
end
xiangcheng = absmean_value2.*absmean_value1;
xiangjia   = absmean_value2+absmean_value1;
%  max(absmean_value);
 
% ��ȡ����������ǰʮ��ֵ��������
num_values = 10; % Ҫ��ȡ�����ֵ������
[sorted_values, sorted_indices] = sort(xiangcheng, 'descend'); % ��������

% ��ȡ����ǰʮ��ֵ�Ͷ�Ӧ������
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

% ��ӡ����ǰʮ��ֵ�����Ӧ������
for i = 1:num_values
    fprintf('������ֵ %d: ֵ = %f, ���� = %d\n', i, top_values(i), top_indices(i));
end
%%
% ��ȡ����������ǰʮ��ֵ��������
num_values = 10; % Ҫ��ȡ�����ֵ������
[sorted_values, sorted_indices] = sort(xiangjia, 'descend'); % ��������

% ��ȡ����ǰʮ��ֵ�Ͷ�Ӧ������
top_values = sorted_values(1:num_values);
top_indices = sorted_indices(1:num_values);

% ��ӡ����ǰʮ��ֵ�����Ӧ������
for i = 1:num_values
    fprintf('������ֵ %d: ֵ = %f, ���� = %d\n', i, top_values(i), top_indices(i));
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
        PLCCjuzhen(i,k) = plccsuoyou(1,2); %���ǵ��������
    end
end



