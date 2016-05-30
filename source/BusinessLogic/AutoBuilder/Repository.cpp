#include "platform.h"
#include "Repository.h"


namespace AutoBuild
{
	const char Repository::LogsFolder[] = "/var/log/autobuild-logs";

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
		if (!OpenLogStream())
		{
			assert(false);
			return false;
		}

		m_logStream << "Repository " << m_deployPath.filename() << " updating..." << std::endl;

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

	//-------------------------------------------------------------------------------------------------
	bool Repository::OpenLogStream()
	{
		if (!m_logStream.is_open())
		{
			boost::filesystem::path logPath;
			logPath /= LogsFolder;
			logPath /= m_deployPath.filename();

			m_logStream.open(logPath.c_str(), std::ios_base::out | std::ios_base::trunc);
		}

		return m_logStream.is_open();
	}
}
