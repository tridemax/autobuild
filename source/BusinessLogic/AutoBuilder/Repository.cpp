#include "platform.h"
#include "Repository.h"


namespace AutoBuild
{
	const char Repository::LogPath[] = "/var/log/autobuild-logs";

	//-------------------------------------------------------------------------------------------------
	Repository::Repository() : m_hasConfig(false)
	{
	}

	//-------------------------------------------------------------------------------------------------
	Repository::~Repository()
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Update()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Build()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Deploy()
	{
		return true;
	}
}
