%%ForALL%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear; clc;close all

%%
%data=xlsread('D:\work\dachuang\2021_bishe\data\MPCCD_Database.xlsx');
%主观分
%MOS=data(:,1);
%客观分
%Projected = data(:,31);
MOS= xlsread('第一次DCT预测.xlsx');
MOS= MOS(:,1);
Projected= xlsread('第一次DCT预测.xlsx');
Projected= Projected(:,6);
%%
[plcc, mae, rms, srcc, krcc] = IQA_eval(MOS, Projected, 'regress_logistic'); %TIP文章中的All数据
% for i = 1:8 % TIP文章中分content的数据
%     [plcc(i), mae(i), rms(i), srcc(i), krcc(i)] = IQA_eval(MOS(1+29*(i-1):29*i), Projected(1+29*(i-1):29*i), 'regress_logistic');
% end
b=[plcc',mae',rms',srcc',krcc'];

%{
Projected_octreelifting = [Projected(1:6,1);Projected(29+1:29+6,1);Projected(29*2+1:29*2+6,1);Projected(29*3+1:29*3+6,1);Projected(29*4+1:29*4+6,1);Projected(29*5+1:29*5+6,1);Projected(29*6+1:29*6+6,1);Projected(26*7+1:29*7+6,1)];
MOS_octreelifting = [MOS(1:6,1);MOS(29+1:29+6,1);MOS(29*2+1:29*2+6,1);MOS(29*3+1:29*3+6,1);MOS(29*4+1:29*4+6,1);MOS(29*5+1:29*5+6,1);MOS(29*6+1:29*6+6,1);MOS(26*7+1:29*7+6,1);];
plot(Projected_octreelifting,MOS_octreelifting,'r^','DisplayName','Octree-lifting');
hold on;

Projected_OctreeRAHT = [Projected(7:12,1);Projected(29+7:29+12,1);Projected(29*2+7:29*2+12,1);Projected(29*3+7:29*3+12,1);Projected(29*4+7:29*4+12,1);Projected(29*5+7:29*5+12,1);Projected(29*6+7:29*6+12,1);Projected(29*7+7:29*7+12,1)];
MOS_OctreeRAHT = [MOS(7:12,1);MOS(29+7:29+12,1);MOS(29*2+7:29*2+12,1);MOS(29*3+7:29*3+12,1);MOS(29*4+7:29*4+12,1);MOS(29*5+7:29*5+12,1);MOS(29*6+7:29*6+12,1);MOS(29*7+7:29*7+12,1)];
plot(Projected_OctreeRAHT,MOS_OctreeRAHT,'g*','DisplayName','Octree-RAHT');
%plot(Projected_noised,MOS_noised,'Marker','*','MarkerEdgeColor',[0,1,0],'MarkerFaceColor',[0,1,0],'DisplayName','Noised');
hold on;
   
Projected_TriSoupLifting = [Projected(13:18,1);Projected(29+13:29+18,1);Projected(29*2+13:29*2+18,1);Projected(29*3+13:29*3+18,1);Projected(29*4+13:29*4+18,1);Projected(29*5+13:29*5+18,1);Projected(29*6+13:29*6+18,1);Projected(29*7+13:29*7+18,1)];
MOS_TriSoupLifting = [MOS(13:18,1);MOS(29+13:29+18,1);MOS(29*2+13:29*2+18,1);MOS(29*3+13:29*3+18,1);MOS(29*4+13:29*4+18,1);MOS(29*5+13:29*5+18,1);MOS(29*6+13:29*6+18,1);MOS(29*7+13:29*7+18,1)];
plot(Projected_TriSoupLifting,MOS_TriSoupLifting,'b+','DisplayName','TriSoup-Lifting');
hold on;

Projected_TriSoupRAHT = [Projected(19:24,1);Projected(29+19:29+24,1);Projected(29*2+19:29*2+24,1);Projected(29*3+19:29*3+24,1);Projected(29*4+19:29*4+24,1);Projected(29*5+19:29*5+24,1);Projected(29*6+19:29*6+24,1);Projected(29*7+19:29*7+24,1)];
MOS_TriSoupRAHT = [MOS(19:24,1);MOS(29+19:29+24,1);MOS(29*2+19:29*2+24,1);MOS(29*3+19:29*3+24,1);MOS(29*4+19:29*4+24,1);MOS(29*5+19:29*5+24,1);MOS(29*6+19:29*6+24,1);MOS(29*7+19:29*7+24,1)];
plot(Projected_TriSoupRAHT,MOS_TriSoupRAHT,'ms','DisplayName','TriSoup-RAHT');
hold on;

Projected_VPCC = [Projected(25:29,1);Projected(29+25:29+29,1);Projected(29*2+25:29*2+29,1);Projected(29*3+25:29*3+29,1);Projected(29*4+25:29*4+29,1);Projected(29*5+25:29*5+29,1);Projected(29*6+25:29*6+29,1);Projected(29*7+25:29*7+29,1)];
MOS_VPCC = [MOS(25:29,1);MOS(29+25:29+29,1);MOS(29*2+25:29*2+29,1);MOS(29*3+25:29*3+29,1);MOS(29*4+25:29*4+29,1);MOS(29*5+25:29*5+29,1);MOS(29*6+25:29*6+29,1);MOS(29*7+25:29*7+29,1)];
plot(Projected_VPCC,MOS_VPCC,'ko','DisplayName','V-PCC');
hold on;


[plcc_d, mae_d, rms_d, srcc_d, krcc_d] = IQA_eval(MOS_octreelifting, Projected_octreelifting, 'regress_logistic');
[plcc_n, mae_n, rms_n, srcc_n, krcc_n] = IQA_eval(MOS_OctreeRAHT, Projected_OctreeRAHT, 'regress_logistic');
[plcc_1, mae_1, rms_1, srcc_1, krcc_1] = IQA_eval(MOS_TriSoupLifting, Projected_TriSoupLifting, 'regress_logistic');
[plcc_2, mae_2, rms_2, srcc_2, krcc_2] = IQA_eval(MOS_TriSoupRAHT, Projected_TriSoupRAHT, 'regress_logistic');
[plcc_3, mae_3, rms_3, srcc_3, krcc_3] = IQA_eval(MOS_VPCC, Projected_VPCC, 'regress_logistic');
a=[plcc_d, mae_d, rms_d, srcc_d, krcc_d;
    plcc_n, mae_n, rms_n, srcc_n, krcc_n;
    plcc_1, mae_1, rms_1, srcc_1, krcc_1;
    plcc_2, mae_2, rms_2, srcc_2, krcc_2;
    plcc_3, mae_3, rms_3, srcc_3, krcc_3;]
%}

%% 
%老师的拟合函数

hold on;
[~, beta, ~, ~] = regress_logistic(Projected, MOS);
xhat = 0:0.1:100;
% yhat = 0:0.1:100;
yhat = logistic5(beta,xhat);
scatter(Projected,MOS,15,'fill','b','DisplayName','V-PCC');
plot(xhat,yhat,'k--','LineWidth',5);


%自己的拟合函数
% f=fittype('a*cos(k*t)*exp(w*t)','independent','t','coefficients',{'a','k','w'});
% f2=fit(Projected,MOS,f);
% xi=0:0.001:0.1;
% yi=f2(xi);
% axis([0 1,0 1])
% plot(xi,yi,'k--','LineWidth',2);

%l = legend({'Octree-lifting','Octree-RAHT','TriSoup-Lifting','TriSoup-RAHT','V-PCC'},'Location','northwest');
%set(l,'Fontsize',15);
xlabel('Objective score');
ylabel('MOS');
grid on;
grid minor;
set(gca,'FontSize',18,'FontName','times new roman');


