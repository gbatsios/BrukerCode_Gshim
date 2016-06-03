/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/parsRelations.c,v $
 *
 * Copyright (c) 2002 -2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsRelations.c,v 1.4.2.7 2008/04/25 15:05:40 sako Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: parsRelations.c,v 1.4.2.7 2008/04/25 15:05:40 sako Exp $ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0


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


/* ------------------------------------------------------------ 
  backbone 
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{
  YesNo refAttIsAviable=No;
  double minFov[3] = {1e-3, 1e-3, 1e-3},minThickness,    /*KW-sliceSPECT*/ 
	 referenceAttenuation=0;
  int seg_size=1;          
  
  
  DB_MSG(("Entering einpuls:backbone"));

  /*
   *  control appearance in GeoEditor: method doesn't support any geometric operation
   */

  GeoModeRange();

  /* Nucleus and  PVM_GradCalConst
     are handled by this funtion: */
  STB_UpdateNuclei(Yes);
 
  DB_MSG(("nucleus ok"));

  /* handle RF pulse */   

  if(PVM_DeriveGains == Yes)
    refAttIsAviable =
      STB_GetRefAtt(1,PVM_Nucleus1,&referenceAttenuation);
  else
    refAttIsAviable = No;

  STB_UpdateRFPulse("ExcPulse",
		    &ExcPulse,
		    refAttIsAviable,
		    referenceAttenuation);

  STB_UpdateExcPulseEnum("ExcPulseEnum",
			 &ExcPulseEnum,
			 ExcPulse.Filename,
			 ExcPulse.Classification);

  PVM_ExcPulseAngle = ExcPulse.FlipAngle;
  

  /* excitation pulse */
/*KW-sliceSPECT (begin)*/
  PVM_ExSlicePulseLength   = ExcPulse.Length;
  PVM_ExSliceBandWidth     = ExcPulse.Bandwidth;
  PVM_ExSliceRephaseFactor = ExcPulse.RephaseFactor * 
                             ExcPulse.TrimRephase / 100.0;
/*KW-sliceSPECT (end)*/
  PVM_NEchoImages = 1;

/*KW-sliceSPECT (begin)*/

  /* begin Update Geometry: */
  
  /* 
   * 1: in-plane geometry:
   * The STB_StandardInplaneGeoParHandler is called twice:
   * first, with a dummy value of minFov, to make size constraints;
   * then, after the true minFov is found, to do the rest.
   * (because the sizes must be set before we find minFov)
   */
      
  STB_StandardInplaneGeoParHandler(minFov,2.0);

  LocalGeometryMinimaRels(&minThickness);

  STB_StandardInplaneGeoParHandler(minFov,2.0);

  STB_UpdateSliceGeoPars(0,0,0,minThickness);	/*gba 040311*/

  LocalGradientStrengthRels();

  echoTimeRels();

  LocalFrequencyOffsetRels();	    /*kw-mod-100721*/

  /*spoiler*/
  SliceSpoilerStrength = MIN_OF(SliceSpoilerStrength, 100.0);
  SliceSpoilerStrength = MAX_OF(SliceSpoilerStrength, -100.0);
  SliceSpoilerDuration = MAX_OF(SliceSpoilerDuration, 2*PVM_RiseTime);
/*KW-sliceSPECT (end)*/

  /* ------------- spectroscopy part ----------------------- */
  STB_UpdateSpectroscopy( PVM_Nucleus1 );
  DB_MSG(("spectro ok"));

  /* update Encoding parclass for multi receiver experiment */

  PARX_change_dims("PVM_AntiAlias", 1);
  PVM_AntiAlias[0] = 1.0;

  STB_UpdateEncoding(1,
		     PVM_SpecMatrix,
		     PVM_AntiAlias,
		     &seg_size,
		     SEG_SEQUENTIAL,
		     No,
		     No,
		     No);


  /* handle the Retro Frequency lock */

  if ( FrequencyLock_OnOff == On ) 
  {
    ParxRelsShowInEditor("NavFlipAngle");
    NavFlipAngle = MAX_OF(2.0,NavFlipAngle);
    NavFlipAngle = MIN_OF(90.0,NavFlipAngle);
    NavAttenuation  = ExcPulse.Attenuation
      + 20.0*log10( ExcPulse.FlipAngle / NavFlipAngle );
  }
  else 
  {
    ParxRelsHideInEditor("NavFlipAngle");
    NavAttenuation  = 150.0;
  }

  
  
  /* --------------handling decoupling modules ------------- */

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1);
  LocalDecNoeHandling();
  STB_UpdateSatSlicesModule(PVM_Nucleus1);
  STB_UpdateWsModule( PVM_Nucleus1 ,PVM_FovSatModuleTime,refAttIsAviable,referenceAttenuation,1);

  /*kw-mod 101014 begin*/
  STB_UpdateTriggerModule();
  STB_UpdateTriggerOutModule();
  /*kw-mod 101014 end*/

  /* DeadTime */
  updateDeadTime();

  /* repetition time */
  repetitionTimeRels();
  DB_MSG(("TR ok"));
  
  /* set GS parameters */
  SetGSparameters();

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();
  DB_MSG(("baselev ok"));

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("Exiting einpuls:backbone"));
}

/* ------------------------------------------------------------
   relations of DeadTime
   -------------------------------------------------------------*/
void DeadTimeRels(void)
{
  DeadTimeRange();
  backbone();
}

void DeadTimeRange(void)
{
  if(ParxRelsParHasValue("DeadTime") == No)
    DeadTime = 0.05;
  DeadTime = MAX_OF(0.001, DeadTime);
  DeadTime = MIN_OF(5.0, DeadTime);
}

void updateDeadTime(void)
{
  double min_us;
  
  /* The minimum delay between RF pulse and ACQ_START is given by the
   * base-level parameter DE. This parameter was set during the update of
   * spectorscopy. */

  min_us = DE; 

  DeadTime =  MAX_OF(DeadTime, min_us*1e-3);
}

/*--------------------------------------------------------------
  ExcPulseAngleRelation
  Redirected relation of PVM_ExcPulseAngle
  -------------------------------------------------------------*/
void ExcPulseAngleRelation(void)
{
  DB_MSG(("-->ExcPulseAngleRelation"));
  ExcPulse.FlipAngle = PVM_ExcPulseAngle;
  ExcPulseRange();
  backbone();
  DB_MSG(("<--ExcPulseAngleRelation"));
}

/*===========================================================
 *
 *  examples for relations concearning special pulses and 
 *  pulselists
 *
 *==========================================================*/



/* --------------------------------------------------------------
   ExcPulseEnumRelation
   Relation of ExcPulseEnum (a dynamic enmueration parameter which
   allows to select one of the existing library exc. pulses)
   Sets the name and the clasification  of the pulse perameter ExcPulse 
   according to the selected enum value.
   --------------------------------------------------------------*/
void ExcPulseEnumRelation(void)
{
  YesNo status;
  DB_MSG(("-->ExcPulsesEnumRelation"));
  
  /* set the name and clasification of ExcPulse: */
  status = STB_UpdateExcPulseName("ExcPulseEnum",
				  &ExcPulseEnum,
				  ExcPulse.Filename,
				  &ExcPulse.Classification);

  /* call the method relations */
  backbone();

  DB_MSG(("<--ExcPulseEnumRelation status = %s",
	  status == Yes? "Yes":"No"));
}



/* -----------------------------------------------------------
   Relation of ExcPulse
 
   All pulses of type PVM_RF_PULSE_TYPE must have relations like this.
   However, if you clone this funtion for a different pulse parameter
   remember to replace the param name in the call to UT_SetRequest!

   IMPORTANT: this function should not be invoked in the backbone!
   -----------------------------------------------------------*/
void ExcPulseRelation(void)
{
  DB_MSG(("-->ExcPulseRelation"));

  /* Tell the request handling system that the parameter
     ExcPulse has been edited */
  UT_SetRequest("ExcPulse");

  /* Check the values of ExcPulse */
  ExcPulseRange();

  /* call the backbone; further handling will take place there
     (by means of STB_UpdateRFPulse)  */
 
  backbone();

  DB_MSG(("-->ExcPulseRelation"));
}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/



void ExcPulseRange(void)
{
  DB_MSG(("-->ExcPulseRange"));
  
  /* allowed clasification */

  switch(ExcPulse.Classification)
  {
  default:
    ExcPulse.Classification = LIB_EXCITATION;
    break;
  case LIB_EXCITATION:
  case PVM_EXCITATION:
  case USER_PULSE:
    break;
  }

  /* allowed angle for this pulse */
 
  ExcPulse.FlipAngle = MIN_OF(90.0,ExcPulse.FlipAngle);


  /* general verifiation of all pulse atributes  */

  STB_CheckRFPulse(&ExcPulse);

  DB_MSG(("<--ExcPulseRange"));

}


void repetitionTimeRels( void )
{
  int i,dim,nSlices;
  double trigger, trigOutSlice, trigOutVol; /*kw_mod 101014*/
  double TotalTime,amplifierenable;

  DB_MSG(("--> minRepetitionTimeRels"));

  TotalTime = 0.0;
  trigger = STB_UpdateTriggerModule(); /*kw-mod 101014*/
  amplifierenable = CFG_AmplifierEnable();
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );  /*gba 040311*/
  /*kw-mod 101014 begin*/
  if(PVM_TriggerOutOnOff == On)
  {
    switch(PVM_TriggerOutMode)
    {
    case PER_SLICE:   
      trigOutSlice = PVM_TriggerOutModuleTime;
      trigOutVol = 0.0;
      break;

    case PER_VOLUME: 
      trigOutSlice = 0.0;
      trigOutVol = PVM_TriggerOutModuleTime;
      break;

    case AT_START:
    default:
      trigOutSlice = 0.0;
      trigOutVol = 0.0;
      
    }
  }
  else
    trigOutSlice = trigOutVol = 0.0;
  /*kw-mod 101014 end*/

  if(FrequencyLock_OnOff == On)
  {
    PVM_MinRepetitionTime =
	nSlices * (						/* gba 040311 */
      2*amplifierenable         +  /* time before RF-Pulse */
	  trigger					+ /*kw-mod 101014*/
      trigOutSlice				+ /*kw-mod 101014*/
	  trigOutVol				+ /*kw-mod 101014*/
      PVM_FatSupModuleTime      +
      PVM_NoeModuleTime         +
      PVM_WsModuleDuration      +
      2*ExcPulse.Length         +
      DeadTime                  +  
      2*PVM_SpecAcquisitionTime +
      2*PVM_DigEndDelOpt        +
      2*1.0                     + /* delay after ADC_END */
      5.0                       + /* min d0 */
      0.090);
  }
  else
  {
    PVM_MinRepetitionTime =
	nSlices * (						/* gba 040311 */
      amplifierenable          +  /* time before RF-Pulse */
      PVM_FatSupModuleTime     +
      PVM_NoeModuleTime        +
      PVM_WsModuleDuration     +
	  trigger					+ /*kw-mod 101014*/
      trigOutSlice				+ /*kw-mod 101014*/
	  trigOutVol				+ /*kw-mod 101014*/
      SliceSpoilerDuration     + /*KW-sliceSPECT*/
      /*ExcPulse.Length          +*/ /*KW-scliceSPECT*/
      PVM_ExSlicePulseLength / 2.0 + /*KW-scliceSPECT*/
      PVM_EchoTime                 + /*KW-sliceSPECT*/
      DeadTime                 +  
      PVM_SpecAcquisitionTime  +
      PVM_DigEndDelOpt         + 
      1.0                      + /* delay after ADC_END */
      5.0                      + /* min d0 */
      0.065);
  }

  PVM_RepetitionTime = ( PVM_RepetitionTime < PVM_MinRepetitionTime ? 
			 PVM_MinRepetitionTime : PVM_RepetitionTime );
  
  /** Calculate Total Scan Time and Set for Scan Editor **/ 

  dim = PTB_GetSpecDim();
  TotalTime = PVM_RepetitionTime*PVM_NAverages;
  /*for(i=1; i<dim; i++)
     TotalTime *= PVM_SpecMatrix[i];*/

  TotalTime *= PVM_NRepetitions;

  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
 

  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels"));
}



void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("Entering Local_NAveragesRange"));
  
  /* 
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
    {
      PVM_NAverages = 1;
    }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);
  
  DB_MSG(("Exiting Local_NAveragesRange"));

}


void Local_NAveragesHandler(void)
{

  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));

  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */

  
  backbone();


  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));
  return;
}


void NrepRange(void)
{
  if(ParxRelsParHasValue("PVM_NRepetitions") ==No)
  {
    PVM_NRepetitions = 1;
  }    
  else
  {
     PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  }

}

void NrepRel(void)
{
  NrepRange();
  backbone();
}

void SpecHandler(void)
{
  DB_MSG(("-->SpecHandler\n"));

  backbone();

  DB_MSG(("<--SpecHandler\n"));
}

/*
 * set parameters of the GS class 
 */
void SetGSparameters(void)
{
  GS_info_normalized_area = Of_raw_data;
}



void GeoModeRange(void)
{
  
  PVM_GeoMode= GeoMRS;
  ParxRelsHideInEditor("PVM_GeoMode");
  ParxRelsShowInFile("PVM_GeoMode");

}

void NdummyRange(void)
{
  if(!ParxRelsParHasValue("Ndummy"))
  {
    Ndummy = 0;
  }
  else
  {
    Ndummy = MAX_OF(0,Ndummy);
  }
}


void NdummyRel(void)
{
  NdummyRange();
  backbone();

}

void LocalDecNoeHandling(void)
{
 if((PVM_DecOnOff == On ) || (PVM_NoeOnOff == On))
 {
   if(PVM_NumberOfNuclei!=2)
   {
     STB_InitNuclei(2);
     if(PVM_NumberOfNuclei < 2)
     {
       /* system configuration does not support 2nd RF channel */
       PVM_DecOnOff=PVM_NoeOnOff=Off;
     }
   }
 }
 else if( (PVM_DecOnOff ==Off ) && (PVM_NoeOnOff == Off))
 {
   if(PVM_NumberOfNuclei!=1)
   {
     STB_InitNuclei(1);
   }
 }

 STB_UpdateDecModule(PVM_Nucleus2,PVM_SpecAcquisitionTime);
 STB_UpdateNoeModule(PVM_Nucleus2);

 if( (PVM_DecOnOff ==On ) && (PVM_NoeOnOff == On))
 {
   ParxRelsHideInEditor("PVM_NoeReferenceppm,PVM_NoeOffsetppm,PVM_NoeReferenceMHz,PVM_NoeFreqMHz");
   PVM_NoeReferenceppm = PVM_DecReferenceppm;
   PVM_NoeOffsetppm = PVM_DecOffsetppm;
   if(PVM_DecMode == Composite_Pulse && PVM_NoeMode == Composite_Pulse_Noe)
   {
     /* both modules use F2 channel and share therefor the CPD pulse 
        element duration PCPD[1] so the element durations have to be matched */

     if(PVM_NoePulseElementDuration != PVM_DecPulseElementDuration)
     {
       PVM_NoePulseElementDuration = PVM_DecPulseElementDuration;
       STB_UpdateNoeModule(PVM_Nucleus2);
     }
   }
 }
 else
 {
   ParxRelsShowInEditor("PVM_NoeReferenceppm,PVM_NoeOffsetppm,PVM_NoeReferenceMHz,PVM_NoeFreqMHz");
 }
}


/* Relations of PVM_AutoRgInitHandler (see callbackDefs.h)
 * This function is called when the RG adjustment starts. It modifies
 * the state of the method just for the adjustment. Afterwards, the
 * original state is re-established. We use it to turn off the pipeline filter
 * during the RG adjustment.
 */
void MyRgInitRel(void)
{

  DB_MSG(("-->MyRgInitRel"));

  if(PVM_EncUseMultiRec == Yes)
  {
    int nrec,availrec,i;
    nrec = (int)PARX_get_dim("ACQ_ReceiverSelect",1);
    availrec = (int)PARX_get_dim("PVM_EncActReceivers",1);
    for(i=0;i<availrec;i++)
      ACQ_ReceiverSelect[i] = PVM_EncActReceivers[i]==On ? Yes:No;
    for(i=availrec;i<nrec;i++)
      ACQ_ReceiverSelect[i]=No;
  }

  if((FrequencyLock_OnOff == On) || (PVM_EncUseMultiRec == Yes))
  {
    ACQ_user_filter = No;
    ParxRelsParRelations("ACQ_user_filter",Yes);
    ACQ_user_filter_size[0] = ACQ_user_filter_size[1] = ACQ_user_filter_size[2]= 0;
    ParxRelsParRelations("ACQ_user_filter_size",Yes);
  }

  if(FrequencyLock_OnOff == On)
  {
    NI=2;
    ParxRelsParRelations("NI",Yes);
  } 

  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);
 
  DB_MSG(("<--MyRgInitRel"));
}

/*KW-sliceSPECT (begin)*/
void LocalGeometryMinimaRels(double *min_thickness )
{
    double sliceRampInteg; /* normalised integral falling slice gradient ramp */
  double sliceRephInteg; /* normalised integral slice rephase gradient      */
    /* SLICE SELECTION GRADIENT
       *
       * Calculate the normalised integral of the descending gradient 
       * ramp after the RF pulse
       */

      sliceRampInteg = MRT_NormGradRampTime( PVM_ExSliceRampDownTime,
					     PVM_ExSliceRampDownIntegral );
      /*
       * Calculate the normalised integral of the slice selection rephasing
       * gradient
       */

      sliceRephInteg = 
	MRT_NormGradPulseTime( PVM_ExSliceRephaseTime,
			       PVM_ExSliceRephaseRampUpTime,
			       PVM_ExSliceRephaseRampUpIntegral,
			       PVM_ExSliceRephaseRampDownTime,
			       PVM_ExSliceRephaseRampDownIntegral );
      /*
       * Calculate the ratio of the strength of the slice selection 
       * gradient to the strength of the slice selection rephase 
       * gradient
       *
       * The variable SliceGradRatio is a parameter defined in the file:
       * parsDefinition.h but it is NOT included in the definition of 
       * MethodClass that appears in parsLayout.h. 
       * The value of SliceGradRatio determined here is used later in 
       * "LocalGradientStrengthRels()"
       */

      SliceGradRatio = 
	MRT_SliceGradRatio( PVM_ExSlicePulseLength,
			    PVM_ExSliceRephaseFactor,
			    PVM_ExSliceRampDownWaitTime,
			    sliceRampInteg,
			    sliceRephInteg );
      /*
       * Calculate the minimum slice thickness
       */
      
      *min_thickness = MRT_MinSliceThickness( PVM_ExSliceBandWidth,
					      SliceGradRatio,
					      PVM_LimExSliceGradient,
					      PVM_LimExSliceRephaseGradient,
					      PVM_GradCalConst );
}

void LocalGradientStrengthRels( void )
{
    /* SLICE SELECTION GRADIENT */

  PVM_ExSliceGradient = 
    MRT_SliceGrad( PVM_ExSliceBandWidth,
		   PVM_SliceThick,
		   PVM_GradCalConst );
  
  PVM_ExSliceRephaseGradient = 
    MRT_SliceRephaseGrad( SliceGradRatio,
			      PVM_ExSliceGradient );
  
  
  DB_MSG(("<--LocalGradientStrengthRels\n"));
}

void LocalFrequencyOffsetRels( void )
{
  int nslices;
    /*
   /* Calculate slice offset */
   
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);    /*gba 040311 */
  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  PVM_ExSliceGradient,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );
}

void echoTimeRels( void )
{
  DB_MSG(("-->echoTimeRels\n"));
  
  PVM_MinEchoTime = 
    PVM_ExSlicePulseLength / 2.0 +
    PVM_RampTime                 +
    PVM_InterGradientWaitTime    +
    PVM_ExSliceRephaseTime       +
    PVM_RampTime;

  
  PVM_EchoTime = PVM_EchoTime < PVM_MinEchoTime   ?
                 PVM_MinEchoTime : PVM_EchoTime;
  

  DB_MSG(("<--echoTimeRels\n"));
}
/*KW-sliceSPECT (end)*/



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/








