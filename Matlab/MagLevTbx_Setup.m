function MagLevTbx_Setup()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% March 25, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   get current working directory
origdir = pwd;

%   get toolbox info
info = MagLevTbx_Info();
rootDir = info.rootDir;

%   print toolbox info
cd( fullfile(rootDir, 'Matlab') );
fprintf('---------------------------------\n');
fprintf('%s\nVersion %s\nLast update: %s\n', ...
    info.name, info.ver, info.lastUpdate);
fprintf('---------------------------------\n\n');

%   set hardware version
app = MagLevTbx_HwConfig();
uiwait(app.MagLevTbxHwConfigUIFigure);

ver = getpref('MagLevTbx', 'HwVersion');
fprintf('Hardware version: %s\n\n', ver);

%   create MEX files
fprintf('Create MEX files: \n\n');
cd( fullfile(rootDir, 'Matlab') );
MagLevTbx_MakeMex(rootDir, rootDir);

%   configure search paths
fprintf('Configure search paths: \n\n');
cd( fullfile(rootDir, 'Matlab') );
MagLevTbx_PathCfg(rootDir, rootDir);

%   save search path
savepath;

%   move back to original working directory
cd(origdir);

fprintf('Setup completed. \n\n');

end