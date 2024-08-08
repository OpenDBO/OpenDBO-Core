#include "StdAfx.h"
#include "NtlPacketUtil.h"
#include "NtlPacketAll.h"

const char* NtlGetPacketName(WORD wOpCode)
{
	if ( 0 <= wOpCode && UA_OPCODE_BEGIN > wOpCode )
	{		
		return "RESERVED SYSTEM PACKET";
	}
	else if (UA_OPCODE_BEGIN <= wOpCode && UA_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UA( wOpCode );
	}
	else if (AU_OPCODE_BEGIN <= wOpCode && AU_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_AU( wOpCode );
	}
	else if (UC_OPCODE_BEGIN <= wOpCode && UC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UC( wOpCode );
	}
	else if (CU_OPCODE_BEGIN <= wOpCode && CU_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_CU( wOpCode );
	}
	else if (UG_OPCODE_BEGIN <= wOpCode && UG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UG( wOpCode );
	}
	else if (GU_OPCODE_BEGIN <= wOpCode && GU_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GU( wOpCode );
	}
	else if (UT_OPCODE_BEGIN <= wOpCode && UT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UT( wOpCode );
	}
	else if (MA_OPCODE_BEGIN <= wOpCode && MA_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MA( wOpCode );
	}
	else if (AM_OPCODE_BEGIN <= wOpCode && AM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_AM( wOpCode );
	}
	else if (MC_OPCODE_BEGIN <= wOpCode && MC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MC( wOpCode );
	}
	else if (CM_OPCODE_BEGIN <= wOpCode && CM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_CM( wOpCode );
	}
	else if (MG_OPCODE_BEGIN <= wOpCode && MG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MG( wOpCode );
	}
	else if (GM_OPCODE_BEGIN <= wOpCode && GM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GM( wOpCode );
	}
	else if (MN_OPCODE_BEGIN <= wOpCode && MN_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MN( wOpCode );
	}
	else if (NM_OPCODE_BEGIN <= wOpCode && NM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_NM( wOpCode );
	}
	else if (MQ_OPCODE_BEGIN <= wOpCode && MQ_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MQ( wOpCode );
	}
	else if (QM_OPCODE_BEGIN <= wOpCode && QM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QM( wOpCode );
	}
	else if (MT_OPCODE_BEGIN <= wOpCode && MT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MT( wOpCode );
	}
	else if (TM_OPCODE_BEGIN <= wOpCode && TM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TM( wOpCode );
	}
	else if (ML_OPCODE_BEGIN <= wOpCode && ML_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_ML( wOpCode );
	}
	else if (LM_OPCODE_BEGIN <= wOpCode && LM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_LM( wOpCode );
	}
	else if (MP_OPCODE_BEGIN <= wOpCode && MP_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MP( wOpCode );
	}
	else if (PM_OPCODE_BEGIN <= wOpCode && PM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_PM( wOpCode );
	}
	else if (MS_OPCODE_BEGIN <= wOpCode && MS_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MS( wOpCode );
	}
	else if (SM_OPCODE_BEGIN <= wOpCode && SM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_SM( wOpCode );
	}
	//- yoshiki : Remove the commented lines!
//	else if (AL_OPCODE_BEGIN <= wOpCode && AL_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_AL( wOpCode );
//	}
//	else if (LA_OPCODE_BEGIN <= wOpCode && LA_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_LA( wOpCode );
//	}
	else if (CQ_OPCODE_BEGIN <= wOpCode && CQ_OPCODE_END >= wOpCode)
	{
			return NtlGetPacketName_CQ( wOpCode );
	}
	else if (QC_OPCODE_BEGIN <= wOpCode && QC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QC( wOpCode );
	}
	//- yoshiki : Remove the commented lines!
//	else if (CL_OPCODE_BEGIN <= wOpCode && CL_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_CL( wOpCode );
//	}
//	else if (LC_OPCODE_BEGIN <= wOpCode && LC_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_LC( wOpCode );
//	}
	else if (GN_OPCODE_BEGIN <= wOpCode && GN_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GN( wOpCode );
	}
	else if (NG_OPCODE_BEGIN <= wOpCode && NG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_NG( wOpCode );
	}
	else if (GQ_OPCODE_BEGIN <= wOpCode && GQ_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GQ( wOpCode );
	}
	else if (QG_OPCODE_BEGIN <= wOpCode && QG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QG( wOpCode );
	}
	else if (GT_OPCODE_BEGIN <= wOpCode && GT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GT( wOpCode );
	}
	else if (TG_OPCODE_BEGIN <= wOpCode && TG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TG( wOpCode );
	}
	//- yoshiki : Remove the commented lines!
//	else if (GL_OPCODE_BEGIN <= wOpCode && GL_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_GL( wOpCode );
//	}
//	else if (LG_OPCODE_BEGIN <= wOpCode && LG_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_LG( wOpCode );
//	}
//	else if (GP_OPCODE_BEGIN <= wOpCode && GP_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_GP( wOpCode );
//	}
//	else if (PG_OPCODE_BEGIN <= wOpCode && PG_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_PG( wOpCode );
//	}
//	else if (NL_OPCODE_BEGIN <= wOpCode && NL_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_NL( wOpCode );
//	}
//	else if (LN_OPCODE_BEGIN <= wOpCode && LN_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_LN( wOpCode );
//	}
//	else if (QL_OPCODE_BEGIN <= wOpCode && QL_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_QL( wOpCode );
//	}
//	else if (LQ_OPCODE_BEGIN <= wOpCode && LQ_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_LQ( wOpCode );
//	}
	else if (TL_OPCODE_BEGIN <= wOpCode && TL_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TL( wOpCode );
	}
	//- yoshiki : Remove the commented lines!
//	else if (LT_OPCODE_BEGIN <= wOpCode && LT_OPCODE_END >= wOpCode)
//	{
//		return NtlGetPacketName_LT( wOpCode );
//	}
	else if (ME_OPCODE_BEGIN <= wOpCode && ME_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_ME( wOpCode );
	}
	else if (EM_OPCODE_BEGIN <= wOpCode && EM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_EM( wOpCode );
	}
	else if (EO_OPCODE_BEGIN <= wOpCode && EO_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_EO( wOpCode );
	}
	else if (OE_OPCODE_BEGIN <= wOpCode && OE_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_OE( wOpCode );
	}
	else if (QT_OPCODE_BEGIN <= wOpCode && QT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QT( wOpCode );
	}
	else if (TQ_OPCODE_BEGIN <= wOpCode && TQ_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TQ( wOpCode );
	}
	else if (MR_OPCODE_BEGIN <= wOpCode && MR_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MR( wOpCode );
	}
	else if (RM_OPCODE_BEGIN <= wOpCode && RM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_RM( wOpCode );
	}
	else if (XR_OPCODE_BEGIN <= wOpCode && XR_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_XR( wOpCode );
	}
	else if (PT_OPCODE_BEGIN <= wOpCode && PT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_PT( wOpCode );
	}
	else if (TP_OPCODE_BEGIN <= wOpCode && TP_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TP( wOpCode );
	}
	else if (PS_OPCODE_BEGIN <= wOpCode && PS_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_PS( wOpCode );
	}
	else if (SP_OPCODE_BEGIN <= wOpCode && SP_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_SP( wOpCode );
	}
	else if (CP_OPCODE_BEGIN <= wOpCode && CP_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_CP( wOpCode );
	}
	else if (PC_OPCODE_BEGIN <= wOpCode && PC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_PC( wOpCode );
	}
	else if (PG_OPCODE_BEGIN <= wOpCode && PG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_PG( wOpCode );
	}
	else if (GP_OPCODE_BEGIN <= wOpCode && GP_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GP( wOpCode );
	}
	else
	{
		return "NOT DEFINED PACKET ID";
	}
}



bool NtlCheckPacketName(FILE * fp /* = NULL */)
{
	bool bPacketNameSuccess = true;

	const char * pTest = NULL;
	for( WORD i = 0; i < USHRT_MAX; i++ )
	{
		pTest = NtlGetPacketName( i );
		if( 0 == strncmp( pTest, "RESERVED SYSTEM PACKET", strlen( pTest) ) )
		{			
		}
		else if( 0 == strncmp( pTest, "NOT DEFINED PACKET ID", strlen( pTest) ) )
		{
		}
		else if( 0 == strncmp( pTest, "NOT DEFINED PACKET NAME : OPCODE LOW", strlen( pTest) ) )
		{
			if( fp ) fprintf_s( fp, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
			bPacketNameSuccess = false;
		}
		else if( 0 == strncmp( pTest, "NOT DEFINED PACKET NAME : OPCODE HIGH", strlen( pTest) ) )
		{
			if( fp ) fprintf_s( fp, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
			bPacketNameSuccess = false;
		}
		else if( 0 == strncmp( pTest, "OPCODE BUFFER OVERFLOW", strlen( pTest) ) )
		{
			if( fp ) fprintf_s( fp, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
			bPacketNameSuccess = false;
		}
		else
		{
			//fprintf_s( TRACE_APP, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
		}
	}

	return bPacketNameSuccess;
}