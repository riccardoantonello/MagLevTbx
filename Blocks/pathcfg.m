function pathcfg(basedir)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   get full path of current dir
mfilepath = mfilename('fullpath');
[fullpath, ~, ~] = fileparts(mfilepath);

%   get relative path from toolbox base dir
relpath = strrep(fullpath, strcat(basedir, filesep), '');

%   Add directories to search path
fprintf('- Add directory: %s\n', relpath);
addpath(fullpath);


end  %  pathcfg