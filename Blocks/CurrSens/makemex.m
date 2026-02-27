function makemex(basedir)

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

%   Generate platform-specific mex files
fprintf('- Compile file: %s\n', fullfile(relpath, 'sfun_MagLevTbx_CurrSens.c'));
mex -R2018a sfun_MagLevTbx_CurrSens.c


end  %  makemex