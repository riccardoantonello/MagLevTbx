function MagLevTbx_UserLED_MaskInit(block, showEnablePort)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 26, 2026
%
% Dept. of Management and Engineering, University of Padova
%


%%  Remove existing blocks

%   get S-Function handle
sfunHandle = find_system(block, ...
    'LookUnderMasks', 'on', ...
    'FindAll', 'on', ...
    'FollowLinks', 'on', ...
    'SearchDepth', '1', ...
    'BlockType', 'S-Function', ...
    'Name', 'S-Function');

%   get Enable block handle
enableHandle = find_system(block, ...
    'LookUnderMasks', 'on', ...
    'FindAll', 'on', ...
    'FollowLinks', 'on', ...
    'SearchDepth', '1', ...
    'BlockType', 'EnablePort');

%   remove enable block
if ~showEnablePort && ~isempty(enableHandle)
    delete_block(enableHandle);
end


%%  Define block sizes and spacings

%   horizontal/vertical space between blocks
hspace = 30;
vspace = 14;

%   Enable block size
enableSize = [20, 20];


%%  Place enable port

%   aux function
getBlockSize = @(pos) [pos(3)-pos(1), pos(4)-pos(2)];
getBlockPos = @(orig, size) [orig, orig+size];

%   S-Function block position and size
sfunPos = get_param(sfunHandle, 'Position');
sfunOrig = sfunPos(1:2);

if showEnablePort
    
    %   Enable block position
    enableOrig = sfunOrig + [0, -vspace-enableSize(2)];
    enablePos = getBlockPos(enableOrig, enableSize);
    
    %   place Enable block
    if isempty(enableHandle)
        add_block( ...
            'simulink/Ports & Subsystems/Enable', [block, '/Enable'], ...
            'Position', enablePos);
    else
        set_param(enableHandle, 'Position', enablePos);
    end
    
end

end


