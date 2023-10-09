

%%% Extracting data from python csv file output %%%
python_data = csvread('python_output.csv')

% x = python_data(1,:)
% time constants
dt = 0.1;
x = 0:dt:110.1;
ytemp = transpose(python_data(2,:))
yheater = transpose(python_data(3,:))

% Plotting
plot(x, [ytemp, yheater], 'LineWidth', 4)
% axis([0 5 0 1.4]);
grid on

%%%%% System Identification for the data read previously :D %%%%%
% fit data to a transfer function with unknown delay term
data = iddata(ytemp, yheater, dt);
Gestimate = tfest(data, 2, 0, NaN)  % getting G_est(s) 


%%%% Model Validation %%%%

% now that we have Gestimate, we can find how different the estimate is
% from the simulated original model
opt = compareOptions;
opt.InitialCondition = 'z';
compare(data, Gestimate, opt);
set(findall(gca, 'Type', 'Line'), 'LineWidth', 4);
grid on




