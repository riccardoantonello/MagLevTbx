function packedData = dataPack(unpackedData, packetInfo)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Validate input arguments

arguments
    unpackedData (1,:) cell
    packetInfo (1,1) struct {mustBeValidPacketInfo}
end


%%  Pack data  

%   init packed data array
packedDataLen = packetInfo.byteSize;
packedData = zeros(1, packedDataLen, 'uint8');

%   start index to packed data of current cell element
inIdx = 1;

for n = 1:packetInfo.cellsNum
    
    %   cell must be numeric and not empty
    if ~isnumeric(unpackedData{n}) || isempty(unpackedData{n})
        error("dataPack:invalidCell", ...
            "unpackedData{%d} must be non-empty numeric.", n);
    end

    %   cell must match the specified type
    if ~isa(unpackedData{n}, packetInfo.cellDType{n})
        error("dataPack:unmatchedDType", ...
            "unpackedData{%d} has type '%s' but expected '%s'.", ...
            n, class(x), packetInfo.cellDType{n});
    end
    
    %   cell must match the specified width
    if numel(unpackedData{n}) ~= packetInfo.cellWidth(n)
        error("dataPack:unmatchedWidth", ...
            "unpackedData{%d} has %d element(s) but expected %d.", ...
            n, numel(x), packetInfo.cellWidth(n));
    end
    
    %   pack data
    for m = 1:packetInfo.cellWidth(n)
        
        %   start index to packed data of next cell element
        nextInIdx = inIdx + packetInfo.cellDTypeSize(n);
        
        %   pack single cell element
        packedData(inIdx:nextInIdx-1) = typecast(unpackedData{n}(m), 'uint8');
        
        %   update start index
        inIdx = nextInIdx;
        
    end
    
end

end


%%  Input arguments validation functions

function mustBeValidPacketInfo(s)

requiredFields = {'byteSize','cellsNum','cellDType','cellDTypeSize','cellWidth'};
for k = 1:numel(requiredFields)
    if ~isfield(s, requiredFields{k})
        error("dataPack:invalidPacketInfo", ...
            "packetInfo missing required field '%s'.", requiredFields{k});
    end
end

end