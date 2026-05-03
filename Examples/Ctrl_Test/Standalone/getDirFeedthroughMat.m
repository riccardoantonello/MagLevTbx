%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% April 16, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Load calibration test data

%   MagLev id number
magLevId = 19;

%   load data
load(fullfile('data', sprintf('CalTestData_MagLev%02d.mat', magLevId)));


%%  Compute direct feedthrough matrix

%   linear LS fitting coeffs
fitCoeffsX = zeros(coilIdNum, 2);
fitCoeffsY = zeros(coilIdNum, 2);
fitCoeffsZ = zeros(coilIdNum, 2);

for n = 1:4

    fitCoeffsX(n,:) = polyfit(currentAvg(n,:), magFieldXAvg(n,:), 1);
    fitCoeffsY(n,:) = polyfit(currentAvg(n,:), magFieldYAvg(n,:), 1);
    fitCoeffsZ(n,:) = polyfit(currentAvg(n,:), magFieldZAvg(n,:), 1);

end

%   compute direct feedthrough matrix
dirFeedthroughMat = zeros(3, coilIdNum);
for n = 1:coilIdNum
    dirFeedthroughMat(1,n) = fitCoeffsX(n,1);
    dirFeedthroughMat(2,n) = fitCoeffsY(n,1);
    dirFeedthroughMat(3,n) = fitCoeffsZ(n,1);
end

%   display the direct feedthrough matrix
disp('Direct Feedthrough Matrix:');
disp(dirFeedthroughMat);

%   save matrix to a .mat file
save(fullfile('data', sprintf('DirFeedthroughMat_MagLev%02d.mat', magLevId)), ...
    'dirFeedthroughMat');


%%  Plot data - Mag Field & Current vs Time

for nFig = 1:3

    figure;
    cmap = colormap('lines');
    colorXp = cmap(1,:);
    colorXn = cmap(2,:);
    colorYp = cmap(3,:);
    colorYn = cmap(4,:);

    subplot(3, 3, 5);
    switch nFig
        case 1, y = magFieldX;
        case 2, y = magFieldY;
        case 3, y = magFieldZ;
    end

    plot(y(1,:), 'LineWidth', 1.0, 'LineStyle', '-', 'Color', colorXp);
    hold on;
    plot(y(2,:), 'LineWidth', 1.0, 'LineStyle', '-', 'Color', colorXn);
    plot(y(3,:), 'LineWidth', 1.0, 'LineStyle', '-', 'Color', colorYp);
    plot(y(4,:), 'LineWidth', 1.0, 'LineStyle', '-', 'Color', colorYn);
    xlabel('Sample #');
    ylabel('Mag Field [mT]');
    title('Magnetic Field Axis: X');
    xlim([0, size(y,2)]);
    grid on;

    subplot(3, 3, 6);
    plot(current(1,:), 'Color', colorXp);
    xlabel('Sample #');
    ylabel('Current [A]');
    title('Active Coil: X+');
    xlim([0, size(current,2)]);
    grid on;

    subplot(3, 3, 4);
    plot(current(2,:), 'Color', colorXn);
    xlabel('Sample #');
    ylabel('Current [A]');
    title('Active Coil: X-');
    xlim([0, size(current,2)]);
    grid on;

    subplot(3, 3, 2);
    plot(current(3,:), 'Color', colorYp);
    xlabel('Sample #');
    ylabel('Current [A]');
    title('Active Coil: Y+');
    xlim([0, size(current,2)]);
    grid on;

    subplot(3, 3, 8);
    plot(current(4,:), 'Color', colorYn);
    xlabel('Sample #');
    ylabel('Current [A]');
    title('Active Coil: Y-');
    xlim([0, size(current,2)]);
    grid on;

end


%%  Plot data - Mag Field vs Current

figTitleStr = { ...
    'Magnetic Field Axis: X', ...
    'Magnetic Field Axis: Y', ...
    'Magnetic Field Axis: Z'};

plotTitleStr = { ...
    'Active Coil: X+', ...
    'Active Coil: X-', ...
    'Active Coil: Y+', ...
    'Active Coil: Y-' };

plotPosId = [6, 4, 2, 8];

for nFig = 1:3

    figure;
    for n = 1:4
    
        %   plot data
        subplot(3, 3, plotPosId(n));
        x = currentAvg(n,:);
        switch nFig
            case 1, y = magFieldXAvg(n,:);
            case 2, y = magFieldYAvg(n,:);
            case 3, y = magFieldZAvg(n,:);
        end
                
        plot(x, y, ...
            'LineWidth', 1.0, 'LineStyle', 'none', ...
            'MarkerSize', 6.0, 'Marker', 'o');
        hold on;
    
        %   plot linear LS fitting
        xv = [min(x), max(x)]; 
        switch nFig
            case 1, yfit = polyval(fitCoeffsX(n,:), xv);
            case 2, yfit = polyval(fitCoeffsY(n,:), xv);
            case 3, yfit = polyval(fitCoeffsZ(n,:), xv);
        end
        plot(xv, yfit, ...
            'LineWidth', 1.0, 'LineStyle', '--', 'Color', 'k');
    
        xlabel('Current [A]');
        ylabel('Mag Field [mT]');
        title(plotTitleStr{n});
        grid on;

        switch nFig
            case 1, ylim([-2, 2]);
            case 2, ylim([-2, 2]);
            case 3, ylim([-25, -5]);
        end

    end

    %   add figure title
    sgtitle( figTitleStr{nFig}, ...
        'FontWeight', 'bold', 'Color', 'red');

end