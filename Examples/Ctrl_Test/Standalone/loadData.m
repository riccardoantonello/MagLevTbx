%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% May 02, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Main params

%   MagLev id number (selects the feedthrough matrix file to load)
magLevId = 19;

%   main (smallest) sampling time
Ts = 1/2500;


%%  Control params

%   control sampling time
ctrl.Ts = Ts;

%   PD gains
ctrl.Kp = 90;
ctrl.Kd = 0.35;

%   output saturation
ctrl.outMax = 150;
ctrl.outMin = -150;


%%  Sensors params

%   sensors sampling time
sens.Ts = Ts;

%   low-pass filter
fc = 100;                               %   cut-off freq [Hz]
wc = 2*pi*fc;                           %   cut-off freq [rad/s] 
sysF = tf(wc, [1, wc]);                 %   continuos-time filter
sysF = c2d(sysF, sens.Ts, 'tustin');    %   discretization
[numF, denF] = tfdata(sysF, 'v');       %   num/den

sens.lpf.fc = fc;
sens.lpf.wc = wc;
sens.lpf.num = numF;
sens.lpf.den = denF;

clear fc wc sysF numF denF;

%   high-pass filter
fc = 100;                               %   cut-off freq [Hz]
wc = 2*pi*fc;                           %   cut-off freq [rad/s] 
sysF = tf([wc, 0], [1, wc]);            %   continuos-time filter
sysF = c2d(sysF, sens.Ts, 'tustin');    %   discretization
[numF, denF] = tfdata(sysF, 'v');       %   num/den

sens.hpf.fc = fc;
sens.hpf.wc = wc;
sens.hpf.num = numF;
sens.hpf.den = denF;

clear fc wc sysF numF denF;

%   direct feedthrough matrix
load(fullfile('data', sprintf('DirFeedthroughMat_MagLev%02d.mat', magLevId)));
sens.Kft = dirFeedthroughMat;


%%  Enable logic params

%   enable logic sampling time
enable.Ts = Ts;

%   sensors turn-on/off time
enable.sensTurnOnTime = 0;
enable.sensTurnOffTime = Inf;

%   duration for bias evaluation
enable.biasEvalTime = 1;

%   controller turn-on time
enable.ctrlTurnOnTime = enable.sensTurnOnTime + enable.biasEvalTime  + 1;
enable.ctrlTurnOffTime = Inf;

%   z-axis mag field threshold [mT]
%   Note: control is enable only if the abs value exceed the threshold
enable.Bz_thr = 5;

%   heart beat sampling time
enable.heartBeat_Ts = 0.5;

%   add dummy variable to support Inf values in struct fiels
enable.dummy = {'ForceInline'};





