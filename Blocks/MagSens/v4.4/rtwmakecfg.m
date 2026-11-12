function makeInfo = rtwmakecfg()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
% 
% May 02, 2026
% 
% Dept. of Information Engineering, University of Padova 


%%  Root folders 

%   get full path of current dir
mfilepath = mfilename('fullpath');
[currdir, ~, ~] = fileparts(mfilepath);

%   get Support Packages base dir
baseRoot = matlabshared.supportpkg.getSupportPackageRoot();

%   get Teensyduino base dir
if ispc
    teensyRoot = fullfile(baseRoot, ...
        '3P.instrset', '..', 'aCLI', 'data', ...
        'packages', 'teensy', 'hardware', 'avr', '1.58.2');
else
    teensyRoot = fullfile(baseRoot, ...
        '3P.instrset', 'arduinoide.instrset', 'aCLI', 'data', ...
        'packages', 'teensy', 'hardware', 'avr', '1.58.2');
end

%   get Teensy cores dir
coreRoot = fullfile(teensyRoot, 'cores', 'teensy4');

%   get Teensy libraries dir
libRoot = fullfile(teensyRoot, 'libraries');


%%  Include paths (recursive)

%   currdir: board_v4_4_0_rc_1.h
makeInfo.includePath    =   { ...
    coreRoot, ...
    fullfile(libRoot, 'SPI'), ...
    fullfile(currdir, '..', 'Lib', 'TMAG5170'), ...
    currdir };


%%  Source paths

makeInfo.sourcePath     =   { ...
    fullfile(coreRoot), ...
    fullfile(libRoot, 'SPI'), ...
    fullfile(currdir, '..', 'Lib', 'TMAG5170') };

makeInfo.sources        =   { ...
    'sfun_MagLevTbx_MagSens_v44_WrappedFcns.cpp', ...
    'SPI.cpp', ...
    'TMAG5170.cpp', ...
    'TMAG5170_Hal_Arduino.cpp'};

makeInfo.linkLibsObjs   =   {};
makeInfo.precompile     =   0;
makeInfo.library        =   {};

end