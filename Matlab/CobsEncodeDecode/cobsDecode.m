function dst = cobsDecode(src)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 19, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Validate input arguments

arguments
    src (1,:) uint8
end

% A valid COBS stream is at least one byte (the final code).
srcLen = numel(src);
if srcLen == 0
    error("cobsDecode:EmptyInput", "COBS input must be non-empty.");
end


%%  Preallocate destination buffer

% Upper bound: decoded length is <= srcLen-1 + (number of blocks - 1),
% but srcLen is a safe simple upper bound (actual length will be trimmed 
% to the actual size at the end)
dst = zeros(1, srcLen, "uint8");


%%  Main decoding loop

readIdx  = 1;
writeIdx = 1;

while readIdx <= srcLen

    %   read code value (i.e. number of bytes until next zero + 1)
    code = src(readIdx);
    if code == 0
        error("cobsDecode:InvalidCode", "Zero code byte encountered at index %d.", readIdx);
    end
    readIdx = readIdx + 1;

    %   bytes to copy
    numBytesToCopy = double(code) - 1;

    %   ensure encoded data contains (code-1) bytes to copy
    if readIdx + numBytesToCopy - 1 > srcLen
        error("cobsDecode:InvalidLength", "Not enough bytes to copy for code at index %d.", readIdx);
    end
    
    %   copy the next (code-1) bytes
    if numBytesToCopy > 0
        dst(writeIdx:writeIdx+numBytesToCopy-1) = src(readIdx:readIdx+numBytesToCopy-1);
        writeIdx = writeIdx + numBytesToCopy;
        readIdx  = readIdx  + numBytesToCopy;
    end

    %   reinsert a zero between blocks when code < 255 and we are not at the end
    if code < 255 && readIdx <= srcLen
        dst(writeIdx) = 0;
        writeIdx = writeIdx + 1;
    end

end

% Trim to actual size
dst = dst(1:writeIdx-1);

end