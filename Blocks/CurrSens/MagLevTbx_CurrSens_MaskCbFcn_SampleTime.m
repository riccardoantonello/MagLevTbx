function MagLevTbx_CurrSens_MaskCbFcn_SampleTime()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
%
% February 17, 2026
%
% Dept. of Information Engineering, University of Padova
%

%   Define sample time parameter number
paramId = 2;  

%   Get sample time
data = Simulink.Mask.get(gcb);      
sampleTime = str2double(data.Parameters(paramId).Value);

%   Check for valid sample time
if ~isscalar(sampleTime) || ...
        ( ~isnan(sampleTime) && ...
        isfloat(sampleTime) && ...
        ~(sampleTime > 0 || sampleTime == -1) )
    
    %   Note: skip check if the value is a char array 
    %   (check on variable values is postponed)
    errordlg({['The sample time must be a positive quantity, ', ...
        'or -1 (for inherited sample time).']}, ...
        'Sample Time Error');

    data.Parameters(paramId).Value = '-1';

end