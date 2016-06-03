/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/initMeth.c,v $
 *
 * Copyright (c) 2001 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: initMeth.c,v 1.5.2.2 2007/06/11 11:49:31 mawi Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: initMeth.c,v 1.5.2.2 2007/06/11 11:49:31 mawi Exp $ (C) 2001 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History: 
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{


  DB_MSG(( "Entering singlepulse:initMeth()" ));

  int dimRange[2] = { 1,2 };
  int lowMat[3]   = { 32, 32, 8 };
  int upMat[3]    = { 32, 32, 8 };

  /* which version of toolboxes should be active */
  PTB_VersionRequirement( Yes,20070101,"");
  
  /*
   * initialize NOE and Decoupling parameter group
   */
  
  STB_NoeOnOffRange();
  STB_DecOnOffRange();
  
  /*
   * init decoupling module
   */
  
  STB_InitDecModule();
  
  /*
   * init noe module
   */
  
  STB_InitNoeModule();
  
  /*
   * init the local adjustments available
   */
  
  
  /*
   * initialize the nuclei dependent on NOE or Decoupling
   */
  
  
  LocalDecNoeHandling();
  
  
  /*  Initialize PVM_NAverages PVM_NRepetitions see code in parsRelations.c */
  Local_NAveragesRange();
  NrepRange();


  /* Initialisation of rf pulse parameters */

  /* 1: flip angle in the scan edidor */
  if(ParxRelsParHasValue("PVM_ExcPulseAngle") == No)
      PVM_ExcPulseAngle = 30.0;
  ParxRelsShowInEditor("PVM_ExcPulseAngle");

  if(ParxRelsParHasValue("FrequencyLock_OnOff") == No)
    FrequencyLock_OnOff = Off;


  /* 2: pulses declared in parDefinitions.h 
     in this case - ExcPulse. We initalise it to default name, 
     1ms, and the flip angle given in PVM_ExcPulseAngle*/
  if(ParxRelsParHasValue("ExcPulse") == No)
  {
     STB_InitRFPulse(&ExcPulse,
		     "gauss.exc",
		     1,
		     PVM_ExcPulseAngle);
  }
  ExcPulseRange();
  
  /* 3: the corresponding pulse enums */
  STB_InitExcPulseEnum("ExcPulseEnum");


/*KW-sliceSPEC (begin) 100714*/
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 1.0;

  if(ParxRelsParHasValue("SliceSpoilerDuration") == No)
    SliceSpoilerDuration = 2;
  if(ParxRelsParHasValue("SliceSpoilerStrength") == No)
    SliceSpoilerStrength = 20;

  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  /* A: in-plane */

  STB_InitStandardInplaneGeoPars(2,dimRange,lowMat,upMat,No);

  /* B: slice geometry */

  STB_InitSliceGeoPars(0,0,0);

  STB_InitExSliceAtoms();
/*KW-sliceSPEC (end) 100714*/

  /* initialisation of spectroscopy */


  ParxRelsHideInEditor("PVM_SpecOffsetHz,PVM_SpecOffsetppm");

  STB_InitSpectroscopy( 1, 1, 1 ,  PVM_Nucleus1 , 200.0, 1000000 );


  /* Initialize multi receiver parameters */
  
  STB_InitEncoding();

 
  /* Initialisation of modules */
  STB_InitWsModule (1);
  STB_InitSatSlicesModule();
  STB_InitFatSupModule();
  NdummyRange();

  /*kw-mod 101014 begin*/
  STB_InitTriggerModule();
  STB_InitTriggerOutModule();
  /*kw-mod 101014 end*/

  /*Gba - shim (start)*/
  if(ParxRelsParHasValue("Slice1ShimStrength_x") == No)
    Slice1ShimStrength_x = 1.0;
  if(ParxRelsParHasValue("Slice1ShimStrength_y") == No)
    Slice1ShimStrength_y = 1.0;
  if(ParxRelsParHasValue("Slice1ShimStrength_z") == No)
    Slice1ShimStrength_z = 1.0;
  if(ParxRelsParHasValue("Slice2ShimStrength_x") == No)
    Slice2ShimStrength_x = 1.0;
  if(ParxRelsParHasValue("Slice2ShimStrength_y") == No)
    Slice2ShimStrength_y = 1.0;
  if(ParxRelsParHasValue("Slice2ShimStrength_z") == No)
    Slice2ShimStrength_z = 1.0;
  /*Gba - shim (end)*/
  
  /* initialisation of DeadTime */
  DeadTimeRange();

  /* setting for Pipeline filter in case of multi channel acquisition */
  PVM_RefScanPCYN = No;
 
 
  /* Once all parameters have initial values, the backbone is called
     to assure they are consistent */
  backbone();
 

  DB_MSG(( "Exiting singlepulse:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









