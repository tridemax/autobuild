﻿#include "platform.h"
#include "Repository.h"


namespace AutoBuild
{
	const char Repository::LogsFolder[] = "/var/log/autobuild-logs";

	//-------------------------------------------------------------------------------------------------
	Repository::Repository() : m_configIsValid(false), m_hasUpdates(false)
	{
	}

	//-------------------------------------------------------------------------------------------------
	Repository::~Repository()
	{
	}

	//-------------------------------------------------------------------------------------------------
	void Repository::LoadConfiguration(const boost::property_tree::ptree::value_type& repositoryConfig)
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
		m_configIsValid = true;

		// Try read 'sourceControl' property
		try
		{
			m_sourceControl = SourceControlStringifier::FromString(repositoryConfig.second.get<std::string>("sourceControl"));

			if (m_sourceControl == SourceControl::Unknown)
			{
				m_configIsValid = false;
				reportInvalidProperty("sourceControl");
			}
		}
		catch (...)
		{
			m_configIsValid = false;
			reportNoProperty("sourceControl");
		}

		// Try read 'sourceControlLogin' property
		try
		{
			m_sourceControlLogin = repositoryConfig.second.get<std::string>("sourceControlLogin");
		}
		catch (...)
		{
			m_configIsValid = false;
			reportNoProperty("sourceControlLogin");
		}

		// Try read 'sourceControlPassword' property
		try
		{
			m_sourceControlPassword = repositoryConfig.second.get<std::string>("sourceControlPassword");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'sourceControlPassword' property." << std::endl;
		}

		// Try read 'sourceUrl' property
		try
		{
			m_sourceUrl = repositoryConfig.second.get<std::string>("sourceUrl");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'sourceUrl' property." << std::endl;
		}

		// Try read 'localPath' property
		try
		{
			m_localPath = repositoryConfig.second.get<std::string>("localPath");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'localPath' property." << std::endl;
		}

		// Try read 'buildMethod' property
		try
		{
			m_buildMethod = BuildMethodStringifier::FromString(repositoryConfig.second.get<std::string>("buildMethod"));

			if (m_buildMethod == BuildMethod::Unknown)
			{
				m_configIsValid = false;
				reportInvalidProperty("buildMethod");
			}
		}
		catch (...)
		{
			m_configIsValid = false;
			reportNoProperty("buildMethod");
		}

		// Try read 'projectFile' property
		try
		{
			m_projectFile = repositoryConfig.second.get<std::string>("projectFile");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'projectFile' property." << std::endl;
		}

		// Try read 'projectConfiguration' property
		try
		{
			m_projectConfiguration = repositoryConfig.second.get<std::string>("projectConfiguration");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'projectConfiguration' property." << std::endl;
		}

		// Try read 'deployPath' property
		try
		{
			m_deployPath = repositoryConfig.second.get<std::string>("deployPath");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'deployPath' property." << std::endl;
		}

		// Try read 'dependentDaemons' property
		try
		{
			m_dependentDaemons = repositoryConfig.second.get<std::string>("dependentDaemons");
		}
		catch (...)
		{
			m_configIsValid = false;
			std::cout << "One of the repositories has no 'dependentDaemons' property." << std::endl;
		}
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

		case SourceControl::Git:
			break;
		}

		// Run specific source control to load a local copy of the repository
		m_logStream << "Loading..." << std::endl;

		switch (m_sourceControl)
		{
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

		case SourceControl::Git:
			break;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool Repository::Build()
	{
		// Setup logging
		if (!OpenLogStream())
		{
			assert(false);
			return false;
		}

		// Run specific build method
		m_logStream << std::endl << "Building..." << std::endl;

		switch (m_buildMethod)
		{
		case BuildMethod::Xbuild:
			break;

		case BuildMethod::Qmake:
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
	bool Repository::QmakeBuild()
	{
		return true;
	}
}
