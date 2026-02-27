function MagLevTbx_UsbSerialPacketReceive_MaskInit(block, packetSpec, showStatusPort, showEnablePort) 

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%

%%  Remove existing blocks

%   get output ports handles 
outPortHandle = find_system(block, ...
    'RegExp', 'on', ...
    'LookUnderMasks', 'on', ...
    'FindAll', 'on', ...
    'FollowLinks', 'on', ...
    'SearchDepth', '1', ...
    'BlockType', 'Outport', ...
    'Name', 'Out*');

statusPortHandle = find_system(block, ...
    'LookUnderMasks', 'on', ...
    'FindAll', 'on', ...
    'FollowLinks', 'on', ...
    'SearchDepth', '1', ...
    'BlockType', 'Outport', ...
    'Name', 'Status Port');

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

%   remove unecessary output ports
cellNum = length(packetSpec);
newOutPortHandle = zeros(1, cellNum);

outPortNum = length(outPortHandle);
for n = 1:outPortNum
    outPortId = str2num( get_param(outPortHandle(n), 'Port') );
    if outPortId > cellNum
        delete_block(outPortHandle(n));
    else
        newOutPortHandle(n) = outPortHandle(n);
    end
end
outPortHandle = newOutPortHandle;

%   remove Status Port
if ~showStatusPort && ~isempty(statusPortHandle)
    delete_block(statusPortHandle);
end

%   remove S-Function block
delete_block(sfunHandle);

%   remove enable block
if ~showEnablePort && ~isempty(enableHandle)
    delete_block(enableHandle);
end


%%  Define block sizes and spacings

%   horizontal/vertical space between blocks
hspace = 30;
vspace = 24;

%   input port block size
outPortSize = [30, 14];

%   S-Function block size
sfunSize = (outPortSize + [0, vspace]) .* [6, cellNum];

%   Enable block size
enableSize = [20, 20];


%%  Place S-Function block

%   aux function
getBlockSize = @(pos) [pos(3)-pos(1), pos(4)-pos(2)];
getBlockPos = @(orig, size) [orig, orig+size];

%   S-Function position
sfunOrig = [20, 20];
sfunPos = getBlockPos(sfunOrig, sfunSize);
    
%   place S-Function
sfunParStr = 'packetSpec, cobsEncoded, nullTerminated, showStatusPort, outputMode, sampleTime';
sfunHandle = add_block( ...
    'simulink/User-Defined Functions/S-Function', [block, '/S-Function'], ...
	'Parameters', sfunParStr, ...
	'FunctionName', 'sfun_MagLevTbx_UsbSerialPacketReceive', ...
	'Position', sfunPos); 


%%  Place output ports blocks

sfunPortConn = get_param(sfunHandle, 'PortConnectivity');

%   place output ports
for n = 1:cellNum
    
    %   output port block position
    outPortOrig = sfunPortConn(n).Position + ...
        [ hspace, -0.5*outPortSize(2)];
    outPortPos = getBlockPos(outPortOrig, outPortSize);
   
    %   place output port block
    if outPortHandle(n) == 0 
        outPortHandle(n) = add_block( ...
            'simulink/Sinks/Out1', [block, sprintf('/Out%d',n)], ...
            'Position', outPortPos, ...
            'Port', num2str(n));
    else
        set_param(outPortHandle(n), ...
            'Position', outPortPos, ...
            'Port', num2str(n));
    end
    
end


%%  Place Status Port

if showStatusPort
    
    %   Status Port position
    statusPortOrig = sfunPortConn(cellNum+1).Position + ...
        [ hspace, -0.5*outPortSize(2)];
    statusPortPos = getBlockPos(statusPortOrig, outPortSize);
    
    %   place Status Port blovk
    if isempty(statusPortHandle)
        statusPortHandle = add_block( ...
            'simulink/Sinks/Out1', [block, '/Status Port'], ...
            'Position', statusPortPos, ...
            'Port', num2str(cellNum+1));
    else
        set_param(statusPortHandle, ...
            'Position', statusPortPos, ...
            'Port', num2str(cellNum+1));
    end
    
end


%%  Connect S-Function block to output ports

sfunPortHandles = get_param(sfunHandle, 'PortHandles');

%   connect S-Function to output ports 
for n = 1:cellNum
    outPortPortHandles = get_param(outPortHandle(n), 'PortHandles');
    add_line(block, sfunPortHandles.Outport(n), outPortPortHandles.Inport(1));
end

%   connect S-Function to Status Port
if showStatusPort
    statusPortPortHandles = get_param(statusPortHandle, 'PortHandles');
    add_line(block, sfunPortHandles.Outport(cellNum+1), statusPortPortHandles.Inport(1));
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

end  %  MagLevTbx_UsbSerialPacketReceive_MaskInit