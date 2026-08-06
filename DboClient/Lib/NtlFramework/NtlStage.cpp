#include "precomp_ntlframework.h"
#include "NtlStage.h"

/**
*
*  constructor for CNtlStage
*  \param pStageName는 stage name이다.
*
*/
CNtlStage::CNtlStage(const char *pStageName)
:m_strName(pStageName)
{

}

/**
*
*  virtual Destructor for CNtlStage
*
*/

CNtlStage::~CNtlStage() 
{
}


bool CNtlStage::Create(void)
{
	return true;
}

void CNtlStage::Destroy(void)
{
}

void CNtlStage::PostRender(void)
{
}


