#include "StdAfx.h"
#include "NtlClientService.h"


namespace
{
	class CNtlClientService : public INtlClientService
	{
	public:
		virtual bool Begin( const char* szServiceCmd )
		{
			return true;
		}

		virtual void End( void )
		{
		}

		virtual unsigned int GetPublisherID( void )
		{
			return eDBO_SERVICE_PUBLISHER_NTL;
		}
	};
}

INtlClientService* GetServiceInterface( void )
{
	static CNtlClientService s_clService;

	return &s_clService;
}
