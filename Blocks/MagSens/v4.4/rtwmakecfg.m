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

makeInfo.includePath    =   { ...
    coreRoot, ...
    fullfile(libRoot, 'Wire'), ...
    fullfile(currdir, '..', 'Lib', 'TCA9548'), ...
    fullfile(currdir, '..', 'Lib', 'TLV493D') };


%%  Source paths

makeInfo.sourcePath     =   { ...
    fullfile(coreRoot), ...
    fullfile(libRoot, 'Wire'), ...
    fullfile(currdir, '..', 'Lib', 'TCA9548'), ...
    fullfile(currdir, '..', 'Lib', 'TLV493D') };

makeInfo.sources        =   { ...
    'sfun_MagLevTbx_MagSens_v40_WrappedFcns.cpp', ...
    'Wire.cpp', ...
    'WireIMXRT.cpp', ...
    'TCA9548.cpp', ...
    'TLV493D.cpp'};

makeInfo.linkLibsObjs   =   {};
makeInfo.precompile     =   0;
makeInfo.library        =   {};

end