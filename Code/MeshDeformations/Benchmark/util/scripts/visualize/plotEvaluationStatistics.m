%% plot evaluation results
function plotEvaluationStatistics(fnIn, dirOut, mode, suffix, algorithms, plotForCategories)

% some global definitions
fontsize = 13;
nAlgorithms = size(algorithms, 2);

% get parameters according to suffix
if strcmp(suffix, 'CD')
    items = {'CD'};
    whatItems = [1,1]; interval = 1;
    ylimit = 0.6;
elseif strcmp(suffix, 'CE')
    items = {'GCE', 'LCE'};
    whatItems = [3,4]; interval = 4;
    ylimit = 0.28;
elseif strcmp(suffix, 'HD')
    items = {'Hamming', 'Hamming-Rm', 'Hamming-Rf'};
    whatItems = [1,3]; interval = 3;
    ylimit = 1;
elseif strcmp(suffix, 'RI')
    items = {'RandIndex'};
    whatItems = [1,1]; interval = 1;
    ylimit = 0.7;
else
    quit;
end
        
% pre-process data
d = load(fnIn);
data = [];
for i = 0:(nAlgorithms-1)
    i1 = i*interval + whatItems(1);
    i2 = i*interval + whatItems(2);
    data = [data, d(:,i1:i2)];
end

% plot for all
fn = char(strcat(dirOut,mode,'-',suffix,'-all','.eps'));
fn1 = char(strcat(dirOut,mode,'-',suffix,'-all','.jpg'));
d = data(1,:); d = reshape(d, size(d,2)/nAlgorithms, nAlgorithms); d = d';
h = bar(d);
set(gca, 'fontsize', fontsize);
xlim([0, 1+nAlgorithms]);
ylim([0, ylimit]);
h1 = my_xticklabels(gca,1:nAlgorithms,algorithms);
set(h1, 'fontname', 'arial', 'fontsize', fontsize);
legend(h, items, 'Location', 'NorthWest', 'fontname', 'arial');
aspectRatio = get(gcf, 'PaperPosition');
aspectRatio(3:4) = [8, 5];
set(gcf, 'PaperPosition', aspectRatio);
ylabel('Error', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1)
close;

%plot for every category
if(plotForCategories)
   for i = 1:20
       fn = char(strcat(dirOut,mode,'-',suffix,'-',int2str(i), '.eps'));
       fn1 = char(strcat(dirOut,mode,'-',suffix,'-',int2str(i), '.jpg'));
       d = data(1+i,:); d = reshape(d, size(d,2)/nAlgorithms, nAlgorithms); d = d';
       h = bar(d);
       set(gca, 'fontsize', fontsize);
       xlim([0, 1+nAlgorithms]);
       ylim([0, ylimit]);
       h1 = my_xticklabels(gca,1:nAlgorithms,algorithms);
       set(h1, 'fontname', 'arial', 'fontsize', fontsize);
       legend(h, items, 'Location', 'NorthWest', 'fontname', 'arial');
       aspectRatio = get(gcf, 'PaperPosition');
       aspectRatio(3:4) = [8, 5];
       set(gcf, 'PaperPosition', aspectRatio);
       ylabel('Error', 'fontname', 'arial', 'fontsize', fontsize);
       saveas(gcf, fn, 'psc2');
       saveas(gcf, fn1);
       close;
   end
end

% output ranks per category according to RI
if(strcmp(suffix,'RI'))
    fn = char(strcat(dirOut,mode,'-',suffix,'-rank','.txt'));
    %data = data(:, 2:end); % delete the one for human
    d = [];
    for i = 1:nAlgorithms
        alg = char(strcat(algorithms{1,i}(1),algorithms{1,i}(2))); 
        if ~strcmp(alg, 'Human')
            d = [d, data(:,i)];
        end
    end
    data = d';
    [tt, I] = sort(data);
    I = I';
    rank = I;
    for i = 1:size(I,1)
        for j = 1:size(I,2)
            rank(i, I(i,j)) = j;
        end
    end
    rank = rank';
    format = '';
    for i = 1:(size(rank,1)-1)
        format = char(strcat(format, '%d\t'));
    end
    format = strcat(format,'%d\n');
    fid = fopen(fn, 'wt');
    fprintf(fid, format, rank);
    fclose(fid);
end

