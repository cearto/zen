%% plot statistics graphs
function plotAnalysisResults(algorithm, dirIn, dirOut, hasYAxis)

%% global variables
fontsize = 26;
linewidth = 1.5;
%% flags deciding which property to plot
doLength = 1;
doLengthRatio = 1;%1;
doArea = 1;
doAreaRatio = 1;%1;
doAngle = 1;
doCurvMin = 1;%1;
doCurvMax = 1;
doCurvMean = 1;
doCurvGauss = 1;
doCurvValley = 1;%1;
doCurvValley1 = 1;
doConvexity = 0;%1;
doNSeg = 1;

%% boundary lengths
if doLength
fn = [dirOut 'length.eps']
fn1 = [dirOut 'length.jpg'];
length = load([dirIn 'all.length']);
x = linspace(0,3,40);
[n, x] = hist(length/2/pi, x);
plot(x, n/sum(n), 'linewidth', linewidth);
%max(n/sum(n))
xlim([0 2.5]);
ylim([0 0.312]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
xlabel('Cut Perimeter', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency','fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
end

%% boundary lengths ratio
if doLengthRatio
fn = [dirOut 'lengthRatio.eps']
fn1 = [dirOut 'lengthRatio.jpg']
lengthRatio = load([dirIn 'all.lengthRatio']);
x = linspace(0,3.5,40);
[n, x] = hist(lengthRatio, x);
plot(x, n/sum(n), 'linewidth', linewidth);
%max(n/sum(n))
xlim([0 3]);
ylim([0 0.166]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
xlabel('Normalized Cut Perimeter','fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency','fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);  
close;
end

%% segment areas
if doArea
fn = [dirOut 'area.eps']
fn1 = [dirOut 'area.jpg']
area = load([dirIn 'all.area']);
x = linspace(0, 1, 40);
[n, x] = hist(area, x);
%max(n/sum(n))
plot(x,n/sum(n), 'linewidth', linewidth);
xlim([0 1]);
ylim([0 0.23]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
xlabel('Area', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1) 
close;
end

%% segment areas ratio
if doAreaRatio
fn = [dirOut 'areaRatio.eps']
fn1 = [dirOut 'areaRatio.jpg']
areaRatio = load([dirIn 'all.areaRatio']);
x = linspace(0,5.01,40);
[n, x] = hist(areaRatio, x);
plot(x,n/sum(n),'linewidth', linewidth);
xlim([0 4.85]);
ylim([0 0.166]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
xlabel('Normalized Segment Area', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1) 
close;
end

%% dehedral angle
if doAngle
fn = [dirOut 'angle.eps']
fn1 = [dirOut 'angle.jpg']
angleOnCut = load([dirIn 'all.angleOnCut']);
angleOnCut = angleOnCut/sum(angleOnCut);
angleNotOnCut = load([dirIn 'all.angleNotOnCut']);
angleNotOnCut = angleNotOnCut / sum(angleNotOnCut);
h = plot([angleOnCut, angleNotOnCut], 'linewidth', linewidth);
xlim([130 230]);
ylim([0 0.221]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Edges on Cuts', 'Others'}, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Angle in degrees','fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency','fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1); 
close;
end

%% cuvature
% 1. min
if doCurvMin
fn = [dirOut 'curv-min.eps']
fn1 = [dirOut 'curv-min.jpg']
curvOnCut = load([dirIn 'all.curvOnCut-min']);
curvNotOnCut = load([dirIn 'all.curvNotOnCut-min']);
curvOnCut(:, 2) = curvOnCut(:, 2) / sum(curvOnCut(:, 2));
curvNotOnCut(:, 2) = curvNotOnCut(:, 2) / sum(curvNotOnCut(:, 2));
x = curvOnCut(:,1);
x = [x x];
y = [curvOnCut(:,2) curvNotOnCut(:, 2)];
h = plot(x, y, 'linewidth', linewidth);
xlim([-45,20]);
ylim([0 0.275]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Vertices on Cuts', 'Others'}, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Curvature', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
end
% 2. max
if doCurvMax
fn = [dirOut 'curv-max.eps']
fn1 = [dirOut 'curv-max.jpg']
curvOnCut = load([dirIn 'all.curvOnCut-max']);
curvNotOnCut = load([dirIn 'all.curvNotOnCut-max']);
curvOnCut(:, 2) = curvOnCut(:, 2) / sum(curvOnCut(:, 2));
curvNotOnCut(:, 2) = curvNotOnCut(:, 2) / sum(curvNotOnCut(:, 2));
x = curvOnCut(:,1);
x = [x x];
y = [curvOnCut(:,2) curvNotOnCut(:, 2)];
max(y)
if 1
h = plot(x, y, 'linewidth', linewidth);
xlim([-15,55]);
ylim([0 0.159]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Vertices on Cuts', 'Others'}, 'Location', 'NorthEast', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Curvature', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1) 
close;
end
end
% 3. mean
if doCurvMean
fn = [dirOut 'curv-mean.eps']
fn1 = [dirOut 'curv-mean.jpg']
curvOnCut = load([dirIn 'all.curvOnCut-mean']);
curvNotOnCut = load([dirIn 'all.curvNotOnCut-mean']);
curvOnCut(:, 2) = curvOnCut(:, 2) / sum(curvOnCut(:, 2));
curvNotOnCut(:, 2) = curvNotOnCut(:, 2) / sum(curvNotOnCut(:, 2));
x = curvOnCut(:,1);
x = [x x];
y = [curvOnCut(:,2) curvNotOnCut(:, 2)];
%max(y)
h = plot(x, y, 'linewidth', linewidth);
xlim([-50,40]);
ylim([0 0.171]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Vertices on Cuts', 'Others'}, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Curvature', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
end
% 4. gauss
if doCurvGauss
fn = [dirOut 'curv-gauss.eps']
fn1 = [dirOut 'curv-gauss.jpg']
curvOnCut = load([dirIn 'all.curvOnCut-gauss']);
curvNotOnCut = load([dirIn 'all.curvNotOnCut-gauss']);
curvOnCut(:, 2) = curvOnCut(:, 2) / sum(curvOnCut(:, 2));
curvNotOnCut(:, 2) = curvNotOnCut(:, 2) / sum(curvNotOnCut(:, 2));
x = curvOnCut(:,1);
x = [x x];
y = [curvOnCut(:,2) curvNotOnCut(:, 2)];
%max(y)
h = plot(x, y, 'linewidth', linewidth);
xlim([-120,80]);
ylim([0 0.221]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Vertices on Cuts', 'Others'}, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Curvature', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1); 
close;
end
% 5. valley
if doCurvValley
fn = [dirOut 'curv-valley.eps']
fn1 = [dirOut 'curv-valley.jpg'];
curvOnCut = load([dirIn 'all.curvOnCut-valley']);
curvNotOnCut = load([dirIn 'all.curvNotOnCut-valley']);
curvOnCut(:, 2) = curvOnCut(:, 2) / sum(curvOnCut(:, 2));
curvNotOnCut(:, 2) = curvNotOnCut(:, 2) / sum(curvNotOnCut(:, 2));
x = curvOnCut(:,1);
len = size(x,1);
interval = 3;
pos = 1:interval:len;
x = x(pos);
x = [x x];
d1 = curvOnCut(:,2); d2 = curvNotOnCut(:, 2);
for i = 2:interval
    d1(1:end-i+1) = d1(1:end-i+1) + curvOnCut(i:end,2);
    d2(1:end-i+1) = d2(1:end-i+1) + curvNotOnCut(i:end,2);
end
d1 = d1(pos); d2 = d2(pos);
%max(d2)
y = [d1 d2];
h = plot(x, y, 'linewidth', linewidth);
xlim([-2,400]);
ylim([0 0.091]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Vertices on Cuts', 'Others'}, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Curvature Derivative', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1); 
close;
end
% 6. valley1
if doCurvValley1
fn = [dirOut 'curv-valley1.eps']
fn1 = [dirOut 'curv-valley1.jpg'];
curvOnCut = load([dirIn 'all.curvOnCut-valley1']);
curvNotOnCut = load([dirIn 'all.curvNotOnCut-valley1']);
curvOnCut(:, 2) = curvOnCut(:, 2) / sum(curvOnCut(:, 2));
curvNotOnCut(:, 2) = curvNotOnCut(:, 2) / sum(curvNotOnCut(:, 2));
x = curvOnCut(:,1);
x = [x x];
y = [curvOnCut(:,2) curvNotOnCut(:, 2)];
%sum(curvOnCut(:,2))
%sum(curvNotOnCut(:,2))
h = plot(x, y, 'linewidth', linewidth);
xlim([-800,800]);
ylim([0,0.0344]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
legend(h, {'Vertices on Cuts', 'Others'}, 'Location', 'NorthWest', 'fontname', 'arial', 'fontsize', fontsize);
xlabel('Curvature Derivative', 'fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency', 'fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
end

%% convexity
if doConvexity
data = load([dirIn 'all.convexity']);
% convexity
fn = [dirOut 'convexity.eps']
fn1 = [dirOut 'convexity.jpg']
convexity = data(:, 1);
x = linspace(0,0.1,36);
[n, x] = hist(convexity, x);
%max(n/sum(n));
plot(x, n/sum(n), 'linewidth', linewidth);
xlim([-0.001, 0.097]);
ylim([0, 0.166]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
xlabel('Convexity','fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency','fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
% compactness
fn = [dirOut 'compactness.eps']
fn1 = [dirOut 'compactness.jpg'];
compactness = data(:, 2);
x = linspace(4,16,50);
[n, x] = hist(compactness, x);
%max(n/sum(n))
plot(x, n/sum(n), 'linewidth', linewidth);
xlim([4.3, 15]);
ylim([0, 0.153]);
if ~hasYAxis
    set(gca, 'ytick', []);
end
set(gca, 'fontsize', fontsize);
xlabel('Compactness','fontname', 'arial', 'fontsize', fontsize);
ylabel('Frequency','fontname', 'arial', 'fontsize', fontsize);
saveas(gcf, fn, 'psc2');
saveas(gcf, fn1);
close;
end

%% nseg
if doNSeg
data = load([dirIn 'all.nseg']);
for i = 1:2
    fn = 0;
    if i == 1
       fn = [dirOut 'nseg-unfiltered.eps']
       fn1 = [dirOut 'nseg-unfiltered.jpg']
    else
       fn = [dirOut 'nseg.eps']
       fn1 = [dirOut 'nseg.jpg']
    end
    nseg = data(:,i);
    x = 1:26;
    [n, x] = hist(nseg, x);
    %max(n/sum(n))
    if 1
    stairs(x, n/sum(n), 'linewidth', linewidth);
    xlim([1, 27]);
    ylim([0, 0.21]);
    xlabel('Number of Segments','fontname', 'arial', 'fontsize', fontsize);
    ylabel('Frequency','fontname', 'arial', 'fontsize', fontsize);
    set(gca, 'fontsize', fontsize);
    if ~hasYAxis
        set(gca, 'ytick', []);
    end
    set(gca, 'xtick', [2, 8, 14, 20, 26], 'xticklabel', {'2', '8', '14', '26', '>25'});
    saveas(gcf, fn, 'psc2');
    saveas(gcf, fn1);
    close;
    end
end
end

