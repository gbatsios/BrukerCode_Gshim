/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/BaseLevelRelations.c,v $
 *
 * Copyright (c) 2001 - 2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: BaseLevelRelations.c,v 1.3.2.3 2008/01/30 08:48:25 sako Exp $
 *
 ****************************************************************/
static const char resid[] = "$Id: BaseLevelRelations.c,v 1.3.2.3 2008/01/30 08:48:25 sako Exp $ (C) 2002 - 2005 Bruker BioSpin MRI GmbH";

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



void SetBaseLevelParam( void )
{

  DB_MSG(("Entering SetBaseLevelParam()"));

  SetBasicParameters();

  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: Error in function call!");
    return;
  }
  
  SetMachineParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  
  SetFrequencyParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  SetPpgParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  SetGradientParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  SetInfoParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  

  /* 
   *  settings for multi channel acquisition 
   */

  if(Yes==ATB_SetMultiRec())
  {
    ATB_SetPulprog("gba_MsliceSPECT_Gshim_trig.4ch");
  }

  if(PVM_EncUseMultiRec == Yes)
  {
      int nrec,i;
      nrec = (int)PARX_get_dim("ACQ_ReceiverSelect",1);

      ACQ_ReceiverSelect[0] = Yes;
      for(i=1;i<nrec;i++)
	ACQ_ReceiverSelect[i]=No; 
  }


  /* setting pipeline filter parameters */

  if((PVM_EncUseMultiRec==Yes) || (FrequencyLock_OnOff == On))
  {
    if((PVM_EncUseMultiRec==Yes) && (FrequencyLock_OnOff == Off))
    {
      /* pipeline filter for data combination */
      ACQ_user_filter = Yes;
      ParxRelsParRelations("ACQ_user_filter",Yes);
      strcpy( ACQ_user_filter_name, "Combine_Spec");
      ParxRelsParRelations("ACQ_user_filter_name",Yes);
      ACQ_user_filter_memory = For_one_PE_step;
      ParxRelsParRelations("ACQ_user_filter_memory",Yes);
      ACQ_user_filter_mode = Special;
      ParxRelsParRelations("ACQ_user_filter_mode",Yes);
      ACQ_user_filter_size[0] = ACQ_user_filter_size[1] = 0;
      ACQ_user_filter_size[2] = PVM_EncAvailReceivers;
      ParxRelsParRelations("ACQ_user_filter_size",Yes);
      L[0]=1;
    }
    else
    {
      ACQ_user_filter = Yes;
      ParxRelsParRelations("ACQ_user_filter",Yes);
      strcpy( ACQ_user_filter_name, "NAV_pressAU");
      ParxRelsParRelations("ACQ_user_filter_name",Yes);
      ACQ_user_filter_memory = For_one_PE_step;
      ParxRelsParRelations("ACQ_user_filter_memory",Yes);
      ACQ_user_filter_mode = Special;
      ParxRelsParRelations("ACQ_user_filter_mode",Yes);
      ACQ_user_filter_size[0] = 0;
      ACQ_user_filter_size[1] = 2*PVM_NRepetitions*PVM_NAverages;
      ACQ_user_filter_size[2] = PVM_EncUseMultiRec == Yes ?
	PVM_EncAvailReceivers:0;
      ParxRelsParRelations("ACQ_user_filter_size",Yes);
      L[0]=2;
      DS *=2;
    }
  }
  else
  {
    ACQ_user_filter = No;
    ParxRelsParRelations("ACQ_user_filter",Yes);
    L[0]=1;
  }

  /* setting baselevel parameters used by modules */
  ATB_SetFatSupBaselevel();
  ATB_SetDecBaseLevel();
  ATB_SetNoeBaseLevel();
  ATB_SetWsBaseLevel();
  ATB_SetSatSlicesBaseLevel();

  /*kw-mod 101014 begin*/
   ATB_SetTriggerBaseLevel();
  ATB_SetTriggerOutBaseLevel();
  /*kw_mod 101014 end*/

  DB_MSG(("Exiting SetBaseLevelParam"));
  
}





void SetBasicParameters( void )
{
  int specDim;
  int dim;
  int nSlices;
  
  DB_MSG(("Entering SetBasicParameters()"));
  
  /* ACQ_dim */
  
  specDim = PTB_GetSpecDim();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  ACQ_dim = specDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */
  
  /*ATB_SetAcqDimDesc( specDim, spatDim, NULL );  */
  for(dim=0; dim<ACQ_dim; dim++)
    ACQ_dim_desc[dim] = Spectroscopic;
  ParxRelsParRelations("ACQ_dim_desc", Yes);
  
  /* ACQ_size */
  
  
  ACQ_size[0] =  2*PVM_SpecMatrix[0];
  
  for(dim=1; dim<ACQ_dim; dim++)
    ACQ_size[dim] = PVM_SpecMatrix[dim];
  
  ParxRelsParRelations("ACQ_size", Yes);



  
  /* NSLICES */
   /* start - Gba_040311 */
   
   nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }

  ATB_SetNSlices( nSlices );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }
	
	/* end - Gba_040311 */
    
  /* NR */
  
  ATB_SetNR( PVM_NRepetitions );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* NI */
  
  ATB_SetNI( nSlices * PVM_NEchoImages );		/*Gba 040311*/
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  /* NA */
  
  ATB_SetNA( PVM_NAverages );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  /* NAE */
  
  ATB_SetNAE( 1 );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  
  
  /* ACQ_ns */
  
  ACQ_ns_list_size = 1;
  
  dim = PARX_get_dim("ACQ_ns_list",1);
  if( dim != 1 )
  {
    PARX_change_dims( "ACQ_ns_list",1 );
  }
  
  NS = 1;
  ACQ_ns = NS;
  ACQ_ns_list[0] = ACQ_ns;
  
  ParxRelsParRelations("ACQ_ns",Yes);
  
  
  /* NECHOES */
  
  NECHOES = 1;
  
  
  
  /* ACQ_obj_order */
  
  PARX_change_dims("ACQ_obj_order",NI);
  ACQ_obj_order[0] = 0;
  
  
  /* DS */
  
  DS =Ndummy ;
  ACQ_DS_enabled = Yes;
  
  
  ATB_DisableAcqUserFilter();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  ATB_SetAcqScanSize( One_scan );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  DB_MSG(("Exiting SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{
  int nslices;
  
  DB_MSG(("Entering SetFrequencyParameters()"));
  
  ATB_SetNuc1(PVM_Nucleus1);
  
  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");
  
  ATB_SetNucleus(PVM_Nucleus1);
  
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  ATB_SetRouting();
  
  /* setting of SW_h, DIGMOD, DSPFIRM and AQ_mod */
  ATB_SetDigPars();
  
  ACQ_O1_mode = BF_plus_Offset_list; //kw-mod
  ParxRelsParRelations("ACQ_O1_mode",Yes);
  
  ACQ_O2_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O2_mode",Yes);
  
  ACQ_O3_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O3_mode",Yes);

  O1 = 0.0; 
  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;

  /*gba begin 040311 */
  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetFrequencyParameters: In function call!");
      return;
    }
  
    ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );
	if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetFrequencyParameters: In function call!");
      return;
    }

    ATB_SetAcqO1BList( nslices,
					PVM_ObjOrderList,
                    PVM_ReadOffsetHz);
    
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetFrequencyParameters: In function call!");
      return;
    }
    /*gba end 040311 */
  
  
  DB_MSG(("Exiting SetFrequencyParameters()"));
}

void SetGradientParameters( void )
{

  int dim;		//gba-shim
  
  DB_MSG(("Entering SetGradientParameters()"));
  
  
  ATB_SetAcqPhaseFactor( 1 );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetGradientParameters: In function call!");
    return;
  }
  
  { 
    double GradOrient[3][3] = {{1.0, 0.0, 0.0}, 
                               {0.0, 1.0, 0.0},
                               {0.0, 0.0, 1.0}};
    int ObjOrderList[1] = {0},
      npack = 1,
        nslperpack[1] = {1};
      

    ATB_SetAcqGradMatrix( PVM_NSPacks, PVM_SPackArrNSlices,
			PtrType3x3 PVM_SPackArrGradOrient[0],		/*KW-sliceSPECT*/
			PVM_ObjOrderList );
	
	/*Gba - shim (start)*/
	CalGradforShim_Slice1();
	dim = PARX_get_dim("ACQ_grad_matrix",1);
    if( dim != 1 )
	{
		CalGradforShim_Slice2();
	}
	else
	{
		GradShimSlice2_p = 0;				/* t7  */
		GradShimSlice2_s = 0;				/* t8  */
		GradShimSlice2_r = 0;
	}
	/*Gba - shim (end)*/
    
	/*
      ATB_SetAcqGradMatrix( npack, nslperpack,
			    &GradOrient,
			    ObjOrderList );*/
  }
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetGradientParameters: In function call!");
    return;
  }
  
  
  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  ACQ_rare_factor = 1;
  
  ACQ_grad_str_X = 0.0;
  ACQ_grad_str_Y = 0.0;
  ACQ_grad_str_Z = 0.0;
  
  
  strcpy(GRDPROG, "");

  ATB_SetAcqTrims( 13,
		  PVM_ExSliceGradient,	         /* t0 */   /*KW-sliceSPECT*/
                   (-PVM_ExSliceRephaseGradient),/* t1 */
		   SliceSpoilerStrength,          /* t2 */
		   (-GradShimSlice1_r),			/* t3  */
		   GradShimSlice1_p,				/* t4  */
		   (-GradShimSlice1_s),				/* t5  */
		   (-GradShimSlice2_r),				/* t6  */
		   GradShimSlice2_p,				/* t7  */
		   (-GradShimSlice2_s),				/* t8  */
		   (PVM_ExSliceGradient - GradShimSlice1_s),				/* t9  */
		   (-PVM_ExSliceRephaseGradient + GradShimSlice1_s)	,			/* t10  */
		   (PVM_ExSliceGradient - GradShimSlice2_s),				/* t11  */
		   (-PVM_ExSliceRephaseGradient + GradShimSlice2_s)				/* t12  */
		   
                   );
  
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetGradientParameters: In function call!");
      return;
    }



  DB_MSG(("Exiting SetGradientParameters()"));
}

void SetInfoParameters( void )
{

  int slices, i;
  
  DB_MSG(("Entering SetInfoParameters()"));
  
  
  ACQ_flip_angle = PVM_ExcPulseAngle;
  
  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse.Length;
  
  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;
  
  /*gba begin 040311 */
  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetInfoParameters: In function call!");
      return;
    }

  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);

  for(i=0;i<slices;i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }


  ACQ_read_ext = (int)PVM_AntiAlias[0];

  PARX_change_dims("ACQ_slice_sepn", slices==1 ? 1 : slices-1);

  if( slices == 1 )
    {
      ACQ_slice_sepn[0] = 0.0;
    }
  else
    {
      for( i=1; i<slices;i++ )
	{
	  ACQ_slice_sepn[i-1]=PVM_SliceOffset[i]-PVM_SliceOffset[i-1];
	}
    }

  ATB_SetAcqSliceSepn( PVM_SPackArrSliceDistance,
                       PVM_NSPacks );
					   
  /* gba end 040311 */					   
  
  ATB_SetAcqPatientPosition();
  
  ATB_SetAcqMethod();
  
  DB_MSG(("Exiting SetInfoParameters()"));
  
}

void SetMachineParameters( void )
{
  DB_MSG(("Entering SetMachineParameters()"));
  
  
  if( ParxRelsParHasValue("ACQ_word_size") == No )
  {
    ACQ_word_size = _32_BIT;
  }
  
  DE = (DE < 6.0) ? 6.0:DE;
  
  DEOSC = (PVM_SpecAcquisitionTime + PVM_DigEndDelOpt)*1000.0;
  ACQ_scan_shift = 0;
  ParxRelsParRelations("ACQ_scan_shift",Yes);
  
  PAPS = QP;
  
  ACQ_BF_enable = Yes;
  
  DB_MSG(("Exiting SetMachineParameters"));
}

void SetPpgParameters( void )
{
  DB_MSG(("Entering SetPpgParameters()"));
  
  if( ParxRelsParHasValue("ACQ_trigger_enable") == No )
  {
    ACQ_trigger_enable = No;
  }
  
  if( ParxRelsParHasValue("ACQ_trigger_reference") == No )
  {
    ACQ_trigger_reference[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_trigger_delay") == No )
  {
    ACQ_trigger_delay = 0;
  }
  
  ParxRelsParRelations("ACQ_trigger_reference",Yes);
  
  
  ACQ_vd_list_size=1;
  PARX_change_dims("ACQ_vd_list",1);
  ACQ_vd_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vd_list",Yes);
  
  ACQ_vp_list_size=1;
  PARX_change_dims("ACQ_vp_list",1);
  ACQ_vp_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vp_list",Yes);





  ATB_SetPulprog("gba_MsliceSPECT_Gshim_trig.ppg");

  D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/NSLICES 
            + 5.0) / 1000.0;
  D[1] = DeadTime / 1000.0;
  D[2] = PVM_DigEndDelOpt/1000.0;
  D[8] = CFG_AmplifierEnable()/1000.0;

  /*KW-sliceSPEC (begin) 100714*/
  D[4]  = PVM_RampTime / 1000.0;
  D[3]  = (PVM_RiseTime) / 1000.0;
  D[10] = (PVM_ExSliceRephaseTime - PVM_RampTime) / 1000.0;
  D[11]  = (PVM_EchoTime - PVM_MinEchoTime) / 1000.0;
  D[6] = (SliceSpoilerDuration - PVM_RiseTime)/1000.0;
  /*KW-sliceSPEC (end) 100714*/

  ParxRelsParRelations("D",Yes);
  
  /* set shaped pulses, in this method TPQQ[0] is used for RF excitation   
   * and TPQQ[1] for navigator signal                                 */
  
  sprintf(TPQQ[0].name,ExcPulse.Filename);
  sprintf(TPQQ[1].name,ExcPulse.Filename);
  if(PVM_DeriveGains == Yes)
  {
    TPQQ[0].power  = ExcPulse.Attenuation;
    TPQQ[1].power  = NavAttenuation;
  }
  TPQQ[0].offset = 0.0;
  TPQQ[1].offset = 0.0;
  
  ParxRelsParRelations("TPQQ",Yes);
  
  
  /* set duration of pulse, in this method P[0] is used          */
    
  P[0] = ExcPulse.Length * 1000;
  
  ParxRelsParRelations("P",Yes);
  
  
  DB_MSG(("Exiting SetPpgParameters"));
}

/*Gba - shim (start)*/
void CalGradforShim_Slice1(void)
{
	//double* a;
	double x, y, z;
	//a = ACQ_grad_matrix[0];
	x = Slice1ShimStrength_x*3/100;
	y = Slice1ShimStrength_y*3/100;
	z = Slice1ShimStrength_z*3/100;
	det = ACQ_grad_matrix[0][0][0]*(ACQ_grad_matrix[0][1][1]*ACQ_grad_matrix[0][2][2]-ACQ_grad_matrix[0][2][1]*ACQ_grad_matrix[0][1][2])-ACQ_grad_matrix[0][0][1]*(ACQ_grad_matrix[0][1][0]*ACQ_grad_matrix[0][2][2]-ACQ_grad_matrix[0][1][2]*ACQ_grad_matrix[0][2][0])+ACQ_grad_matrix[0][0][2]*(ACQ_grad_matrix[0][1][0]*ACQ_grad_matrix[0][2][1]-ACQ_grad_matrix[0][1][1]*ACQ_grad_matrix[0][2][0]);  //adjoin
	
	out[0][0] = (ACQ_grad_matrix[0][1][1]*ACQ_grad_matrix[0][2][2]-ACQ_grad_matrix[0][1][2]*ACQ_grad_matrix[0][2][1])/det;
	out[0][1] = (ACQ_grad_matrix[0][0][2]*ACQ_grad_matrix[0][2][1]-ACQ_grad_matrix[0][0][1]*ACQ_grad_matrix[0][2][2])/det;
	out[0][2] = (ACQ_grad_matrix[0][0][1]*ACQ_grad_matrix[0][1][2]-ACQ_grad_matrix[0][0][2]*ACQ_grad_matrix[0][1][1])/det;
	out[1][0] = (ACQ_grad_matrix[0][1][2]*ACQ_grad_matrix[0][2][0]-ACQ_grad_matrix[0][1][0]*ACQ_grad_matrix[0][2][2])/det;
	out[1][1] = (ACQ_grad_matrix[0][0][0]*ACQ_grad_matrix[0][2][2]-ACQ_grad_matrix[0][0][2]*ACQ_grad_matrix[0][2][0])/det;
	out[1][2] = (ACQ_grad_matrix[0][0][2]*ACQ_grad_matrix[0][1][0]-ACQ_grad_matrix[0][0][0]*ACQ_grad_matrix[0][1][2])/det;
	out[2][0] = (ACQ_grad_matrix[0][1][0]*ACQ_grad_matrix[0][2][1]-ACQ_grad_matrix[0][1][1]*ACQ_grad_matrix[0][2][0])/det;
	out[2][1] = (ACQ_grad_matrix[0][0][1]*ACQ_grad_matrix[0][2][0]-ACQ_grad_matrix[0][0][0]*ACQ_grad_matrix[0][2][1])/det;
	out[2][2] = (ACQ_grad_matrix[0][0][0]*ACQ_grad_matrix[0][1][1]-ACQ_grad_matrix[0][0][1]*ACQ_grad_matrix[0][1][0])/det;

	GradShimSlice1_r = x*out[0][0]+y*out[1][0]+z*out[2][0];
	GradShimSlice1_p = x*out[0][1]+y*out[1][1]+z*out[2][1];
	GradShimSlice1_s = x*out[0][2]+y*out[1][2]+z*out[2][2];
}
void CalGradforShim_Slice2(void)
{
	double x, y, z;
	//a = ACQ_grad_matrix[1];
	x = Slice2ShimStrength_x*3/100;
	y = Slice2ShimStrength_y*3/100;
	z = Slice2ShimStrength_z*3/100;
	det2 = ACQ_grad_matrix[1][0][0]*(ACQ_grad_matrix[1][1][1]*ACQ_grad_matrix[1][2][2]-ACQ_grad_matrix[1][2][1]*ACQ_grad_matrix[1][1][2])-ACQ_grad_matrix[1][0][1]*(ACQ_grad_matrix[1][1][0]*ACQ_grad_matrix[1][2][2]-ACQ_grad_matrix[1][1][2]*ACQ_grad_matrix[1][2][0])+ACQ_grad_matrix[1][0][2]*(ACQ_grad_matrix[1][1][0]*ACQ_grad_matrix[1][2][1]-ACQ_grad_matrix[1][1][1]*ACQ_grad_matrix[1][2][0]);  //adjoin
	
	out2[0][0] = (ACQ_grad_matrix[1][1][1]*ACQ_grad_matrix[1][2][2]-ACQ_grad_matrix[1][1][2]*ACQ_grad_matrix[1][2][1])/det2;
	out2[0][1] = (ACQ_grad_matrix[1][0][2]*ACQ_grad_matrix[1][2][1]-ACQ_grad_matrix[1][0][1]*ACQ_grad_matrix[1][2][2])/det2;
	out2[0][2] = (ACQ_grad_matrix[1][0][1]*ACQ_grad_matrix[1][1][2]-ACQ_grad_matrix[1][0][2]*ACQ_grad_matrix[1][1][1])/det2;
	out2[1][0] = (ACQ_grad_matrix[1][1][2]*ACQ_grad_matrix[1][2][0]-ACQ_grad_matrix[1][1][0]*ACQ_grad_matrix[1][2][2])/det2;
	out2[1][1] = (ACQ_grad_matrix[1][0][0]*ACQ_grad_matrix[1][2][2]-ACQ_grad_matrix[1][0][2]*ACQ_grad_matrix[1][2][0])/det2;
	out2[1][2] = (ACQ_grad_matrix[1][0][2]*ACQ_grad_matrix[1][1][0]-ACQ_grad_matrix[1][0][0]*ACQ_grad_matrix[1][1][2])/det2;
	out2[2][0] = (ACQ_grad_matrix[1][1][0]*ACQ_grad_matrix[1][2][1]-ACQ_grad_matrix[1][1][1]*ACQ_grad_matrix[1][2][0])/det2;
	out2[2][1] = (ACQ_grad_matrix[1][0][1]*ACQ_grad_matrix[1][2][0]-ACQ_grad_matrix[1][0][0]*ACQ_grad_matrix[1][2][1])/det2;
	out2[2][2] = (ACQ_grad_matrix[1][0][0]*ACQ_grad_matrix[1][1][1]-ACQ_grad_matrix[1][0][1]*ACQ_grad_matrix[1][1][0])/det2;

	GradShimSlice2_r = x*out2[0][0]+y*out2[1][0]+z*out2[2][0];
	GradShimSlice2_p = x*out2[0][1]+y*out2[1][1]+z*out2[2][1];
	GradShimSlice2_s = x*out2[0][2]+y*out2[1][2]+z*out2[2][2];
}
/*Gba - shim (end)*/
