/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/NSPECT.c,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: NSPECT.c,v 1.2 2005/08/16 15:02:51 dwe Exp $
 *
 ****************************************************************/


method gba_MsliceSPECT_Gshim_trig
{

/****************************************************************/
/*	TYPE DEFINITIONS					*/
/****************************************************************/


#include "bruktyp.h"
#include "acqutyp.h"
#include "acqumtyp.h"
#include "recotyp.h" 
#include "subjtyp.h" 
#include "ta_config.h" 
#include "methodTypes.h"
#include "adjManagerDefs.h"
#include "adjManagerTypes.h"




/****************************************************************/
/*	PARAMETER DEFINITIONS					*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Include external definitions for parameters in the classes
 * ACQU ACQP GO GS RECO RECI PREEMP CONFIG
 *--------------------------------------------------------------*/
#include "proto/acq_extern.h"
#include "proto/subj_extern.h"

/*--------------------------------------------------------------*
 * Include references to the standard method parameters
 *--------------------------------------------------------------*/
#include "proto/pvm_extern.h"

/*--------------------------------------------------------------*
 * Include references to any method specific parameters
 *--------------------------------------------------------------*/

#include "methodFormat.h"
#include "parsTypes.h"
#include "parsDefinition.h"

/****************************************************************/
/*	RE-DEFINITION OF RELATIONS				*/
/****************************************************************/

#include "callbackDefs.h"

/****************************************************************/
/*	PARAMETER CLASSES					*/
/****************************************************************/
#include "methodClassDefs.h"
#include "seqApiClassDefs.h"
#include "modulesClassDefs.h"
#include "parsLayout.h"

};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

