#include "platform.h"
#include "BuildAction.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	BuildAction::BuildAction(SharedState& sharedState) : SharedStateRef(sharedState)
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool BuildAction::LoadConfiguration(const boost::property_tree::ptree& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty)
	{
		// Assume all properties are valid
		bool successFlag = true;

		try
		{
			// Load properties
			m_buildMethod = BuildMethodStringifier::FromString(actionConfig.get<std::string>("buildMethod"));
			m_buildScript = actionConfig.get<std::string>("buildScript");
			m_buildConfiguration = actionConfig.get<std::string>("buildConfiguration");
			m_buildPlatform = actionConfig.get<std::string>("buildPlatform");

			// Check properties
			if (m_buildMethod == BuildMethod::Unknown)
			{
				successFlag = false;
				reportInvalidProperty("buildMethod");
			}
		}
		catch (std::exception& exception)
		{
			successFlag = false;
			reportException(exception.what());
		}

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool BuildAction::Run()
	{
		// Run specific build method
		m_logStream << StageTag << "Building..." << std::endl;
		m_clock.Update();

		switch (m_buildMethod)
		{
		case BuildMethod::Make:
			if (!RunMake())
			{
				m_logStream << ErrorTag << "Failed to build the repository using make." << std::endl;
				return false;
			}
			break;

		case BuildMethod::Qmake:
			if (!RunQmake())
			{
				m_logStream << ErrorTag << "Failed to build the repository using qmake." << std::endl;
				return false;
			}
			break;

		case BuildMethod::Xbuild:
			if (!RunXbuild())
			{
				m_logStream << ErrorTag << "Failed to build the repository using xbuild." << std::endl;
				return false;
			}
			break;
		}

		m_logStream << SuccessTag << "Building completed (" << m_clock.DeltaMilliseconds() << "ms)." << std::endl;

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool BuildAction::IsInstallAction() const
	{
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	bool BuildAction::RunMake()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "cd ";
		commandLine += m_cacheFolder.string();
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
	bool BuildAction::RunQmake()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "cd ";
		commandLine += m_cacheFolder.string();
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
			if (boost::istarts_with(lineBuffer, "sh") ||
				boost::istarts_with(lineBuffer, "qmake"))
			{
				successFlag = false;
				m_logStream << ErrorTag << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "message"))
			{
				m_logStream << DetailTag << lineBuffer;
			}
			else if (boost::ifind_first(lineBuffer, "warning"))
			{
				m_logStream << WarningTag << lineBuffer;
			}
			else
			{
				successFlag = false;
				m_logStream << ErrorTag << lineBuffer;
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
	bool BuildAction::RunXbuild()
	{
		// Build the command line
		std::string commandLine;

		commandLine.reserve(512u);

		commandLine += "cd ";
		commandLine += m_cacheFolder.string();
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
