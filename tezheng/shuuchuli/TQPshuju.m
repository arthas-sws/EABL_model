clc;clear all;
%ʹ��TQP���DCTб�ʵ�
YUVZ= xlsread('DCT.xlsx','9.16��������');
YUV = YUVZ(8,2:9);
data= xlsread('DCT.xlsx','9.16��������');
DCT = data(1:5,2:9);
% DCT = reshape(DCT,5,8);
MOSxielv = YUVZ(11,2:9);
for i = 1:5
    coefficients = polyfit(DCT(i,:), YUV, 1);
    xielv(i) = coefficients(1);
    jieju(i) = coefficients(2);
end
%��ϳ�б�ʺͽؾ����TQP�Ĺ�ϵʽ
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
%������GQP������PLCC����
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
% %%��ƽ��PLCC ʹ�ò���Ϊ8
% for k = 1:5
% plcche(k)=mean(plcc_values(8,k,:));
% end
% %%
% %�ɴ˿ɵ� YUVʹ�ò���Ϊ8 DCTϵ��ʹ��4*4��һ�� ����3,5��j��������б�ʺͽؾ�
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
% % ��ȡ��ϵĶ���ʽϵ��
% a = TQPxielvnihe(1);
% b = TQPxielvnihe(2);
% 
% % ��������ʽ���ַ������ʽ
% fit_equation = sprintf('б�� = %.6fx + %.4f', a, b);
% 
% % ��ʾ��Ϲ�ʽ
% disp(fit_equation);
% %%
% % ��ȡ��ϵĶ���ʽϵ��
% a = TQPjiejunihe(1);
% b = TQPjiejunihe(2);
% 
% % ��������ʽ���ַ������ʽ
% fit_equation = sprintf('�ؾ� = %.6fx + %.4f', a, b);
% 
% % ��ʾ��Ϲ�ʽ
% disp(fit_equation);

