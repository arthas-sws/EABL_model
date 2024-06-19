clc;clear all;
%使用 DCT系数除以分辨率系数和来做的DCT-YUV
%%
%数据准备
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
%使用 polyfit 函数进行线性拟合（一阶拟合）
coefficients = polyfit(yuvfenkuai8, MOSXL, 1);

%提取拟合的斜率和截距
slope = coefficients(1); % 斜率
intercept = coefficients(2); % 截距

%使用 polyval 函数生成拟合线
fit_line = polyval(coefficients, yuvfenkuai8);
figure(1);
% 绘制原始数据和拟合线
plot(yuvfenkuai8, MOSXL, 'o', yuvfenkuai8, fit_line, '-');
legend('原始数据', '拟合线');
xlabel('A');
ylabel('B');
%%
% 使用 polyfit 函数进行线性拟合（一阶拟合）
coefficients2 = polyfit(attributemean, yuvfenkuai8, 1);

% 提取拟合的斜率和截距
slope2 = coefficients2(1); % 斜率
intercept2 = coefficients2(2); % 截距

% 使用 polyval 函数生成拟合线
fit_line2 = polyval(coefficients2, attributemean);
figure(2);
% 绘制原始数据和拟合线
plot(attributemean, yuvfenkuai8, 'o', attributemean, fit_line2, '-');
legend('原始数据', '拟合线');
xlabel('A');
ylabel('B');
%%
% % 使用 polyfit 函数进行二阶多项式拟合
% coefficients3 = polyfit(yuvfenkuai8, MOSXL, 2);
% 
% % 使用 polyval 函数生成二阶多项式拟合曲线
% fit_curve = polyval(coefficients3, yuvfenkuai8);
% figure(3);
% % 绘制原始数据和拟合曲线
% plot(yuvfenkuai8, MOSXL, 'o', yuvfenkuai8, fit_curve, '-');
% legend('原始数据', '拟合曲线');
%%
% % 定义指数模型函数
% exp_model = fittype('a * exp(b * x)', 'coefficients', {'a', 'b'});
% 
% % 创建一个拟合选项对象
% options = fitoptions('Method', 'NonlinearLeastSquares', 'StartPoint', [1, 1]);
% % 假设 yuvfenkuai8 和 MOSXL 是列向量
% yuvfenkuai8 = yuvfenkuai8(:);  % 将 yuvfenkuai8 转换为列向量
% MOSXL = MOSXL(:);            % 将 MOSXL 转换为列向量
% 
% % 现在 yuvfenkuai8 和 MOSXL 变成了列向量，可以进行拟合
% fit_result = fit(yuvfenkuai8, MOSXL, exp_model, options);
% 
% 
% % 提取拟合参数
% a = fit_result.a;
% b = fit_result.b;
% figure(4);
% % 绘制原始数据和拟合曲线
% plot(yuvfenkuai8, MOSXL, 'o', yuvfenkuai8, a * exp(b * yuvfenkuai8), '-');
% legend('原始数据', '指数拟合曲线');