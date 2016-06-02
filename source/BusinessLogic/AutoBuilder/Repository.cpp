#include "platform.h"
#include "Repository.h"


namespace AutoBuild
{
	const char Repository::LogsFolder[] = "/var/log/autobuild-logs";

	//-------------------------------------------------------------------------------------------------
	Repository::Repository() : m_lastBuildStatus(BuildStatus::Unknown), m_hasUpdates(false), m_wasFinalized(false)
	{
	}

	//-------------------------------------------------------------------------------------------------
	Repository::~Repository()
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::LoadConfiguration(const boost::property_tree::ptree::value_type& repositoryConfig)
	{
		const auto reportNoProperty = [](const char* property)
		{
			std::cerr << "One of the repositories has no '" << property << "' property." << std::endl;
		};

		const auto reportInvalidProperty = [](const char* property)
		{
			std::cerr << "One of the repositories has invalid '" << property << "' property." << std::endl;
		};

		// Assume all properties are valid
		bool successFlag = true;

		// Try read 'sourceControl' property
		try
		{
			m_sourceControl = SourceControlStringifier::FromString(repositoryConfig.second.get<std::string>("sourceControl"));

			if (m_sourceControl == SourceControl::Unknown)
			{
				successFlag = false;
				reportInvalidProperty("sourceControl");
			}
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("sourceControl");
		}

		// Try read 'sourceControlLogin' property
		try
		{
			m_sourceControlLogin = repositoryConfig.second.get<std::string>("sourceControlLogin");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("sourceControlLogin");
		}

		// Try read 'sourceControlPassword' property
		try
		{
			m_sourceControlPassword = repositoryConfig.second.get<std::string>("sourceControlPassword");
		}
		catch (...)
		{
			successFlag = false;
			std::cout << "One of the repositories has no 'sourceControlPassword' property." << std::endl;
		}

		// Try read 'sourceUrl' property
		try
		{
			m_sourceUrl = repositoryConfig.second.get<std::string>("sourceUrl");
		}
		catch (...)
		{
			successFlag = false;
			std::cout << "One of the repositories has no 'sourceUrl' property." << std::endl;
		}

		// Try read 'localPath' property
		try
		{
			m_localPath = repositoryConfig.second.get<std::string>("localPath");
		}
		catch (...)
		{
			successFlag = false;
			std::cout << "One of the repositories has no 'localPath' property." << std::endl;
		}

		// Try read 'buildMethod' property
		try
		{
			m_buildMethod = BuildMethodStringifier::FromString(repositoryConfig.second.get<std::string>("buildMethod"));

			if (m_buildMethod == BuildMethod::Unknown)
			{
				successFlag = false;
				reportInvalidProperty("buildMethod");
			}
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("buildMethod");
		}

		// Try read 'projectFile' property
		try
		{
			m_projectFile = repositoryConfig.second.get<std::string>("projectFile");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("projectFile");
		}

		// Try read 'projectConfiguration' property
		try
		{
			m_projectConfiguration = repositoryConfig.second.get<std::string>("projectConfiguration");
		}
		catch (...)
		{
			successFlag = false;
			std::cout << "One of the repositories has no 'projectConfiguration' property." << std::endl;
		}

		// Try read 'deployPath' property
		try
		{
			m_deployPath = repositoryConfig.second.get<std::string>("deployPath");
		}
		catch (...)
		{
			successFlag = false;
			std::cout << "One of the repositories has no 'deployPath' property." << std::endl;
		}

		// Try read 'dependentDaemons' property
		try
		{
			m_dependentDaemons = repositoryConfig.second.get<std::string>("dependentDaemons");
		}
		catch (...)
		{
			successFlag = false;
			std::cout << "One of the repositories has no 'dependentDaemons' property." << std::endl;
		}

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Update()
	{
		// Setup logging
		if (!OpenLogStream())
		{
			assert(false);
			return false;
		}

		// Run specific source control to update a local copy of the repository
		m_logStream << "Updating..." << std::endl;

		switch (m_sourceControl)
		{
		case SourceControl::Git:
			break;

		case SourceControl::Subversion:
			if (boost::filesystem::is_directory(m_localPath / ".svn"))
			{
				uint64_t currentRevision = 0u, updatedRevision = 0u;

				if (!SubversionGetRevision(currentRevision))
				{
					m_logStream << "Local copy is corrupted, trying to download a new one." << std::endl;
					break;
				}

				if (!SubversionUpdate(updatedRevision))
				{
					m_logStream << "Failed to update local copy, trying to download a new one." << std::endl;
					break;
				}

				m_hasUpdates = (currentRevision < updatedRevision);

				m_logStream << "Successfully updated." << std::endl;

				return true;
			}
			else
			{
				m_logStream << "Local copy not found, trying to download a new one." << std::endl;
			}
			break;
		}

		// Run specific source control to load a local copy of the repository
		m_logStream << "Loading..." << std::endl;

		switch (m_sourceControl)
		{
		case SourceControl::Git:
			break;

		case SourceControl::Subversion:
			try
			{
				boost::filesystem::remove_all(m_localPath);
			}
			catch (std::exception& exception)
			{
				m_logStream << "Unable to clear the repository because of: " << exception.what() << std::endl;
				return false;
			}

			if (!SubversionLoad())
			{
				m_logStream << "Failed to load the repository using subversion." << std::endl;
				return false;
			}

			m_hasUpdates = true;

			m_logStream << "Successfully loaded." << std::endl;

			break;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Build()
	{
		// Run specific build method
		m_logStream << std::endl << "Building..." << std::endl;

		switch (m_buildMethod)
		{
		case BuildMethod::Qmake:
			if (!QmakeRun())
			{
				m_logStream << "Failed to build the repository using qmake." << std::endl;
				return false;
			}
			break;

		case BuildMethod::Xbuild:
			if (!XbuildRun())
			{
				m_logStream << "Failed to build the repository using xbuild." << std::endl;
				return false;
			}
			break;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Deploy()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	void Repository::Finalize(BuildStatus buildStatus)
	{
		switch (buildStatus)
		{
		case BuildStatus::Success:
			m_logStream << "Build status: success." << std::endl;
			break;

		case BuildStatus::UpdateFailed:
			m_logStream << "Build status: update failed." << std::endl;
			break;

		case BuildStatus::BuildFailed:
			m_logStream << "Build status: build failed." << std::endl;
			break;

		case BuildStatus::DeployFailed:
			m_logStream << "Build status: deploy failed." << std::endl;
			break;

		default:
			m_logStream << "Build status: unknown." << std::endl;
		}

		m_logStream.flush();
		m_logStream.close();

		m_wasFinalized = true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::OpenLogStream()
	{
		// Build log path
		boost::filesystem::path logPath;

		logPath /= LogsFolder;
		logPath /= m_deployPath.filename();

		// Try extract last build status before the log will be overwritten
		ReadLastBuildStatus(logPath.c_str());

		// Truncate log file and open for writing
		if (!m_logStream.is_open())
		{
			m_logStream.open(logPath.c_str(), std::ios_base::out | std::ios_base::trunc);
		}

		return m_logStream.is_open();
	}

	//-------------------------------------------------------------------------------------------------
	void Repository::ReadLastBuildStatus(const char* logPath)
	{
		FILE* logFile = fopen(logPath, "r");

		if (!logFile)
		{
			m_lastBuildStatus = BuildStatus::Unknown;
			return;
		}

		// Read subversion output
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, logFile) != -1)
		{
			if (boost::istarts_with(lineBuffer, "build status"))
			{
				std::string revisionStr(lineBuffer + 12u);
				boost::trim_if(revisionStr, boost::is_any_of("\r\n\t :."));

				if (strcasecmp(revisionStr.c_str(), "success") == 0)
				{
					 m_lastBuildStatus = BuildStatus::Success;
				}
				else if (strcasecmp(revisionStr.c_str(), "update failed") == 0)
				{
					m_lastBuildStatus = BuildStatus::UpdateFailed;
				}
				else if (strcasecmp(revisionStr.c_str(), "build failed") == 0)
				{
					m_lastBuildStatus = BuildStatus::BuildFailed;
				}
				else if (strcasecmp(revisionStr.c_str(), "deploy failed") == 0)
				{
					m_lastBuildStatus = BuildStatus::DeployFailed;
				}
				else
				{
					m_lastBuildStatus = BuildStatus::Unknown;
				}
			}
		}

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		fclose(logFile);
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::SubversionGetRevision(uint64_t& revisionValue)
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "svn info ";
		commandLine += m_localPath.string();
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = false;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "svn"))
			{
				m_logStream << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "revision"))
			{
				std::string revisionStr(lineBuffer + 8u);
				boost::trim_if(revisionStr, boost::is_any_of("\r\n\t :."));

				revisionValue = atoll(revisionStr.c_str());

				successFlag = true;
			}
		}

		m_logStream.flush();

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		pclose(processPipe);

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::SubversionUpdate(uint64_t& revisionValue)
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "svn update --force --no-auth-cache --non-interactive --trust-server-cert --accept theirs-full --username ";
		commandLine += m_sourceControlLogin;
		commandLine += " --password ";
		commandLine += m_sourceControlPassword;
		commandLine += ' ';
		commandLine += m_localPath.string();
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = false;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "svn") ||
				boost::istarts_with(lineBuffer, "updating") ||
				boost::istarts_with(lineBuffer, "fetching") ||
				boost::istarts_with(lineBuffer, "external"))
			{
				m_logStream << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "at revision"))
			{
				std::string revisionStr(lineBuffer + 11u);
				boost::trim_if(revisionStr, boost::is_any_of("\r\n\t :."));

				revisionValue = atoll(revisionStr.c_str());

				successFlag = true;

				m_logStream << lineBuffer;
			}
		}

		m_logStream.flush();

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		pclose(processPipe);

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::SubversionLoad()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "svn checkout --force --no-auth-cache --non-interactive --trust-server-cert --username ";
		commandLine += m_sourceControlLogin;
		commandLine += " --password ";
		commandLine += m_sourceControlPassword;
		commandLine += ' ';
		commandLine += m_sourceUrl;
		commandLine += ' ';
		commandLine += m_localPath.string();
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = false;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "svn") ||
				boost::istarts_with(lineBuffer, "fetching"))
			{
				m_logStream << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "checked out"))
			{
				m_logStream << lineBuffer;

				if (!boost::ifind_first(lineBuffer, "external"))
				{
					successFlag = true;
				}
			}
		}

		m_logStream.flush();

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		pclose(processPipe);

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::QmakeRun()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::XbuildRun()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "xbuild /nologo /verbosity:normal /target:rebuild /property:configuration=";
		commandLine += m_projectConfiguration;
		commandLine += " /property:platform=x64 ";
		commandLine += (m_localPath / m_projectFile).string();
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = true;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			m_logStream << lineBuffer;

			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "msbuild") ||
				boost::istarts_with(lineBuffer, "configuration"))
			{
//				m_logStream << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "warning"))
			{
//				m_logStream << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "error"))
			{
				successFlag = false;

//				m_logStream << lineBuffer;
			}
		}

		m_logStream.flush();

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		pclose(processPipe);

		return successFlag;
	}
}
