/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/config/proto.head,v $
 *
 * Copyright (c) 1995
 * BRUKER ANALYTISCHE MESSTECHNIK GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $State: Exp $
 *
 *******************************************************************
 */

#ifndef _P_
#	if defined(HAS_PROTO) || defined(__STDC__) || defined(__cplusplus)
#		define _P_(s) s
#	else
#		define _P_(s) ()
#	endif
#endif

/* /home/aic/Desktop/George/gba_MsliceSPECT_Gshim_trig/initMeth.c */
void initMeth _P_((void));
/* /home/aic/Desktop/George/gba_MsliceSPECT_Gshim_trig/loadMeth.c */
void loadMeth _P_((const char *));
/* /home/aic/Desktop/George/gba_MsliceSPECT_Gshim_trig/parsRelations.c */
void backbone _P_((void));
void DeadTimeRels _P_((void));
void DeadTimeRange _P_((void));
void updateDeadTime _P_((void));
void ExcPulseAngleRelation _P_((void));
void ExcPulseEnumRelation _P_((void));
void ExcPulseRelation _P_((void));
void ExcPulseRange _P_((void));
void repetitionTimeRels _P_((void));
void Local_NAveragesRange _P_((void));
void Local_NAveragesHandler _P_((void));
void NrepRange _P_((void));
void NrepRel _P_((void));
void SpecHandler _P_((void));
void SetGSparameters _P_((void));
void GeoModeRange _P_((void));
void NdummyRange _P_((void));
void NdummyRel _P_((void));
void LocalDecNoeHandling _P_((void));
void MyRgInitRel _P_((void));
void LocalGeometryMinimaRels _P_((double *));
void LocalGradientStrengthRels _P_((void));
void LocalFrequencyOffsetRels _P_((void));
void echoTimeRels _P_((void));
/* /home/aic/Desktop/George/gba_MsliceSPECT_Gshim_trig/BaseLevelRelations.c */
void SetBaseLevelParam _P_((void));
void SetBasicParameters _P_((void));
void SetFrequencyParameters _P_((void));
void SetGradientParameters _P_((void));
void SetInfoParameters _P_((void));
void SetMachineParameters _P_((void));
void SetPpgParameters _P_((void));
void CalGradforShim_Slice1 _P_((void));
void CalGradforShim_Slice2 _P_((void));
/* /home/aic/Desktop/George/gba_MsliceSPECT_Gshim_trig/RecoRelations.c */
void SetRecoParam _P_((void));
