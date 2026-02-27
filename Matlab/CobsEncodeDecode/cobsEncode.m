function dst = cobsEncode(src)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 18, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Validate input arguments

arguments 
    src  (1,:)  uint8
end


%%  Preallocate destination buffer

%   empty input still produces one byte code (0x01)
srcLen = numel(src);
if srcLen == 0 
    dst = 1;
    return
end

%   calculate number of code bytes needed
dstLen = srcLen + ceil(srcLen/254);

%   preallocate destination buffer
dst = zeros(1, dstLen, 'uint8');


%%  Main encoding loop

codeIdx = 1;        %   index where current code byte lives
writeIdx = 2;       %   index to next write position
code = uint8(1);    %   code value
for i = 1:srcLen
    b = src(i);

    if b == 0
        %   zero found: finish current block
        dst(codeIdx) = code;        %   write code byte
        codeIdx = writeIdx;         %   start new block
        writeIdx = writeIdx + 1;    %   reserve code byte
        code = uint8(1);            %   reset code value
    else
        %   non-zero byte: copy it and extend current block
        dst(writeIdx) = b;
        writeIdx = writeIdx + 1;
        code = code + 1;

        if code == 255
            %   block full (254 non-zero bytes max): finish current block
            dst(codeIdx) = code;
            codeIdx = writeIdx;
            writeIdx = writeIdx + 1;
            code = uint8(1);
        end
    end
end

%   write last block
dst(codeIdx) = code;

%   Trim to actual size
dst = dst(1:writeIdx-1);  

end