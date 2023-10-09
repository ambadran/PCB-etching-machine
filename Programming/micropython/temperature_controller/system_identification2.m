%%% Extracting data from python csv file output %%%
python_data = csvread('python_output2.csv')

% Step Response
ytemp = transpose(python_data(1,:))/124  % dividing by max value to normalize
% now designing a lowpass filter to remove noise
filtertype = 'FIR';
Fs = 1/60;
N = 3;
Fpass = 1/(24*60*60);
Fstop = 1/(2*60*60);
Rp = 0.5;
Astop = 50;
LPF = dsp.LowpassFilter('SampleRate',Fs,...
                        'FilterType',filtertype,...
                        'PassbandFrequency',Fpass,...
                        'StopbandFrequency',Fstop,...
                        'PassbandRipple',Rp,...
                        'StopbandAttenuation',Astop);
ytemp_filtered = step(LPF, ytemp)

% time constants
dt = 0.1  % in ms
t = 0:dt:((length(ytemp)-1)*dt)

% Step input u(s)
u = ones(length(t), 1);

% Plotting
plot(t, [u, ytemp, ytemp_filtered], 'LineWidth', 4)
% axis([0 5 0 1.4]);
grid on
legend(['u'; 'y']);

%%%%% System Identification for the data read previously :D %%%%%
% fit data to a transfer function with unknown delay term
data = iddata(ytemp_filtered, u, dt);
Gestimate = tfest(data, 2, 0, NaN)  % getting G_est(s) 

%%%% Model Validation %%%%

%%%% Model Validation %%%%

% now that we have Gestimate, we can find how different the estimate is
% from the simulated original model
opt = compareOptions;
opt.InitialCondition = 'z';
compare(data, Gestimate, opt);
set(findall(gca, 'Type', 'Line'), 'LineWidth', 4);
grid on
































