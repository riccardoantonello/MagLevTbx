function serialPacketSend(sp, packetData, cobsEncoded, nullTerminated)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Validate input arguments

arguments
    sp  (1,1)   internal.Serialport
    packetData (1,:) cell {mustBeValidPacketData}
    cobsEncoded (1,1) logical = true
    nullTerminated (1,1) logical = true
end


%%  Compute packet size in bytes

%   supported data types names
typeName = {
    'double',   ...
    'single',   ...
    'int8',    ...
    'uint8',    ...
    'int16',    ...
    'uint16',    ...
    'int32',    ...
    'uint32'  };

%   supported data types byte sizes
typeByteSize = [
    8, ...
    4, ...
    1, ...
    1, ...
    2, ...
    2, ...
    4, ...
    4 ];

%   compute packet byte size
packetByteSize = 0;
packetCellsNum = numel(packetData);
for n = 1:packetCellsNum
    [~, k] = ismember(class(packetData{n}), typeName);
    packetByteSize = packetByteSize + numel(packetData{n}) * typeByteSize(k);
end


%%  Serialize data 

%   preallocate buffer
serializedData = zeros(1, packetByteSize, 'uint8');

%   serialize data 
idx = 1;
for n = 1:packetCellsNum
    bytes = typecast(packetData{n}, 'uint8');
    nextIdx = idx + numel(bytes);

    serializedData(idx:nextIdx-1) = bytes;
    idx = nextIdx;
end


%%  Encode data

if cobsEncoded
    serializedData = cobsEncode(serializedData);
end


%%  Terminate packet

if nullTerminated
    serializedData = [serializedData, uint8(0)];
end


%%  Send packet

%   write the serialized data to the serial port
write(sp, serializedData, 'uint8');


end


%%  Input arguments validation functions

function mustBeValidPacketData(c)

    validDTypeNames = [ ...
        "double", ...
        "single", ...
        "int8", ...
        "uint8", ...
        "int16", ...
        "uint16", ...
        "int32", ...
        "uint32" ];

    if ~iscell(c)
        error("packetData must be a cell array.");
    end

    for k = 1:numel(c)
        x = c{k};

        if isempty(x)
            error("packetData{%d} must not be empty.", k);
        end
        
        if ~isnumeric(x)
            error("packetData{%d} must be numeric (scalar or array).", k);
        end

        if ~ismember(string(class(x)), validDTypeNames)
            error("packetData{%d} has type '%s'. Allowed types: %s.", ...
                k, class(x), strjoin(validDTypeNames, ", "));
        end

        if ~isreal(x)
            error("packetData{%d} must be real-valued.", k);
        end

        if issparse(x)
            error("packetData{%d} must not be sparse.", k);
        end

        if (isfloat(x) && any(~isfinite(x), "all"))
            error("packetData{%d} must not contain NaN or Inf.", k);
        end

    end
end



