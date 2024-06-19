function[prst_mean, prst_var, prst_kur] = presentation_statistcs(data)
    num = sum(~isnan(data), 1);
    prst_mean = sum(data .* ~isnan(data), 1)./ num;
    prst_var = sum( (data - repmat(prst_mean, [size(data,1),1] ) ).^2 .* ~isnan(data), 1 ) ./ (num-1);
    m2 = prst_var .* (num-1) ./ num;
    m4 = sum( ( data - repmat(prst_mean,[size(data,1),1]) ).^4 .* (~isnan(data)), 1 ) ./ (num);
    prst_kur=m4./(m2).^2;
end