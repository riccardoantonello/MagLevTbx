function unpackedData = dataUnpack(packedData, packetInfo)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Validate input arguments

arguments
    packedData (1,:) uint8
    packetInfo (1,1) struct {mustBeValidPacketInfo}
end


%%  Data unpack

%   init unpacked data cell array
unpackedDataLen = packetInfo.cellsNum;
unpackedData = cell(1, unpackedDataLen);

%   init packed data length 
packedDataLen = length(packedData);

%   packedData length must be equal to the byte size specified in packetInfo
if packedDataLen ~= packetInfo.byteSize
    error("dataUnpack:invalidPackedDataSize", ...
        "The length of packed data array must be equal to the byte size specified in packetInfo.");    
end

%   start index to packed data of current cell
inIdx = 1;

%   unpack data
for n = 1:unpackedDataLen

    %   start index to packed data of next cell    
    nextInIdx = inIdx + packetInfo.cellDTypeSize(n) * packetInfo.cellWidth(n);
    
    %   unpack data
    inData = packedData( inIdx:(nextInIdx-1) );
    outData = typecast( uint8(inData), packetInfo.cellDType{n});
    
    %   update start index
    inIdx = nextInIdx;

    %   store unpacked data to output cell array
    unpackedData{n} = outData;
    
end

end


%%  Input arguments validation functions

function mustBeValidPacketInfo(s)

requiredFields = {'byteSize','cellsNum','cellDType','cellDTypeSize','cellWidth'};
for k = 1:numel(requiredFields)
    if ~isfield(s, requiredFields{k})
        error("dataUnpack:invalidPacketInfo", ...
            "packetInfo missing required field '%s'.", requiredFields{k});
    end
end

end