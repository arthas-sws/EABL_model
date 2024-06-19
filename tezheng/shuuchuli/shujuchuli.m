clc;clear all;
%ʹ�� DCTϵ�����Էֱ���ϵ����������DCT-YUV
%%
%����׼��
MOSXL =[-0.174148649
-0.258818651
-0.292585192
-0.316302987
-0.360415529
-0.26204704
-0.224150776
-0.333786789
];
yuvfenkuai8 = [7.719337488
4.590260243
5.997447563
3.078813486
2.726447455
6.772315885
6.231463403
2.8197538
];
attributemean = [152.743691
50.91097327
79.68148384
27.20619386
37.2980906
85.47183004
131.7788276
30.33434978
];
%%
%ʹ�� polyfit ��������������ϣ�һ����ϣ�
coefficients = polyfit(yuvfenkuai8, MOSXL, 1);

%��ȡ��ϵ�б�ʺͽؾ�
slope = coefficients(1); % б��
intercept = coefficients(2); % �ؾ�

%ʹ�� polyval �������������
fit_line = polyval(coefficients, yuvfenkuai8);
figure(1);
% ����ԭʼ���ݺ������
plot(yuvfenkuai8, MOSXL, 'o', yuvfenkuai8, fit_line, '-');
legend('ԭʼ����', '�����');
xlabel('A');
ylabel('B');
%%
% ʹ�� polyfit ��������������ϣ�һ����ϣ�
coefficients2 = polyfit(attributemean, yuvfenkuai8, 1);

% ��ȡ��ϵ�б�ʺͽؾ�
slope2 = coefficients2(1); % б��
intercept2 = coefficients2(2); % �ؾ�

% ʹ�� polyval �������������
fit_line2 = polyval(coefficients2, attributemean);
figure(2);
% ����ԭʼ���ݺ������
plot(attributemean, yuvfenkuai8, 'o', attributemean, fit_line2, '-');
legend('ԭʼ����', '�����');
xlabel('A');
ylabel('B');
%%
% % ʹ�� polyfit �������ж��׶���ʽ���
% coefficients3 = polyfit(yuvfenkuai8, MOSXL, 2);
% 
% % ʹ�� polyval �������ɶ��׶���ʽ�������
% fit_curve = polyval(coefficients3, yuvfenkuai8);
% figure(3);
% % ����ԭʼ���ݺ��������
% plot(yuvfenkuai8, MOSXL, 'o', yuvfenkuai8, fit_curve, '-');
% legend('ԭʼ����', '�������');
%%
% % ����ָ��ģ�ͺ���
% exp_model = fittype('a * exp(b * x)', 'coefficients', {'a', 'b'});
% 
% % ����һ�����ѡ�����
% options = fitoptions('Method', 'NonlinearLeastSquares', 'StartPoint', [1, 1]);
% % ���� yuvfenkuai8 �� MOSXL ��������
% yuvfenkuai8 = yuvfenkuai8(:);  % �� yuvfenkuai8 ת��Ϊ������
% MOSXL = MOSXL(:);            % �� MOSXL ת��Ϊ������
% 
% % ���� yuvfenkuai8 �� MOSXL ����������������Խ������
% fit_result = fit(yuvfenkuai8, MOSXL, exp_model, options);
% 
% 
% % ��ȡ��ϲ���
% a = fit_result.a;
% b = fit_result.b;
% figure(4);
% % ����ԭʼ���ݺ��������
% plot(yuvfenkuai8, MOSXL, 'o', yuvfenkuai8, a * exp(b * yuvfenkuai8), '-');
% legend('ԭʼ����', 'ָ���������');