/*****************************************************************************
*
* File			: NtlClientService.h
* Abstract		: Stand-in for the original NCS-style publisher/service
*                 abstraction, which was not part of this source drop.
*                 This fork only targets the NTL/Dev publisher, so the
*                 interface below always resolves to that publisher.
*
*****************************************************************************/


#pragma once


enum eDBO_SERVICE_PUBLISHER
{
	eDBO_SERVICE_PUBLISHER_NTL = 0,
	eDBO_SERVICE_PUBLISHER_CJI_KOR,
};


class INtlClientService
{
public:
	virtual ~INtlClientService( void ) {}

	virtual bool			Begin( const char* szServiceCmd ) = 0;
	virtual void			End( void ) = 0;
	virtual unsigned int	GetPublisherID( void ) = 0;
};

INtlClientService* GetServiceInterface( void );
