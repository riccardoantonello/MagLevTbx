function MagLevTbx_MakeMex(currdir, basedir)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   move to specified directory
cd(currdir);

%   get relative path from toolbox base dir
relpath = strrep(currdir, strcat(basedir, filesep), '');

%   create mex files in current dir 
if isfile('makemex.m')
    
    %   create mex files
    fprintf('### Generate MEX files in directory: %s\n', relpath);
    makemex(basedir);
    fprintf('\n');
    
end

%   get subdirs
filelist = dir();

%   recursively explore each subdir in current dir
N = length(filelist);
for n = 1:N
    
    if filelist(n).isdir && ~ismember(filelist(n).name, {'.', '..'})
        cd( fullfile(basedir, 'Matlab') );
        MagLevTbx_MakeMex( ...
            fullfile( filelist(n).folder, filelist(n).name ), ...
            basedir ); 
    end
    
end

end
