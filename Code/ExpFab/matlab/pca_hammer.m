good = csvread('data2.csv');
bad = csvread('false.csv');
good = good(:, 2:7);
good = good ./ norm(good);
bad = bad(:, 2:7) ./ norm(bad);

all = [good; bad];

[coeff1, eigValueDiag1] = princomp(all);

close all
figure;

imagesc(all * coeff1);

