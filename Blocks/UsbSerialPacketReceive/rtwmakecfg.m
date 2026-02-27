function makeInfo = rtwmakecfg()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
% 
% February 18, 2026
% 
% Dept. of Information Engineering, University of Padova 


%%  Root folders 

%   get full path of current dir
mfilepath = mfilename('fullpath');
[currdir, ~, ~] = fileparts(mfilepath);

%   get Support Packages base dir
baseRoot = matlabshared.supportpkg.getSupportPackageRoot();

%   get Teensyduino base dir
teensyRoot = fullfile(baseRoot, ...
    '3P.instrset', 'arduinoide.instrset', 'aCLI', 'data', ...
    'packages', 'teensy', 'hardware', 'avr', '1.58.2');

%   get Teensy cores dir
coreRoot = fullfile(teensyRoot, 'cores', 'teensy4');

%   get Teensy libraries dir
libRoot = fullfile(teensyRoot, 'libraries');


%%  Include paths (recursive)  

makeInfo.includePath    =   { ...
    coreRoot, ...
    fullfile(currdir, 'Lib', 'COBS') };


%%  Source paths

makeInfo.sourcePath     =   { ...
    fullfile(coreRoot), ...
    fullfile(currdir, 'Lib', 'COBS') };

makeInfo.sources        =   { ...
    'sfun_MagLevTbx_UsbSerialPacketReceive_WrappedFcns.cpp', ...
    'cobs.c' };

makeInfo.linkLibsObjs   =   {};
makeInfo.precompile     =   0;
makeInfo.library        =   {};

end