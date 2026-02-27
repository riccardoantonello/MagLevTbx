%% 
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 19, 2026
%
% Dept. of Information Engineering, University of Padova
%

%%  Create serial object

%   create serialport object
fprintf("Connecting ... ");
sp = serialport("/dev/cu.usbmodem163412001", 115200);
fprintf("Done.\n");

%   pause before continuing (wait serial port to set)
pause(1.0);


%%  Main loop - send & receive sequence of packets

%   payload
dataToSend = {uint8(0)};

%   number of packets to send
N = 20;

%   packet options
nullTerminated = true;
cobsEncoded = true;

%   main iteration
for n = 1:N

    %   send packet
    serialPacketSend(sp, dataToSend, cobsEncoded, nullTerminated);
    
    %   receive reply
    dataReceived = serialPacketReceive(sp, {'uint8'});
    
    %   print received packet 
    fprintf('%d) %d\n', n, dataReceived{1});
    
    %   update data to send
    dataToSend = dataReceived;
    
    pause(0.1);
    
end


%%  Remove serial object

clear sp;


