function [plcc, srcc, rmse] = eabl_quality_metrics(mos, predicted)
%EABL_QUALITY_METRICS Evaluate paired MOS and predicted scores.
% SRCC uses the raw predictions. PLCC and RMSE use predictions mapped
% to the MOS scale by a four-parameter logistic fit.
% Requires Statistics and Machine Learning Toolbox.

validateattributes(mos, {'numeric'}, {'vector', 'real', 'finite', 'nonempty'});
validateattributes(predicted, {'numeric'}, ...
    {'vector', 'real', 'finite', 'numel', numel(mos)});
mos = double(mos(:));
predicted = double(predicted(:));

srcc = corr(mos, predicted, 'Type', 'Spearman');
initialParameters = [max(mos), min(mos), mean(predicted), 0.5];
[parameters, residuals, jacobian] = nlinfit( ...
    predicted, mos, @logisticMapping, initialParameters);
mapped = nlpredci(@logisticMapping, predicted, parameters, residuals, jacobian);
plcc = corr(mos, mapped, 'Type', 'Pearson');
rmse = sqrt(mean((mos - mapped).^2));
end

function mapped = logisticMapping(parameters, predicted)
upper = parameters(1);
lower = parameters(2);
center = parameters(3);
width = abs(parameters(4));
mapped = (upper - lower) ./ (1 + exp(-(predicted - center) / width)) + lower;
end
