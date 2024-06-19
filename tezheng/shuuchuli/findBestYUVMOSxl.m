clc;clear all;
data= xlsread('94组合.xlsx','Sheet4');
mos = data(:,1);
YUV =data(:,2:16);
for i =1:15
PLCC = corrcoef(YUV(:,i), mos);
plcc(i)=PLCC(1,2);
end


newmos = sort(mos, 'descend'); 
newyuv = sort(YUV(:,1), 'descend'); 
% PLCC = corrcoef(newmos,newyuv);
% figure()
% scatter(newyuv,newmos, 'filled'); % 使用scatter绘制散点图
% xlabel('AD'); % 添加x轴标签
% ylabel('MOSXL'); % 添加y轴标签

% % 随机打乱数组
% shuffled_indices= randperm(length(newyuv));
% shuffled_array = sorted_array(shuffled_indices);


PLCC = corrcoef(mos,YUV(:,1));
%%
% figure()
% scatter(YUV(:,1),mos, 'filled'); % 使用scatter绘制散点图
% xlabel('AD'); % 添加x轴标签
% ylabel('MOSXL'); % 添加y轴标签

%%
%调换了两个数值 mosxielv为-0.12 YUV为9.77 mos斜率为0.31 YUV为31.26 这两个的YUV互换效果从0.36-0.74
NMOS = [-0.096243764
-0.222543075
-0.315422015
-0.11992407
-0.292585192
-0.174148649
-0.197397181
-0.224150776
-0.3499225
-0.26204704
-0.360415529
-0.316302987
-0.354319628
-0.333786789
-0.258818651
-0.125093272
];
NYUV = [39.68100782
35.03270005
9.778871507
27.65029882
26.53657964
26.24191581
25.08239881
23.30661082
22.93777583
20.57041136
19.36802467
17.92393705
15.83804875
13.52264403
13.11453347
31.26808934
];
figure()
PLCC2 = corrcoef(NYUV,NMOS);
scatter(NYUV,NMOS, 'filled'); % 使用scatter绘制散点图
xlabel('CD','FontSize', 20); % 添加x轴标签
ylabel('α','FontSize', 20); % 添加y轴标签
set(gca, 'FontSize', 20);
print('SWSPLCCCD2', '-dpng');
