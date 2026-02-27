function makeInfo = rtwmakecfg()

%%
% Riccardo Antonello (riccardo.antonello@unipd.it)
% 
% February 16, 2026
% 
% Dept. of Information Engineering, University of Padova 

%%

%   set makeInfo struct
makeInfo.includePath    =   { };
makeInfo.sourcePath     =   { };
makeInfo.sources        =   { 'sfun_MagLevTbx_CurrDrv_WrappedFcns.cpp' };

makeInfo.linkLibsObjs   =   {};
makeInfo.precompile     =   0;
makeInfo.library        =   {};

