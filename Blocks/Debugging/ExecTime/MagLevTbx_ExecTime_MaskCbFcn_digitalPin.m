function MagLevTbx_ExecTime_MaskCbFcn_digitalPin()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% June 05, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   valid Teensy pins - escludes:
%   - micro-SD pins (42-47)
%   - memory expander pads (48-54)
validTeensyEdgePins = 0:41; 

%   current digitalPin value
digitalPin = str2num( get_param(gcb, 'digitalPin') );

%   check for pin validity
if ~ismember(digitalPin, validTeensyEdgePins)

    %   error message
    errorMessage = { ...
        ['The selected pin (' num2str(digitalPin) ') is not valid.'], ...
        '', ...
        'Please select a valid pin number for Teensy 4.1 (0 to 41).' };
    
    %   window title
    dialogTitle = 'Pin Selection Error';
    
    %   window options
    dialogOptions = struct('WindowStyle', 'modal', 'Interpreter', 'none');
    
    %   open dialog box
    errordlg(errorMessage, dialogTitle, dialogOptions);

    %   set default value for digitalPin
    set_param(gcb, 'digitalPin', '0');

end

