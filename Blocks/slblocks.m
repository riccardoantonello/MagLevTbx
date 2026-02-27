function blkStruct = slblocks
% This function specifies that the library should appear in the Library
% Browser, and be cached in the browser repository.

%   get toolbox name
info = MagLevTbx_Info();

%   library filename
Browser.Library = 'MagLevTbx_BaseLib';    

%   library name in Simulink Library Browser
Browser.Name = info.name;

blkStruct.Browser = Browser; 