%%ForALL%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;
clear all;

%%
%  data = xlsread('�ĸ���������.xlsx','vsense��ѡ');
% data = xlsread('�ĸ���������.xlsx','vsense2��ѡ');
% data = xlsread('�ĸ���������.xlsx','MPCCD��ѡ');
 data = xlsread('�ĸ���������.xlsx','WPC2��ѡ');
MOS= data(:,1);
block_size  = 25;
fenkuai = 16;
NOsuanfa = size(data,2)-1;
algorithmNames = {'EABL','GraphSIM','PCM_{RR}','PointSSIM_{\sigma^2}','PointSSIM_{��AD}','MSE_{p2po}','PSNR_{p2po,M}','D_{p2pl,H}','PSNR_{p2pl,H}','PSNR_{Y}'};
for i=1:NOsuanfa %1:NOsuanfa
    figure;
    Projected = data(:,i+1);
    [PLCCALL(i), MAEALL(i), RMSEALL(i), SRCCALL(i), KRCCALL(i)] = IQA_eval(MOS, Projected, 'regress_logistic');
    
%     for j = 1:fenkuai
%     block_start = (j - 1) * block_size + 1;%��һ��������
%     block_end = j * block_size;%��β����  
%     
%     MOSfenlei = MOS(block_start:block_end,1);
%     Projectedfenlei = Projected(block_start:block_end,1);
%     
%     [plcc(j,i), mae(j,i), rms(j,i), srcc(j,i), krcc(j,i)] = IQA_eval(MOSfenlei, Projectedfenlei, 'regress_logistic');
% 
%     end
%%
%��ʦ����Ϻ���

hold on;
[~, beta, ~, ~] = regress_logistic(Projected, MOS);
tic
xhat = min(Projected):0.0001:max(Projected);%x�᷶Χ
yhat = logistic5(beta,xhat);%y�������
scatter(Projected,MOS,15,'fill','b','DisplayName','V-PCC');
plot(xhat,yhat,'k--','LineWidth',1);
toc
xlabel(algorithmNames{i},'FontSize', 20);
ylabel('MOS','FontSize', 20);

grid on;
grid minor;
set(gca,'FontSize',20,'FontName','times new roman');
hold off;

saveas(gcf, ['Algorithm_', num2str(i), '.png']);
end
% xlswrite('�ĸ���������.xlsx', PLCCALL, '��ʱ', 'A4');
% xlswrite('�ĸ���������.xlsx', MAEALL , '��ʱ', 'A5');
% xlswrite('�ĸ���������.xlsx', RMSEALL, '��ʱ', 'A6');
% xlswrite('�ĸ���������.xlsx', SRCCALL, '��ʱ', 'A7');
% xlswrite('�ĸ���������.xlsx', KRCCALL, '��ʱ', 'A8');
% xlswrite('�ĸ���������.xlsx', KRCCALL, '��ʱ', 'A9');
result = vertcat(PLCCALL, MAEALL, RMSEALL,SRCCALL,KRCCALL);
% result2=vertcat(plcc,mae,rms,srcc,krcc); 




