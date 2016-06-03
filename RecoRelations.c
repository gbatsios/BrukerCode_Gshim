/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/RecoRelations.c,v $
 *
 * Copyright (c) 2001 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: RecoRelations.c,v 1.1 2005/08/16 14:51:29 dwe Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      1



#include "method.h"

void SetRecoParam( void )
{

  int dim,i,complexDataCorr;

  DB_MSG(("-->SetRecoParam\n"));

  /* set baselevel reconstruction parameter */
  /* default initialization of reco based on acqp pars allready set */
  
  ATB_InitDefaultReco();

  /* configure information available during setup mode */

  GS_info_dig_filling     = Yes;
  ParxRelsParRelations("GS_info_dig_filling",Yes); 
  GS_info_normalized_area = Of_raw_data;
  ParxRelsParRelations("GS_info_normalized_area",Yes); 

  /* set reco rotate according to phase offsets     */

  dim = (int) PARX_get_dim("ACQ_size",1);
    
  for (i=0; i<dim; i++)
  {
    if (i==0)
      complexDataCorr = 2;
    else
      complexDataCorr = 1;
    RECO_size[i] = (int)(ACQ_size[i] / complexDataCorr) ;
  }


 DB_MSG(("<--SetRecoParam\n"));
}

