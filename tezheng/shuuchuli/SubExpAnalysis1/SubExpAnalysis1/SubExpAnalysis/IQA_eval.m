function [plcc mae rms srcc krcc] = IQA_eval(smos, sobj, regression_model)
% pmos = feval(regression_model, sobj, smos);
% srcc = corr(smos, pmos, 'type', 'Spearman');
% krcc = corr(smos, pmos, 'type', 'Kendall');
srcc = corr(smos, sobj, 'type', 'Spearman');
krcc = corr(smos, sobj, 'type', 'Kendall');
pmos = feval(regression_model, sobj, smos);
% [sobj_sort ind] = sort(sobj);
% pmos_sort =pmos(ind);
% figure;plot(sobj,smos,'r*',sobj_sort,pmos_sort,'b-');
plcc = corr(smos, pmos);
mae = mean(abs(smos - pmos));
rms = sqrt(mean((smos - pmos).^2));

return;