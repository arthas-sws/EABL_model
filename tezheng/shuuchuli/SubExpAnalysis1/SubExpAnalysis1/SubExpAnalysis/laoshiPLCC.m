%%ForALL%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear; clc;close all

%%
% data = xlsread('�ĸ���������.xlsx','vsense1');
% data = xlsread('�ĸ���������.xlsx','vsense2');
data = xlsread('�ĸ���������.xlsx','MPCCD');
% data = xlsread('�ĸ���������.xlsx','vsense1')
% MOS= xlsread('��һ��DCTԤ��.xlsx');
 MOS= data(:,1);
% Projected= xlsread('��һ��DCTԤ��.xlsx');
 Projected= data(:,2);
%%
[plcc, mae, rms, srcc, krcc] = IQA_eval(MOS, Projected, 'regress_logistic'); %TIP�����е�All����
% 
% b=[plcc',mae',rms',srcc',krcc'];









