%% Magnetometer recording
% Plots MagLev_data_x and MagLev_data_y (timeseries in the base workspace)


%   interval of interest [s]  (use [] for the whole recording)
tInterval = [ ];

%% Data
tx = MagLev_data_x.Time;    x = squeeze(MagLev_data_x.Data);
ty = MagLev_data_y.Time;    y = squeeze(MagLev_data_y.Data);

if isempty(tInterval)
    tInterval = [min(tx(1), ty(1)), max(tx(end), ty(end))];
end

%% Plot
figure('Name', 'Magnetometer recording');

ax(1) = subplot(2,1,1);
plot(tx, x); grid on; hold on;
ylabel('x');
title('Magnetometer recording');

ax(2) = subplot(2,1,2);
plot(ty, y); grid on; hold on;
ylabel('y'); xlabel('Time [s]');

linkaxes(ax, 'x');          % zooming/panning one axis moves the other
xlim(ax(1), tInterval);
