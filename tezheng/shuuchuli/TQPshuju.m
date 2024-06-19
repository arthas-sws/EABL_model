clc;clear all;
%使用TQP拟合DCT斜率等
YUVZ= xlsread('DCT.xlsx','9.16最优数据');
YUV = YUVZ(8,2:9);
data= xlsread('DCT.xlsx','9.16最优数据');
DCT = data(1:5,2:9);
% DCT = reshape(DCT,5,8);
MOSxielv = YUVZ(11,2:9);
for i = 1:5
    coefficients = polyfit(DCT(i,:), YUV, 1);
    xielv(i) = coefficients(1);
    jieju(i) = coefficients(2);
end
%拟合出斜率和截距关于TQP的关系式
TQP = [26 32 38 44 50];

 cftool;
 
     coeffxielv = polyfit(TQP, xielv, 2);
    xielva = coeffxielv(1);
    xielvb = coeffxielv(2);
    xielvc = coeffxielv(3);
    coeffjieju = polyfit(TQP, jieju, 2);
    jiejua = coeffxielv(1);
    jiejub = coeffxielv(2);
    jiejuc = coeffxielv(3);
    
% coexielv = polyfit(TQP,xielv,1);
% coejieju = polyfit(TQP,jieju,1);
% coemosyuv = polyfit(TQP,jieju,1);
%求所有GQP步长的PLCC矩阵
% for i = 2:9
%     for k = 10:14
%         for j = 1:5
%             X = DCTs{j}(:, i);
%             Y = DCTs{j}(:, k);
%             coefficients = polyfit(X, Y, 1);
%             plcc = corrcoef(X, Y);
%             plcc_values(i-1, k-9, j) = plcc(1, 2);
%             xielv_values(i-1, k-9, j) = coefficients(1);
%             jieju_values(i-1, k-9, j) = coefficients(2);
%         end
%     end
% end
% %%求平均PLCC 使用步长为8
% for k = 1:5
% plcche(k)=mean(plcc_values(8,k,:));
% end
% %%
% %由此可得 YUV使用步长为8 DCT系数使用4*4归一化 即（3,5，j）的所有斜率和截距
% %
% xielvjuzhen=xielv_values(8,5,:);
% jiejujuzhen=jieju_values(8,5,:);
% xielvjuzhen = squeeze(xielvjuzhen)';
% jiejujuzhen = squeeze(jiejujuzhen)';
% 
% TQP = [26 32 38 44 50];
% TQPxielvnihe = polyfit(TQP, xielvjuzhen, 1);
% TQPjiejunihe = polyfit(TQP, jiejujuzhen, 1);
% %%
% % 获取拟合的多项式系数
% a = TQPxielvnihe(1);
% b = TQPxielvnihe(2);
% 
% % 构建多项式的字符串表达式
% fit_equation = sprintf('斜率 = %.6fx + %.4f', a, b);
% 
% % 显示拟合公式
% disp(fit_equation);
% %%
% % 获取拟合的多项式系数
% a = TQPjiejunihe(1);
% b = TQPjiejunihe(2);
% 
% % 构建多项式的字符串表达式
% fit_equation = sprintf('截距 = %.6fx + %.4f', a, b);
% 
% % 显示拟合公式
% disp(fit_equation);

