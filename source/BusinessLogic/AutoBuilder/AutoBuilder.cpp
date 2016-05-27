#include "platform.h"
#include "AutoBuilder.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	AutoBuilder::AutoBuilder()
	{
	}

	//-------------------------------------------------------------------------------------------------
	AutoBuilder::~AutoBuilder()
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool AutoBuilder::Run(const char* confPath)
	{
		m_repositoryList.clear();

		// Clear all logs
		boost::filesystem::remove_all(Repository::LogPath);
		boost::filesystem::create_directories(Repository::LogPath);

		// Load configuration
		if (!LoadConfiguration(confPath))
		{
			assert(false);
			return false;
		}

		// Update and build repositories
		tbb::task_group taskGroup;

		for (auto& repository : m_repositoryList)
		{
			if (!repository.m_hasConfig)
			{
				continue;
			}

			taskGroup.run([&repository]()
			{
				if (!repository.Update())
				{
				}

				repository.Build();
			});
		}

		taskGroup.wait();

		// Deploy repositories

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	const std::string& AutoBuilder::LastError()
	{
		return m_lastError;
	}

	//-------------------------------------------------------------------------------------------------
	bool AutoBuilder::LoadConfiguration(const char* confPath)
	{
		// Load configuration
		boost::property_tree::ptree propertyTree;

		try
		{
			boost::property_tree::read_json(confPath, propertyTree);
		}
		catch (...)
		{
			m_lastError = "Unable to parse the configuration.";
			return false;
		}

		for (const auto& repositoryConfig : propertyTree.get_child("repos"))
		{
			m_repositoryList.emplace_back(Repository());

			Repository& repository = m_repositoryList.back();
			repository.m_hasConfig = true;

			// Try read 'sourceControl' property
			try
			{
				repository.m_sourceControl = repositoryConfig.second.get<std::string>("sourceControl");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'sourceControl' property." << std::endl;
			}

			// Try read 'sourceControlLogin' property
			try
			{
				repository.m_sourceControlLogin = repositoryConfig.second.get<std::string>("sourceControlLogin");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'sourceControlLogin' property." << std::endl;
			}

			// Try read 'sourceControlPassword' property
			try
			{
				repository.m_sourceControlPassword = repositoryConfig.second.get<std::string>("sourceControlPassword");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'sourceControlPassword' property." << std::endl;
			}

			// Try read 'sourceUrl' property
			try
			{
				repository.m_sourceUrl = repositoryConfig.second.get<std::string>("sourceUrl");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'sourceUrl' property." << std::endl;
			}

			// Try read 'localPath' property
			try
			{
				repository.m_localPath = repositoryConfig.second.get<std::string>("localPath");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'localPath' property." << std::endl;
			}

			// Try read 'projectFile' property
			try
			{
				repository.m_projectFile = repositoryConfig.second.get<std::string>("projectFile");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'projectFile' property." << std::endl;
			}

			// Try read 'projectConfiguration' property
			try
			{
				repository.m_projectConfiguration = repositoryConfig.second.get<std::string>("projectConfiguration");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'projectConfiguration' property." << std::endl;
			}

			// Try read 'deployPath' property
			try
			{
				repository.m_deployPath = repositoryConfig.second.get<std::string>("deployPath");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'deployPath' property." << std::endl;
			}

			// Try read 'dependentDaemons' property
			try
			{
				repository.m_dependentDaemons = repositoryConfig.second.get<std::string>("dependentDaemons");
			}
			catch (...)
			{
				repository.m_hasConfig = false;
				std::cout << "One of the repositories has no 'dependentDaemons' property." << std::endl;
			}
		}

		return true;
	}
}
