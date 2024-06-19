clear all; clc;

addpath('./fwdcode/');

load('..\MOS.mat');
load('..\sourceVideo.mat');
load('..\..\VQAResult\ASQoE\averageBitrate\averageBitrate.mat');
sourceNames = sourceVideo.name;
scorePath = '..\..\VQAResult\ASQoE\';

QO_FTW = [];
QO_Mok = [];
QO_Liu12 = [];
QO_VsQM = [];
QO_Kim12 = [];
QO_Xue = [];
QO_Yin = [];
QO_Liu15 = [];
QO_Bentaleb16 = [];
QO_SQI_SSIMplus = [];
QO_VQM = [];
QO_SSIMplus = [];
QO_NARX = [];
QO_ATLAS = [];
QO_P1203 = [];

for iii = 1:length(sourceNames)
    load([scorePath 'FTW\' sourceNames{iii}]);
    load([scorePath 'Mok2011\' sourceNames{iii}]);
    load([scorePath 'Liu2012\' sourceNames{iii}]);
    load([scorePath 'VsQM\' sourceNames{iii}]);
    load([scorePath 'Kim2012\' sourceNames{iii}]);
    load([scorePath 'Xue2014\' sourceNames{iii}]);
    load([scorePath 'Yin2015\' sourceNames{iii}]);
    load([scorePath 'Liu2015\' sourceNames{iii}]);
    load([scorePath 'Liu2015\' sourceNames{iii}]);
    load([scorePath 'Bentaleb2016\' sourceNames{iii}]);
    load([scorePath 'SQI_SSIMplus\' sourceNames{iii}]);
    load([scorePath 'SSIMplus\' sourceNames{iii}]);
    load([scorePath 'VQM\' sourceNames{iii}]);
    load([scorePath 'NARX\' sourceNames{iii}]);
    load([scorePath 'ATLAS\' sourceNames{iii}]);
    load([scorePath 'P1203\' sourceNames{iii}]);
    
    QO_FTW = [QO_FTW; Q_FTW']; %#ok
    QO_Mok = [QO_Mok; Q_Mok']; %#ok
    QO_Liu12 = [QO_Liu12; Q_Liu12']; %#ok
    QO_VsQM = [QO_VsQM; Q_VsQM']; %#ok
    QO_Kim12 = [QO_Kim12; Q_Kim']; %#ok
    QO_Xue = [QO_Xue; Q_Xue']; %#ok
    QO_Yin = [QO_Yin; Q_Yin']; %#ok
    QO_Liu15 = [QO_Liu15; Q_Liu15']; %#ok
    QO_Bentaleb16 = [QO_Bentaleb16; Q_Bentaleb16']; %#ok
    QO_SQI_SSIMplus = [QO_SQI_SSIMplus; Q_SQI_SSIMplus']; %#ok
    QO_VQM = [QO_VQM; Q_VQM']; %#ok
    QO_SSIMplus = [QO_SSIMplus; Q_SSIMplus']; %#ok
    QO_NARX = [QO_NARX; Q_NARX']; %#ok
    QO_ATLAS = [QO_ATLAS; Q_ATLAS']; %#ok
    QO_P1203 = [QO_P1203; Q_P1203']; %#ok
end

MOS = MOS';

temp = feval('regress_hamid', QO_SSIMplus, MOS);
X(:,1) = temp - MOS;
temp = feval('regress_hamid', QO_VQM, MOS);
X(:,2) = temp - MOS;
temp = feval('regress_hamid', QO_Liu12, MOS);
X(:,3) = temp - MOS;
temp = feval('regress_hamid', QO_Yin, MOS);
X(:,4) = temp - MOS;
temp = feval('regress_hamid', QO_FTW, MOS);
X(:,5) = temp - MOS;
temp = feval('regress_hamid', QO_Bentaleb16, MOS);
X(:,6) = temp - MOS;
temp = feval('regress_hamid', QO_Kim12, MOS);
X(:,7) = temp - MOS;
temp = feval('regress_hamid', QO_Xue, MOS);
X(:,8) = temp - MOS;
temp = feval('regress_hamid', QO_Mok, MOS);
X(:,9) = temp - MOS;
temp = feval('regress_hamid', QO_VsQM, MOS);
X(:,10) = temp - MOS;
temp = feval('regress_hamid', QO_NARX, MOS);
X(:,11) = temp - MOS;
temp = feval('regress_hamid', QO_ATLAS, MOS);
X(:,12) = temp - MOS;
temp = feval('regress_logistic', QO_Liu15, MOS);
X(:,13) = temp - MOS;
temp = feval('regress_logistic', QO_SQI_SSIMplus, MOS);
X(:,14) = temp - MOS;
temp = feval('regress_logistic', QO_P1203, MOS);
X(:,15) = temp - MOS;


for i = 1 : size(X,2)
    for j = 1 : size(X,2)
        x = X(:,i);
        y = X(:,j);
        if(vartest2(x,y) == 0)
            H(i,j) = '-';
        else
            if(vartest2(x,y, 0.05, 'right') == 0)
                H(i,j) = '1';
            else
                H(i,j) = '0';
            end
        end
    end
end

H