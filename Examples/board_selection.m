%%  Board selection
%
%   Sets:
%     magLevId        MagLev unit id (selects the calibration data files)
%     serialPortName  serial port of the Teensy


%   MagLev id number (used in calibration data filenames)
magLevId = 12;

%   serial port of the Teensy
%   Leave it as "" to auto-detect the port
%   If multiple teensies are connected or auto-detection doesn't work,
%   it can be set explicitly, e.g. "/dev/cu.usbmodem146674101"
%   or "COM3"
serialPortName = "";


%%  Auto-detect serial port

if strlength(serialPortName) == 0

    %   Teensy USB serial ports: /dev/cu.usbmodem* (macOS),
    %   /dev/ttyACM* (Linux), COM* (Windows)
    boardSelPorts = serialportlist();
    boardSelPorts = boardSelPorts( ...
        startsWith(boardSelPorts, ["/dev/cu.usbmodem", "/dev/ttyACM", "COM"]));

    if isscalar(boardSelPorts)
        serialPortName = boardSelPorts;
        fprintf("Board selection: MagLev%02d on %s (auto-detected)\n", ...
            magLevId, serialPortName);
    elseif isempty(boardSelPorts)
        fprintf("Board selection: MagLev%02d, no serial port found\n", ...
            magLevId);
    else
        fprintf(['Board selection: MagLev%02d, no serial port selected: ' ...
            'found %s. Set serialPortName in board_selection.m.\n'], ...
            magLevId, strjoin(boardSelPorts, ", "));
    end

    clear boardSelPorts;

else
    fprintf("Board selection: MagLev%02d on %s\n", magLevId, serialPortName);
end
