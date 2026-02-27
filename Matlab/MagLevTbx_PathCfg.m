function MagLevTbx_PathCfg(currdir, basedir)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Management and Engineering, University of Padova
%

%   move to specified directory
cd(currdir);

%   get relative path from toolbox base dir
relpath = strrep(currdir, strcat(basedir, filesep), '');

%   configure search path for specified directory
if isfile('pathcfg.m')
    
    %   add extra paths to current search path
    fprintf('### Configure paths for directory: %s \n', relpath);
    pathcfg(basedir);
    fprintf('\n');
    
end

%   get subdirs
filelist = dir();

%   recursively explore each subdir in current dir
N = length(filelist);
for n = 1:N
    
    if filelist(n).isdir && ~ismember(filelist(n).name, {'.', '..'})
        cd( fullfile(basedir, 'Matlab') );
        MagLevTbx_PathCfg( ...
            fullfile( filelist(n).folder, filelist(n).name ), ...
            basedir ); 
    end
    
end

end