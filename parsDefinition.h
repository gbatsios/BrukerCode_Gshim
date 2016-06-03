/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/parsDefinition.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsDefinition.h,v 1.4 2006/11/07 13:09:59 sako Exp $
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/


double parameter out[3][3];
double parameter det;
double parameter out2[3][3];
double parameter det2;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulseRelation;
}ExcPulse;


double parameter
{
  display_name "Acquisition Delay";
  relations DeadTimeRels;
  format "%.3f";
  units "ms";
} DeadTime;

int parameter
{
  display_name "Number of Dummy Scans";
  relations NdummyRel;
}Ndummy;

OnOff parameter
{
    display_name "Frequency Lock";
    relations backbone;
} FrequencyLock_OnOff;

double parameter
{
    display_name "FlipAngle for Navigator";
    units "deg";
    format "%.2f";
    relations backbone;
} NavFlipAngle;

double parameter NavAttenuation;
double parameter SliceGradRatio; /*KW-sliceSPECT*/

/*KW-sliceSPECT (begin)*/
double parameter
{
  display_name "Slice Spoiler Duration";
  format "%.2f";
  units "ms";
  relations backbone;
} SliceSpoilerDuration;

double parameter
{
  display_name "Slice Spoiler Strength";
  format "%.1f";
  units "%";
  relations backbone;
} SliceSpoilerStrength;
/*KW-sliceSPECT (end)*/

/*Gba - shim (start)*/
double parameter
{
	display_name "x Shim values -  Slice 1";
	format "%.3f";
	units "%";
	relations backbone;
} Slice1ShimStrength_x;
double parameter
{
	display_name "y Shim values - Slice 1";
	format "%.3f";
	units "%";
	relations backbone;
} Slice1ShimStrength_y;
double parameter
{
	display_name "z Shim values - Slice 1";
	format "%.3f";
	units "%";
	relations backbone;
} Slice1ShimStrength_z;

double parameter
{
	display_name "x Shim values - Slice 2";
	format "%.3f";
	units "%";
	relations backbone;
} Slice2ShimStrength_x;
double parameter
{
	display_name "y Shim values - Slice 2";
	format "%.3f";
	units "%";
	relations backbone;
} Slice2ShimStrength_y;
double parameter
{
	display_name "z Shim values - Slice 2";
	format "%.3f";
	units "%";
	relations backbone;
} Slice2ShimStrength_z;

double parameter
{
	display_name "read Shim values - Slice 1";
} GradShimSlice1_r;
double parameter
{
	display_name "phase Shim values - Slice 1";
} GradShimSlice1_p;
double parameter
{
	display_name "slice Shim values - Slice 1";
} GradShimSlice1_s;
double parameter
{
	display_name "read Shim values - Slice 2";
} GradShimSlice2_r;
double parameter
{
	display_name "phase Shim values - Slice 2";
} GradShimSlice2_p;
double parameter
{
	display_name "slice Shim values - Slice 2";
} GradShimSlice2_s;


/*Gba - shim (end)*/
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

