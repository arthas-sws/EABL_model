%%ForALL%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;
clear all;

%%
%  data = xlsread('四个库总数据.xlsx','vsense挑选');
% data = xlsread('四个库总数据.xlsx','vsense2挑选');
% data = xlsread('四个库总数据.xlsx','MPCCD挑选');
 data = xlsread('四个库总数据.xlsx','WPC2挑选');
MOS= data(:,1);
block_size  = 25;
fenkuai = 16;
NOsuanfa = size(data,2)-1;
algorithmNames = {'EABL','GraphSIM','PCM_{RR}','PointSSIM_{\sigma^2}','PointSSIM_{μAD}','MSE_{p2po}','PSNR_{p2po,M}','D_{p2pl,H}','PSNR_{p2pl,H}','PSNR_{Y}'};
for i=1:NOsuanfa %1:NOsuanfa
    figure;
    Projected = data(:,i+1);
    [PLCCALL(i), MAEALL(i), RMSEALL(i), SRCCALL(i), KRCCALL(i)] = IQA_eval(MOS, Projected, 'regress_logistic');
    
%     for j = 1:fenkuai
%     block_start = (j - 1) * block_size + 1;%第一个块索引
%     block_end = j * block_size;%结尾索引  
%     
%     MOSfenlei = MOS(block_start:block_end,1);
%     Projectedfenlei = Projected(block_start:block_end,1);
%     
%     [plcc(j,i), mae(j,i), rms(j,i), srcc(j,i), krcc(j,i)] = IQA_eval(MOSfenlei, Projectedfenlei, 'regress_logistic');
% 
%     end
%%
%老师的拟合函数

hold on;
[~, beta, ~, ~] = regress_logistic(Projected, MOS);
tic
xhat = min(Projected):0.0001:max(Projected);%x轴范围
yhat = logistic5(beta,xhat);%y拟合曲线
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
% xlswrite('四个库总数据.xlsx', PLCCALL, '暂时', 'A4');
% xlswrite('四个库总数据.xlsx', MAEALL , '暂时', 'A5');
% xlswrite('四个库总数据.xlsx', RMSEALL, '暂时', 'A6');
% xlswrite('四个库总数据.xlsx', SRCCALL, '暂时', 'A7');
% xlswrite('四个库总数据.xlsx', KRCCALL, '暂时', 'A8');
% xlswrite('四个库总数据.xlsx', KRCCALL, '暂时', 'A9');
result = vertcat(PLCCALL, MAEALL, RMSEALL,SRCCALL,KRCCALL);
% result2=vertcat(plcc,mae,rms,srcc,krcc); 




