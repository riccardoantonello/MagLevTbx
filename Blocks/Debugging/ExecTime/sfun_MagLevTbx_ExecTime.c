/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * June 05, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#define S_FUNCTION_NAME  sfun_MagLevTbx_ExecTime
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NUM_INPUTS          0
#define NUM_OUTPUTS         1
#define NUM_PARAMS          2
#define NUM_CONT_STATES     0
#define NUM_DISC_STATES     0
#define NUM_DWORKS          1

#define SAMPLE_TIME_0       INHERITED_SAMPLE_TIME

/* Output Port  0 */
#define OUTPUT_0_WIDTH      1
#define OUTPUT_0_DTYPE      SS_UINT32
#define OUTPUT_0_COMPLEX	COMPLEX_NO

/*  DWork 0 (start + elapsed time for execution of step routine)  */
#define DWORK_0_WIDTH       2
#define DWORK_0_DTYPE       SS_UINT32       
#define DWORK_0_COMPLEX     COMPLEX_NO    

    
/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlCheckParameters ======================================== */
#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
static void mdlCheckParameters(SimStruct *S)
{
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  digital pin enable  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  digital pin number  */
    
    /* ----------------------------------------------------------------- * 
     *  1st parameter check (digital pin enable) 
     * ----------------------------------------------------------------- */
    
    if(  mxIsEmpty(pa0) || 
        !mxIsLogicalScalar(pa0) )
    {
        ssSetErrorStatus(S, "1st parameter must be a valid digital pin enable flag (true/false).");
        return;
    }  

    /* ----------------------------------------------------------------- * 
     *  1st parameter check (digital pin number) 
     * ----------------------------------------------------------------- */
    
    if( mxIsEmpty(pa1) ||
            !mxIsScalar(pa1) ||
            !mxIsNumeric(pa1) ||
            mxIsComplex(pa1) )
    {
        ssSetErrorStatus(S, "2nd parameter must be a valid digital pin number.");
        return;        
    }    
}
#endif


/* Function: mdlInitializeSizes ======================================== */
static void mdlInitializeSizes(SimStruct *S)
{
    int_T n;
    
    /* check for expected number of parameters */
    ssSetNumSFcnParams(S, NUM_PARAMS); 
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) 
    {
        /* Return if number of expected != number of actual parameters */
        ssSetErrorStatus(S, "Invalid number of parameters.");
        return;
    }
    
    /* check parameters */
    mdlCheckParameters(S);
    
    /* set parameters as non-tunable */
    for(n = 0; n < NUM_PARAMS; n++)
        ssSetSFcnParamTunable(S, n, 0);
    
    /* set number of input ports */
    if (!ssSetNumInputPorts(S, NUM_INPUTS)) 
        return;
    
    /* set number of output ports */
    if (!ssSetNumOutputPorts(S, NUM_OUTPUTS))
        return;
    
    /* set output ports properties */
    ssSetOutputPortWidth(S, 0, OUTPUT_0_WIDTH);
    ssSetOutputPortDataType(S, 0, OUTPUT_0_DTYPE);
    ssSetOutputPortComplexSignal(S, 0, OUTPUT_0_COMPLEX);
    
    /* set number of states */
    ssSetNumContStates(S, NUM_CONT_STATES);
    ssSetNumDiscStates(S, NUM_DISC_STATES);

    /* set elementary work vectors */
    ssSetNumSampleTimes(S, 1);
    ssSetNumDWork(S, NUM_DWORKS);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
    /*  set DWork properties  */
    ssSetDWorkWidth(S, 0, DWORK_0_WIDTH);
    ssSetDWorkDataType(S, 0, DWORK_0_DTYPE);
    ssSetDWorkUsageType(S, 0, SS_DWORK_USED_AS_DWORK);

    /* Specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, 0);
}


/* Function: mdlInitializeSampleTimes ================================== */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, SAMPLE_TIME_0);
    ssSetOffsetTime(S, 0, 0.0);
}



#undef MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ================================= */
  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart ================================================ */
  static void mdlStart(SimStruct *S)
  {
  }
#endif /*  MDL_START */



/* Function: mdlOutputs ================================================ */
static void mdlOutputs(SimStruct *S, int_T tid)
{
}



#undef MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate =============================================== */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
  }
#endif /* MDL_UPDATE */



#undef MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives ========================================== */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate ============================================== */
static void mdlTerminate(SimStruct *S)
{
}



#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ==================================================== */
static void mdlRTW(SimStruct *S)
{   
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  digital pin enable  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  digital pin number  */
    
    /*  extract data  */
    boolean_T digitalPinEnable = mxIsLogicalScalarTrue(pa0); 
    mxUint8 digitalPin = (mxUint8) mxGetScalar(pa1);                        
    
    /* write ParamSettings to rtw file */
    if(!ssWriteRTWParamSettings(S, 
            2,  /*  number of parameters to write on model.rtw  */
            SSWRITE_VALUE_DTYPE_NUM, "digitalPinEnable", (void *)&digitalPinEnable, DTINFO(SS_BOOLEAN, 0),
            SSWRITE_VALUE_DTYPE_NUM, "digitalPin", (void *)&digitalPin, DTINFO(SS_UINT8, 0) ) ) 
    {
        ssSetErrorStatus(S, "Unable to write block dialog parameters to model.rtw file.");
        return; /* An error occurred. */
    }     
}
#endif /* mdlRTW */


/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
