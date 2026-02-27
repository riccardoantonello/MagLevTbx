function MagLevTbx_UsbSerialPacketSend_MaskInit(block, packetSpec, showEnablePort)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Management and Engineering, University of Padova
%

%%  Remove existing blocks

%   get input ports handles
inPortHandle = find_system(block, ...
    'LookUnderMasks', 'on', ...
    'FindAll', 'on', ...
    'FollowLinks', 'on', ...
    'SearchDepth', '1', ...
    'BlockType', 'Inport');

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

%   remove S-Function connections
sfunPortConn = get_param(sfunHandle, 'PortConnectivity');
sfunPortNum = length(sfunPortConn);
for n = 1:sfunPortNum
    delete_line(block, sfunPortConn(n).Position);
end

%   remove unecessary input ports
cellNum = length(packetSpec);
newInPortHandle = zeros(1, cellNum);

inPortNum = length(inPortHandle);
for n = 1:inPortNum
    inPortId = str2num( get_param(inPortHandle(n), 'Port') );
    if inPortId > cellNum
        delete_block(inPortHandle(n));
    else
        newInPortHandle(n) = inPortHandle(n);
    end
end
inPortHandle = newInPortHandle;

%   remove S-Function block
delete_block(sfunHandle);

%   remove enable block
if ~showEnablePort && ~isempty(enableHandle)
    delete_block(enableHandle);
end


%%  Define block sizes and spacings

%   horizontal/vertical space between blocks
hspace = 30;
vspace = 14;

%   input port block size
inPortSize = [30, 14];

%   S-Function block size
sfunSize = (inPortSize + [0, vspace]) .* [6, cellNum];

%   Enable block size
enableSize = [20, 20];


%%  Place S-Function block

%   aux function
getBlockSize = @(pos) [pos(3)-pos(1), pos(4)-pos(2)];
getBlockPos = @(orig, size) [orig, orig+size];

%   S-Function position
sfunOrig = [20, 20] + [ inPortSize(1) + hspace, vspace ];
sfunPos = getBlockPos(sfunOrig, sfunSize);
    
%   place S-Function
sfunParStr = 'packetSpec, cobsEncoded, nullTerminated, waitDTR, sampleTime';
sfunHandle = add_block( ...
    'simulink/User-Defined Functions/S-Function', [block, '/S-Function'], ...
    'Parameters', sfunParStr, ...
    'FunctionName', 'sfun_MagLevTbx_UsbSerialPacketSend', ...
    'Position', sfunPos);


%%  Place input ports blocks

sfunPortConn = get_param(sfunHandle, 'PortConnectivity');

for n = 1:cellNum
    
    %   input port block position
    inPortOrig = sfunPortConn(n).Position + ...
        [ -hspace-inPortSize(1), -0.5*inPortSize(2)];
    inPortPos = getBlockPos(inPortOrig, inPortSize);
   
    %   place input port block
    if inPortHandle(n) == 0 
        inPortHandle(n) = add_block( ...
            'simulink/Sources/In1', [block, sprintf('/In%d',n)], ...
            'Position', inPortPos);
    else
        set_param(inPortHandle(n), 'Position', inPortPos);
    end
     
end


%%  Connect input ports blocks to S-Function block

%   connect input ports blocks to S-Function 
sfunPortHandles = get_param(sfunHandle, 'PortHandles');
for n = 1:cellNum
    inPortPortHandles = get_param(inPortHandle(n), 'PortHandles');
    add_line(block, inPortPortHandles.Outport(1), sfunPortHandles.Inport(n));
end


%%  Place enable port

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

end  %  MagLevTbx_UsbSerialPacketSend_MaskInit
