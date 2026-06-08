function MagLevTbx_ExecTime_MaskCbFcn_digitalPinEnable()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% June 05, 2026
%
% Dept. of Information Engineering, University of Padova
%

% Set the 'MaskVisibilities' property for each parameter in mask
digitalPinEnable = get_param(gcb, 'digitalPinEnable');
if strcmp(digitalPinEnable, 'on')
    set_param( gcb,'MaskVisibilities', ...
        {'on', 'on'} );
else
    set_param( gcb,'MaskVisibilities', ...
        {'on', 'off'} );    
end