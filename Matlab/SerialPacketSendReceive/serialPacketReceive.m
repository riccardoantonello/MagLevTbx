function packetData = serialPacketReceive(sp, packetSpec, cobsEncoded, nullTerminated)

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 19, 2026
%
% Dept. of Information Engineering, University of Padova
%


%%  Validate input arguments

arguments
    sp  (1,1)   internal.Serialport
    packetSpec (1,:) cell   %   content will be validated by getPacketInfo
    cobsEncoded (1,1) logical = true
    nullTerminated (1,1) logical = true
end


%%  Receive packet

%   get packet info
packetInfo = getPacketInfo(packetSpec);

%   compute expected frame size
expectedLen = packetInfo.byteSize;

if cobsEncoded
    expectedLen = packetInfo.byteSize + ceil(packetInfo.byteSize/254);
end

if nullTerminated
    expectedLen = expectedLen + 1;
end

%   get one frame
if nullTerminated
    frame = readFrameNullTerminated(sp, expectedLen);
    frame = frame(1:end-1);
else
    frame = readFrameFixedLen(sp, expectedLen);
end

%   decode
if cobsEncoded
    frame = cobsDecode(frame);
end

%   unpack data
packetData = dataUnpack(frame, packetInfo);

end


%%  Helper functions

% ----------------------------------------------------------------------- %
function frame = readFrameNullTerminated(sp, expectedLen)

%   get payload of a single packet
frame = uint8( read(sp, expectedLen, 'uint8') );

%   read until a null-terminator is found
while frame(end) ~= 0
    frame(1:end-1) = frame(2:end);
    frame(end) = read(sp, 1, 'uint8');
end

end

% ----------------------------------------------------------------------- %
function frame = readFrameFixedLen(sp, expectedLen)

frame = read(sp, expectedLen, "uint8");

end

