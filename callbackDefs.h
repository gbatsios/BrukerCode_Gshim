/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/callbackDefs.h,v $
 *
 * Copyright (c) 1999-2001
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: callbackDefs.h,v 1.2.2.2 2007/07/26 19:27:13 mawi Exp $
 *
 ****************************************************************/

/* init of auto RG */
relations PVM_AutoRgInitHandler MyRgInitRel;

/* digitizer group */
relations PVM_DigHandler        backbone;

/* spectroscopy group */
relations PVM_SpecHandler       backbone;

/* modules */
relations PVM_FatSupHandler     backbone;

/*kw-mod 101014 begin*/
relations PVM_TriggerHandler    backbone;
relations PVM_TriggerOutHandler backbone;
/*kw-mod 101014 end*/

/* slice geometry: */
relations PVM_SliceGeometryHandler  backbone; /*KW-sliceSPECT*/

relations PVM_ExSliceSelectionHandler backbone; /*KW-sliceSPECT*/

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_NRepetitions      NrepRel;    
relations PVM_ExcPulseAngle     ExcPulseAngleRelation;
relations PVM_GeoMode           backbone;
relations PVM_SatSlicesHandler  backbone;
relations PVM_WsHandler         backbone;
relations PVM_DecHandler        backbone;
relations PVM_NoeHandler        backbone;
relations PVM_EncodingHandler   backbone;
/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







