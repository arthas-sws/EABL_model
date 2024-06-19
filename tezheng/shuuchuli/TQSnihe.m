%ֱ�����
clc; clear all;
DATA =[87.89236849	81.64304521	75.31547102	70.69705053	47.48677611
86.93593505	85.47863801	81.6946559	69.56396174	54.74608311
92.98218434	93.13624992	89.17895455	56.66911015	30.54403914
82.99466308	78.35711357	74.37854846	45.34390773	29.57189469
88.12212138	82.48026711	71.1177798	45.85674466	28.66016866
86.67226612	81.66017563	75.12271305	55.39273891	18.1811391
81.03591012	82.46863047	80.05489138	79.04653867	63.23731563
87.39980101	90.76325098	85.36191982	59.33388396	26.06648207];
TQS = [12.69920842	25.39841683	50.79683366	101.5936673	203.1873347];

% ����ʾ�����ݣ������ж������ߣ�ÿ�����߶���һЩ���ݵ㣩
xData = TQS;
yData1 = DATA(1,:);
yData2 = DATA(2,:);
yData3 = DATA(3,:);
yData4 = DATA(4,:);
yData5 = DATA(5,:);
yData6 = DATA(6,:);
yData7 = DATA(7,:);
yData8 = DATA(8,:);
% �ϲ����ݵ�
x = [xData, xData,xData, xData,xData, xData,xData, xData];
y = [yData1, yData2,yData3, yData4,yData5, yData6,yData7, yData8];

% ִ���������
coefficients = polyfit(x, y, 1); % 1 ��ʾ�������

% ��ȡ��ϲ���
slope = coefficients(1); % б��
intercept = coefficients(2); % �ؾ�

% ����ԭʼ���ݺ����ֱ��
figure;
for i = 1:8
    plot(xData, DATA(i, :), 'o'); % ���Ƶ� i �����ߵ�ԭʼ���ݵ�
    hold on;
end
xlabel('X������');
ylabel('Y������');
title('�������ߵ�ԭʼ���ݵ�');
legend('����1', '����2', '����3', '����4', '����5', '����6', '����7', '����8');
x_fit = min(x):0.1:max(x); % �������ڻ�������ߵ� x ֵ
y_fit = slope * x_fit + intercept; % ����������ϵ� y ֵ
plot(x_fit, y_fit, 'k-', 'LineWidth', 2); % �������ֱ��
hold off;

MOS_fit = slope * TQS + intercept;
for i = 1:8
    plcc = corrcoef(MOS_fit, DATA(i, :)); 
   PLCC(i) = plcc(1,2);
end
mean(PLCC)
TQSDATA = [12.69920842
12.69920842
12.69920842
12.69920842
12.69920842
12.69920842
12.69920842
12.69920842
25.39841683
25.39841683
25.39841683
25.39841683
25.39841683
25.39841683
25.39841683
25.39841683
50.79683366
50.79683366
50.79683366
50.79683366
50.79683366
50.79683366
50.79683366
50.79683366
101.5936673
101.5936673
101.5936673
101.5936673
101.5936673
101.5936673
101.5936673
101.5936673
203.1873347
203.1873347
203.1873347
203.1873347
203.1873347
203.1873347
203.1873347
203.1873347
];
MOSDATA = [87.89236849
86.93593505
92.98218434
82.99466308
88.12212138
86.67226612
81.03591012
87.39980101
81.64304521
85.47863801
93.13624992
78.35711357
82.48026711
81.66017563
82.46863047
90.76325098
75.31547102
81.6946559
89.17895455
74.37854846
71.1177798
75.12271305
80.05489138
85.36191982
70.69705053
69.56396174
56.66911015
45.34390773
45.85674466
55.39273891
79.04653867
59.33388396
47.48677611
54.74608311
30.54403914
29.57189469
28.66016866
18.1811391
63.23731563
26.06648207
];
yucezhi = [87.24042191
87.24042191
87.24042191
87.24042191
87.24042191
87.24042191
87.24042191
87.24042191
83.84084381
83.84084381
83.84084381
83.84084381
83.84084381
83.84084381
83.84084381
83.84084381
77.04168763
77.04168763
77.04168763
77.04168763
77.04168763
77.04168763
77.04168763
77.04168763
63.44337526
63.44337526
63.44337526
63.44337526
63.44337526
63.44337526
63.44337526
63.44337526
36.2467505
36.2467505
36.2467505
36.2467505
36.2467505
36.2467505
36.2467505
36.2467505
];

a=corrcoef(yucezhi, MOSDATA); 
nihezhi = -0.2677*TQS+ 90.64 ;
b = a(1,2);
% c = corrcoef()