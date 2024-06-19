clc;clear all;
data= xlsread('94���.xlsx','Sheet4');
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
% scatter(newyuv,newmos, 'filled'); % ʹ��scatter����ɢ��ͼ
% xlabel('AD'); % ���x���ǩ
% ylabel('MOSXL'); % ���y���ǩ

% % �����������
% shuffled_indices= randperm(length(newyuv));
% shuffled_array = sorted_array(shuffled_indices);


PLCC = corrcoef(mos,YUV(:,1));
%%
% figure()
% scatter(YUV(:,1),mos, 'filled'); % ʹ��scatter����ɢ��ͼ
% xlabel('AD'); % ���x���ǩ
% ylabel('MOSXL'); % ���y���ǩ

%%
%������������ֵ mosxielvΪ-0.12 YUVΪ9.77 mosб��Ϊ0.31 YUVΪ31.26 ��������YUV����Ч����0.36-0.74
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
scatter(NYUV,NMOS, 'filled'); % ʹ��scatter����ɢ��ͼ
xlabel('CD','FontSize', 20); % ���x���ǩ
ylabel('��','FontSize', 20); % ���y���ǩ
set(gca, 'FontSize', 20);
print('SWSPLCCCD2', '-dpng');
