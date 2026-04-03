function makeInfo = rtwmakecfg()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
% 
% February 16, 2026
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

makeInfo.includePath    =   { coreRoot };


%%  Source paths

makeInfo.sourcePath     =   {};
makeInfo.sources        =   { 'sfun_MagLevTbx_CurrSens_WrappedFcns.cpp' };

makeInfo.linkLibsObjs   =   {};
makeInfo.precompile     =   0;
makeInfo.library        =   {};

end