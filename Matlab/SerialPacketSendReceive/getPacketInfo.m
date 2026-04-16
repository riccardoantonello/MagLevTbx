function packetInfo = getPacketInfo(packetSpec)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% April 16, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   supported data types names
validDTypeNames = {
    'double',   ...
    'single',   ...
    'int8',    ...
    'uint8',    ...
    'int16',    ...
    'uint16',    ...
    'int32',    ...
    'uint32'  };

%   supported data types byte sizes
validDTypeSizes = [
    8, ...
    4, ...
    1, ...
    1, ...
    2, ...
    2, ...
    4, ...
    4 ];

%   packetSpec must be a cell array of valid data type specifiers
if ~iscell(packetSpec)
    error("getPacketInfo:invalidPacketSpec", ...
        "packetSpec must be a cell array of data type specs like {'2*uint8','single'}.");
end

%   set number of packet cells
cellsNum = length(packetSpec);

%   init cellDType, cellDTypeSize and cellWidth arrays
cellDType = cell(1, cellsNum);
cellDTypeSize = zeros(1, cellsNum);
cellWidth = zeros(1, cellsNum);

%   init packet byte size
byteSize = 0;

%   parse packetSpec input argument
for n = 1:cellsNum
    
    %   validate data type specifier
    spec = packetSpec{n};
    if ~(ischar(spec) || (isstring(spec) && isscalar(spec)))
        error("getPacketInfo:invalidPacketSpec", ...
            "packetSpec{%d} must be a char vector or scalar string.", n);
    end

    %   match either "type" or "width*type" (allow spaces around *)
    spec = strtrim(string(spec));
    parts = split(spec, "*");
    if isscalar(parts)
        % Only dtype matched, no width part
        cellWidth(n) = 1;
        cellDType{n} = char(strtrim(parts));
    elseif numel(parts) == 2
        % Width + dtype matched
        cellWidth(n) = str2double(strtrim(parts(1)));
        cellDType{n} = strtrim(parts(2));
    else
        error("getPacketInfo:invalidPacketSpec", ...
            "Invalid data type spec '%s'. Use 'type' or 'width*type' (e.g. '2*int16').", spec);
    end

    %   integer width check (now guaranteed digits, but still good)
    if cellWidth(n) < 1 || floor(cellWidth(n)) ~= cellWidth(n)
        error("getPacketInfo:invalidPacketSpec", ...
            "Invalid data type spec '%s' - width must be a positive integer.", spec);
    end
    
    %   check for valid data type names
    [isValidDTypeName, validDTypeIdx] = ismember(cellDType{n}, validDTypeNames);
    
    if ~isValidDTypeName
        error("getPacketInfo:invalidPacketSpec", ...
            "Invalid data type spec '%s' - name must be of supported data types.", ...
            packetSpec{n});
    end
    
    %   save cell data type size (bytes)
    cellDTypeSize(n) = validDTypeSizes(validDTypeIdx);
    
    %   check for valid data width
    if cellWidth(n) <= 0
        error("getPacketInfo:invalidPacketSpec", ...
            "Invalid data type spec '%s' - width must be a positive number.", ...
            packetSpec{n});
    end
    
    %   update packet byte size
    byteSize = byteSize + cellDTypeSize(n)*cellWidth(n);
    
end

%   store results in packetInfo struct
packetInfo.byteSize = byteSize;
packetInfo.cellsNum = cellsNum;
packetInfo.cellDType = cellDType;
packetInfo.cellDTypeSize = cellDTypeSize;
packetInfo.cellWidth = cellWidth;

end     %   getPacketInfo