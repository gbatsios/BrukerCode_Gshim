/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/parsLayout.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsLayout.h,v 1.4.2.1 2007/03/22 16:41:59 sako Exp $
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

parclass
{
  DeadTime;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;


parclass
{
  FrequencyLock_OnOff;
  NavFlipAngle;
} Optimize;

parclass
{
  ExcPulseEnum;
  ExcPulse;
}
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

parclass
{ 
  Ndummy;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_FovSatOnOff;
  Sat_Slices_Parameters;
  Suppression;
  PVM_DecOnOff;
  Decoupling_Parameters;
  PVM_NoeOnOff;
  NOE_Parameters;
  /*kw-mod 101014 begin*/
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_TriggerOutOnOff;
  TriggerOut_Parameters;
  /*kw-mod 101014 end*/
} Preparation;

/*Gba - shim (start)*/
parclass
{
   Slice1ShimStrength_x;
   Slice1ShimStrength_y;
   Slice1ShimStrength_z;
   GradShimSlice1_r;
   GradShimSlice1_p;
   GradShimSlice1_s;
   Slice2ShimStrength_x;
   Slice2ShimStrength_y;
   Slice2ShimStrength_z;
   GradShimSlice2_r;
   GradShimSlice2_p;
   GradShimSlice2_s;
}
attributes
{
  display_name "Shim Values";
} ShimValues;

/*Gba - shim (end)*/

parclass
{
  Method;
  PVM_GeoMode;
  PVM_EchoTime;			/*KW-sliceSPECT*/
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  SliceSpoilerDuration;		/*KW-sliceSPECT*/
  SliceSpoilerStrength;		/*KW-sliceSPECT*/
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  StandardSliceGeometry;	/*KW-sliceSPECT*/
  Spectroscopy;
  Preparation;
  Optimize;
  Encoding;
  Sequence_Details;
  ShimValues;			/* gba-shim*/
} MethodClass;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



