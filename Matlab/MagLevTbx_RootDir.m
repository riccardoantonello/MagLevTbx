function rootDir = MagLevTbx_RootDir()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 17, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   get path of matlab subdir (where current Matlab function is stored)
fullpath = mfilename('fullpath');
[rootDir, ~, ~] = fileparts(fullpath);

%   get path of parent dir (toolbox base dir)
[rootDir, ~, ~] = fileparts(rootDir);

end  