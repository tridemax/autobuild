#include "platform.h"
#include "Repository.h"


namespace AutoBuild
{
	const char Repository::LogsFolder[] = "/var/log/autobuild-logs";
	const char Repository::StageTag[] = "Stage:\t\t";
	const char Repository::DetailTag[] = "Detail:\t\t";
	const char Repository::WarningTag[] = "Warning:\t";
	const char Repository::ErrorTag[] = "Error:\t\t";
	const char Repository::SuccessTag[] = "Success:\t";

	//-------------------------------------------------------------------------------------------------
	Repository::Repository() : m_lastAttemptFailed(false), m_hasUpdates(false)
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
			reportNoProperty("sourceControlPassword");
		}

		// Try read 'sourceUrl' property
		try
		{
			m_sourceUrl = repositoryConfig.second.get<std::string>("sourceUrl");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("sourceUrl");
		}

		// Try read 'localPath' property
		try
		{
			m_localPath = repositoryConfig.second.get<std::string>("localPath");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("localPath");
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

		// Try read 'buildScript' property
		try
		{
			m_buildScript = repositoryConfig.second.get<std::string>("buildScript");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("buildScript");
		}

		// Try read 'buildConfiguration' property
		try
		{
			m_buildConfiguration = repositoryConfig.second.get<std::string>("buildConfiguration");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("buildConfiguration");
		}

		// Try read 'buildPlatform' property
		try
		{
			m_buildPlatform = repositoryConfig.second.get<std::string>("buildPlatform");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("buildPlatform");
		}

		// Try read 'deployPath' property
		try
		{
			m_deployPath = repositoryConfig.second.get<std::string>("deployPath");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("deployPath");
		}

		// Try read 'dependentDaemons' property
		try
		{
			m_dependentDaemons = repositoryConfig.second.get<std::string>("dependentDaemons");
		}
		catch (...)
		{
			successFlag = false;
			reportNoProperty("dependentDaemons");
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
		m_logStream << StageTag << "Updating..." << std::endl;

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
					m_logStream << WarningTag << "Local copy is corrupted, trying to download a new one." << std::endl;
					break;
				}

				if (!SubversionUpdate(updatedRevision))
				{
					m_logStream << WarningTag << "Failed to update local copy, trying to download a new one." << std::endl;
					break;
				}

				m_hasUpdates = (currentRevision < updatedRevision);

				m_logStream << SuccessTag << "Updating completed." << std::endl;

				return true;
			}
			else
			{
				m_logStream << WarningTag << "Local copy not found, trying to download a new one." << std::endl;
			}
			break;
		}

		// Run specific source control to load a local copy of the repository
		m_logStream << StageTag << "Loading..." << std::endl;

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
				m_logStream << ErrorTag << "Unable to clear the repository because of: " << exception.what() << std::endl;
				return false;
			}

			if (!SubversionLoad())
			{
				m_logStream << ErrorTag << "Failed to load the repository using subversion." << std::endl;
				return false;
			}

			m_hasUpdates = true;

			m_logStream << SuccessTag << "Loading completed." << std::endl;

			break;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Build()
	{
		// Run specific build method
		m_logStream << StageTag << "Building..." << std::endl;

		switch (m_buildMethod)
		{
		case BuildMethod::Qmake:
			if (!QmakeRun())
			{
				m_logStream << ErrorTag << "Failed to build the repository using qmake." << std::endl;
				return false;
			}
			if (!MakeRun())
			{
				m_logStream << ErrorTag << "Failed to build the repository using make." << std::endl;
				return false;
			}
			m_logStream << SuccessTag << "Building completed." << std::endl;
			break;

		case BuildMethod::Xbuild:
			if (!XbuildRun())
			{
				m_logStream << ErrorTag << "Failed to build the repository using xbuild." << std::endl;
				return false;
			}
			m_logStream << SuccessTag << "Building completed." << std::endl;
			break;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Deploy()
	{
		m_logStream << StageTag << "Deploying..." << std::endl;

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::OpenLogStream()
	{
		// Build log path
		boost::filesystem::path logPath;

		logPath /= LogsFolder;
		logPath /= m_deployPath.filename();

		// Try extract last build status before the log will be overwritten
		CheckLastAttemptStatus(logPath.c_str());

		// Truncate log file and open for writing
		if (!m_logStream.is_open())
		{
			m_logStream.open(logPath.c_str(), std::ios_base::out | std::ios_base::trunc);
		}

		return m_logStream.is_open();
	}

	//-------------------------------------------------------------------------------------------------
	void Repository::CheckLastAttemptStatus(const char* logPath)
	{
		FILE* logFile = fopen(logPath, "r");

		if (!logFile)
		{
			m_lastAttemptFailed = true;
			return;
		}

		// Read subversion output
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, logFile) != -1)
		{
			if (boost::istarts_with(lineBuffer, "error"))
			{
				m_lastAttemptFailed = true;
				break;
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
			m_logStream << ErrorTag << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = false;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "svn"))
			{
				m_logStream << ErrorTag << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "revision"))
			{
				std::string revisionStr(lineBuffer + 8u);
				boost::trim_if(revisionStr, boost::is_any_of("\r\n\t :."));

				revisionValue = atoll(revisionStr.c_str());

				successFlag = true;
			}
		}

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		// Wait for the running process and analyze exit code
		pclose(processPipe);

		m_logStream.flush();

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
			m_logStream << ErrorTag << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = false;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "svn"))
			{
				m_logStream << ErrorTag << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "updating") ||
				boost::istarts_with(lineBuffer, "fetching") ||
				boost::istarts_with(lineBuffer, "external"))
			{
				m_logStream << DetailTag << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "at revision"))
			{
				std::string revisionStr(lineBuffer + 11u);
				boost::trim_if(revisionStr, boost::is_any_of("\r\n\t :."));

				revisionValue = atoll(revisionStr.c_str());

				successFlag = true;

				m_logStream << DetailTag << lineBuffer;
			}
		}

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		// Wait for the running process and analyze exit code
		pclose(processPipe);

		m_logStream.flush();

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
			m_logStream << ErrorTag << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = false;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "svn"))
			{
				m_logStream << ErrorTag << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "fetching"))
			{
				m_logStream << DetailTag << lineBuffer;
			}
			else if (boost::istarts_with(lineBuffer, "checked out"))
			{
				m_logStream << DetailTag << lineBuffer;

				if (!boost::ifind_first(lineBuffer, "external"))
				{
					successFlag = true;
				}
			}
		}

		if (lineBuffer)
		{
			free(lineBuffer);
		}

		// Wait for the running process and analyze exit code
		pclose(processPipe);

		m_logStream.flush();

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::QmakeRun()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "cd ";
		commandLine += m_localPath.string();
		commandLine += " && ";
		commandLine += "qmake -makefile -nocache -o Makefile -Wall CONFIG+=";
		commandLine += m_buildConfiguration;
		commandLine += ' ';
		commandLine += m_buildScript;
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << ErrorTag << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = true;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			m_logStream << ErrorTag << lineBuffer;

			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "qmake"))
			{
				successFlag = false;
				m_logStream << ErrorTag << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "warning"))
			{
//				m_logStream << WarningTag << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "error"))
			{
//				m_logStream << ErrorTag << lineBuffer;
			}
		}

		if (lineBuffer)
		{
			free(lineBuffer);
		}

		// Wait for the running process and analyze exit code
		const auto exitCode = pclose(processPipe);

		if (exitCode)
		{
			successFlag = false;
			m_logStream << ErrorTag << "qmake exited with " << WEXITSTATUS(exitCode) << std::endl;
		}

		m_logStream.flush();

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::MakeRun()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "cd ";
		commandLine += m_localPath.string();
		commandLine += " && ";
		commandLine += "make ";
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << ErrorTag << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = true;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "make"))
			{
				successFlag = false;

				m_logStream << ErrorTag << lineBuffer;
			}
		}

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		// Wait for the running process and analyze exit code
		const auto exitCode = pclose(processPipe);

		if (exitCode)
		{
			successFlag = false;
			m_logStream << ErrorTag << "make exited with " << WEXITSTATUS(exitCode) << std::endl;
		}

		m_logStream.flush();

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::XbuildRun()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "cd ";
		commandLine += m_localPath.string();
		commandLine += " && ";
		commandLine += "xbuild /nologo /verbosity:minimal /target:rebuild /property:configuration=";
		commandLine += m_buildConfiguration;
		commandLine += " /property:platform=";
		commandLine += m_buildPlatform;
		commandLine += ' ';
		commandLine += m_buildScript;
		commandLine += " 2>&1";

		boost::replace_all(commandLine, "\'", "\\\'");
		boost::replace_all(commandLine, "\"", "\\\"");

		// Run the command
		FILE* processPipe = popen(commandLine.c_str(), "r");

		if (!processPipe)
		{
			m_logStream << ErrorTag << "Failed to run command: " << commandLine << std::endl;
			assert(false);
			return false;
		}

		// Read subversion output
		bool successFlag = true;
		char* lineBuffer = reinterpret_cast<char*>(malloc(512));
		size_t lineLength = 512;

		while (getline(&lineBuffer, &lineLength, processPipe) != -1)
		{
			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "xbuild") ||
				boost::istarts_with(lineBuffer, "msbuild"))
			{
				successFlag = false;

				m_logStream << ErrorTag << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "warning"))
			{
				m_logStream << WarningTag << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "error"))
			{
				successFlag = false;

				m_logStream << ErrorTag << lineBuffer;
			}
		}

		// Free resources
		if (lineBuffer)
		{
			free(lineBuffer);
		}

		// Wait for the running process and analyze exit code
		const auto exitCode = pclose(processPipe);

		if (exitCode)
		{
			successFlag = false;
			m_logStream << ErrorTag << "xbuild exited with " << WEXITSTATUS(exitCode) << std::endl;
		}

		m_logStream.flush();

		return successFlag;
	}
}
