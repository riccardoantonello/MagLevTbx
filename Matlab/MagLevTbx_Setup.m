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

%   set magnetometer variant (TLV493D-based boards only; v4.4 uses the TMAG5170)
if ismember(ver, {'4.0', '4.3'})
    selectTLV493DVariant(rootDir);
end

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


function selectTLV493DVariant(rootDir)
%   Ask which TLV493D variant is fitted and enable the matching
%   TLV493D_VARIANT_* define in TLV493D.h (the other one is commented out).

hFile = fullfile(rootDir, 'Blocks', 'MagSens', 'Lib', 'TLV493D', 'TLV493D.h');
variants = {'A1B6', 'A2B6'};
txt = fileread(hFile);

%   variant currently enabled in the header (uncommented #define)
cur = regexp(txt, '^[ \t]*#define[ \t]+TLV493D_VARIANT_(\w+)', ...
    'tokens', 'once', 'lineanchors');
if isempty(cur) || ~ismember(cur{1}, variants)
    cur = variants(1);
end

sel = questdlg('Select the magnetometer variant:', ...
    'MagLevTbx magnetometer', variants{:}, cur{1});
if isempty(sel)
    sel = cur{1};   % dialog closed: keep current setting
end

for i = 1:numel(variants)
    if strcmp(variants{i}, sel)
        prefix = '';
    else
        prefix = '// ';
    end
    txt = regexprep(txt, ...
        ['^[ \t]*(//[ \t]*)?(#define[ \t]+TLV493D_VARIANT_' variants{i} ')\>'], ...
        [prefix '$2'], 'lineanchors');
end

fid = fopen(hFile, 'w');
if fid < 0
    error('MagLevTbx:setup', 'Cannot write to %s', hFile);
end
fwrite(fid, txt);
fclose(fid);

fprintf('Magnetometer variant: TLV493D-%s\n\n', sel);

end