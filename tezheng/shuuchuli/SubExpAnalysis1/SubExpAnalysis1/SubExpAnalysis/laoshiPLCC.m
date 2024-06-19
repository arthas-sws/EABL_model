%%ForALL%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear; clc;close all

%%
% data = xlsread('四个库总数据.xlsx','vsense1');
% data = xlsread('四个库总数据.xlsx','vsense2');
data = xlsread('四个库总数据.xlsx','MPCCD');
% data = xlsread('四个库总数据.xlsx','vsense1')
% MOS= xlsread('第一次DCT预测.xlsx');
 MOS= data(:,1);
% Projected= xlsread('第一次DCT预测.xlsx');
 Projected= data(:,2);
%%
[plcc, mae, rms, srcc, krcc] = IQA_eval(MOS, Projected, 'regress_logistic'); %TIP文章中的All数据
% 
% b=[plcc',mae',rms',srcc',krcc'];









