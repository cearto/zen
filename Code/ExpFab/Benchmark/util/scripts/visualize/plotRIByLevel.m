function plotRIByLevel(pathIn, fnOut, algorithms, mask)

% some global definitions
fontsize = 13;

levels = {
    'BySegmentation', 'ByModel', 'ByCategory', 'ByDataset'
};
nAlgorithms = size(algorithms, 2);
nLevels = size(levels, 2);
whatItems = [1,1]; interval = 1;
ylimit = 0.7;

% read in data
data = [];
for i = 1:nLevels
    fn = char(strcat(pathIn,levels(i),'.RI'));
    tmp = load(fn);
    data = [data; tmp(1,:)];
end
if size(mask,2) > 1
    data(1,:) = data(1,:).*mask;
    data(3:4,:) = data(3:4, :) .* [mask;mask];
end
data = data';

% plot
fn = char(strcat(fnOut, '.eps'));
fn1 = char(strcat(fnOut, '.jpg'));
h = bar(data);
if size(mask,2) > 1
    ch1 = get(h(1), 'children');
    fvcd1 = get(ch1, 'facevertexcdata');
    ch2 = get(h(2), 'children');
    fvcd2 = get(ch2, 'facevertexcdata');
    set(ch1, 'facevertexcdata', fvcd2);
    set(ch2, 'facevertexcdata', fvcd1);
end
xlim([0.5,0.5+nAlgorithms]);
ylim([0,ylimit]);
h1 = my_xticklabels(gca,1:nAlgorithms,algorithms);
set(h1, 'fontname', 'arial', 'fontsize', fontsize);
legend(h, levels, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Error', 'fontname', 'arial', 'fontsize', fontsize);
set(gca, 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
