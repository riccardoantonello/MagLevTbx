%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% April 16, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Define MagLev id number

%   MagLev id number (used in filenames)
magLevId = 19;


%%  Create serial object

%   create serialport object
fprintf("Connecting ... ");
sp = serialport("/dev/cu.usbmodem163410501", 115200);
fprintf("Done.\n");

%   reset DTR
setDTR(sp, false);
flush(sp);

%   pause before continuing (wait serial port to set)
pause(1.0);


%%  Main loop

%   coils to test
%   1 - coil X+
%   2 - coil X-
%   3 - coil Y+
%   4 - coil Y-
coilId = [1, 2, 3, 4];
coilIdNum = numel(coilId);

%   duty cycle levels to test
dutyLevels = [0, 50, 100, 150, 100, 50, 0, -50, -100, -150, -100, -50, 0];
dutyLevelsNum = numel(dutyLevels); 

%   number of measurement samples to collect for each duty cycle level
samplesPerAverage = 25;

%   packet sending options
nullTerminated = true;
cobsEncoded = true;

%   measurement buffers
N = samplesPerAverage * dutyLevelsNum;
current = zeros(coilIdNum, N);
magFieldX = zeros(coilIdNum, N);
magFieldY = zeros(coilIdNum, N);
magFieldZ = zeros(coilIdNum, N);

%   average buffers
currentAvg = zeros(coilIdNum, dutyLevelsNum);
magFieldXAvg = zeros(coilIdNum, dutyLevelsNum);
magFieldYAvg = zeros(coilIdNum, dutyLevelsNum);
magFieldZAvg = zeros(coilIdNum, dutyLevelsNum);

%   main iteration
for nCoil = 1:coilIdNum
    
    m = 1;
    for nDuty = 1:dutyLevelsNum

        %   set duty level
        duty = dutyLevels(nDuty);
        clc;
        fprintf("Apply duty level %3.1f to coil %d\n", duty, nCoil);
        
        dataToSend = {int16(0), int16(0), int16(0), int16(0)};
        dataToSend{coilId(nCoil)} = int16(duty);
        serialPacketSend(sp, dataToSend, cobsEncoded, nullTerminated);

        %   wait for current to settle after transient
        fprintf("Wait for current to settle ... ");
        pause(0.5);
        fprintf("Done.\n");

        %   prepare progress bar indicator
        fprintf("Get current and magnetic field measurements ...\n");
        fprintf("0%% ................. 50%% ................. 100%%\n");
        barLength = 47;
        numBars = 0;

        %   get current and magnetic field measurements
        for n = 1:samplesPerAverage

            %   send duty cycles for current drivers
            serialPacketSend(sp, dataToSend, cobsEncoded, nullTerminated);

            %   receive current and magnetic field measurements
            setDTR(sp, true);
            dataReceived = serialPacketReceive(sp, {'3*single', '4*single'});
            setDTR(sp, false);

            %   store data to buffers
            magFieldX(nCoil,m) = dataReceived{1}(1);
            magFieldY(nCoil,m) = dataReceived{1}(2);
            magFieldZ(nCoil,m) = dataReceived{1}(3);
            current(nCoil,m) = dataReceived{2}(coilId(nCoil));

            %   cumulative sum for computing averages
            magFieldXAvg(nCoil,nDuty) = magFieldXAvg(nCoil,nDuty) + magFieldX(nCoil,m);
            magFieldYAvg(nCoil,nDuty) = magFieldYAvg(nCoil,nDuty) + magFieldY(nCoil,m);
            magFieldZAvg(nCoil,nDuty) = magFieldZAvg(nCoil,nDuty) + magFieldZ(nCoil,m);
            currentAvg(nCoil,nDuty) = currentAvg(nCoil,nDuty) + current(nCoil,m);

            %   increment sample index
            m = m + 1;

            %   pause before next measurement
            pause(0.05);

            %   update progress bar
            progress = round(n/samplesPerAverage*barLength);
            if progress > numBars
                fprintf(repmat('|', 1, progress-numBars));
                numBars = progress;
            end

        end
        fprintf("\nDone.\n");

        %   compute averages
        magFieldXAvg(nCoil,nDuty) = magFieldXAvg(nCoil,nDuty)/samplesPerAverage;
        magFieldYAvg(nCoil,nDuty) = magFieldYAvg(nCoil,nDuty)/samplesPerAverage;
        magFieldZAvg(nCoil,nDuty) = magFieldZAvg(nCoil,nDuty)/samplesPerAverage;
        currentAvg(nCoil,nDuty) = currentAvg(nCoil,nDuty)/samplesPerAverage;

        %   turn off coils before next duty level
        fprintf("Cool down coils ... ");
        dataToSend = {int16(0), int16(0), int16(0), int16(0)};
        serialPacketSend(sp, dataToSend, cobsEncoded, nullTerminated);
        flush(sp);
        pause(1);
        fprintf("Done.\n\n");
        pause(0.5);

    end
end

%   turn off coils
fprintf("Turn off coils.\n");
dataToSend = {int16(0), int16(0), int16(0), int16(0)};
serialPacketSend(sp, dataToSend, cobsEncoded, nullTerminated);


%%  Remove serial object

clear sp;


%%  Save calibration test data

%   check if "data" folder exists
if ~isfolder('data')
    mkdir('data')
end

%   save the averaged measurements to a file
save(fullfile('data', sprintf('CalTestData_MagLev%02d.mat', magLevId)), ...
    'coilId', 'coilIdNum', ...
    'dutyLevels', 'dutyLevelsNum', ...
    'current', 'magFieldX', 'magFieldY', 'magFieldZ', ...
    'currentAvg', 'magFieldXAvg', 'magFieldYAvg', 'magFieldZAvg');

