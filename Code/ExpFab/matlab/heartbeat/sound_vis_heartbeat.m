clear all; clc; 

[heart, fs] = wavread('heartbeat-01a.wav');
heart_short = heart(2000:40:6000, 2); %extract the first heartbeat; take samples such that the matrix is in a workable size
[x, y] = meshgrid(-5:0.1:5, -5:0.1:5); 
sigma = 1.5; %set sigma for gaussian
gauss = 1/(2*pi*sigma^2)*exp(-(x.^2+y.^2)/(2*sigma^2)); %create gaussian function
gauss = gauss./sum(gauss(:)); %normalize the guassian


test = cos(x)+sin(x); %a simple cosine and sine test function
gauss_test = test.* gauss; 
heart_short_sym = toeplitz(heart_short);%make the heartbeat matrix symmetrical
gauss_heart_short = heart_short_sym.*gauss;
figure(1), surfc(gauss); 
figure(2); surfc(gauss_test); %gaussianized (:D) test function 
figure(3); surfc(heart_short_sym); %hearbeat mesh plot without gaussian function 
figure(4); surfc(gauss_heart_short); %gaussianized heartbeat

