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

		// Load configuration
		if (!LoadConfiguration(confPath))
		{
			assert(false);
			return false;
		}

		// Ensure logs folder exists
		try
		{
			if (!boost::filesystem::is_directory(Repository::LogsFolder))
			{
				boost::filesystem::create_directories(Repository::LogsFolder);
			}
		}
		catch (std::exception& exception)
		{
			m_lastError = exception.what();
			return false;
		}

		// Update and build repositories
		std::list<std::thread> buildingThreadList;

		for (auto& repository : m_repositoryList)
		{
			buildingThreadList.emplace_back(std::thread([&repository]()
			{
				repository->m_requiresInstallation = repository->UpdateAndBuild() && repository->RequiresInstallation();
			}));
		}

		for (auto& buildingThread : buildingThreadList)
		{
			buildingThread.join();
		}

		// Build dependencies tree
		/*
		std::map<std::string, DependentDaemon> allDependentDaemons;

		for (auto& repository : m_repositoryList)
		{
			if (repository->m_readyToPublish && repository->RequiresInstallation())
			{
				for (const auto& dependency : repository->DependentDaemons())
				{
					auto dependentDaemonIt = allDependentDaemons.find(dependency);

					if (dependentDaemonIt == allDependentDaemons.end())
					{
						dependentDaemonIt = allDependentDaemons.emplace(std::make_pair(dependency, DependentDaemon())).first;
					}

					assert(dependentDaemonIt != allDependentDaemons.end());

					dependentDaemonIt->second.m_associatedRepositories.push_back(repository.get());
				}
			}
		}
*/
		// Stop all dependent daemons
/*
		for (auto& dependentDaemon : allDependentDaemons)
		{
			std::ostringstream outputStream;

			if (!StopDaemon(dependentDaemon.first, outputStream))
			{
				for (auto* associatedRepository : dependentDaemon.second.m_associatedRepositories)
				{
				}
			}
		}
*/
		/*
		// Install repositories
		tbb::task_group installingTaskGroup;

		for (auto& repository : m_repositoryList)
		{
			if (repository->m_requiresInstallation)
			{
				installingTaskGroup.run([&repository]()
				{
					repository->Install();
				});
			}
		}

		installingTaskGroup.wait();
		*/

		// Start all dependent daemons
		/*
		for (auto& dependentDaemon : allDependentDaemons)
		{
			std::ostringstream outputStream;

			if (!StartDaemon(dependentDaemon.first, outputStream))
			{

			}
		}
*/
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

			for (const auto& repositoryConfig : propertyTree.get_child("repos"))
			{
				std::unique_ptr<RepositoryEntry> repositoryEntry(new RepositoryEntry());

				if (repositoryEntry->LoadConfiguration(repositoryConfig.second))
				{
					m_repositoryList.emplace_back(repositoryEntry.release());
				}
			}
		}
		catch (std::exception& exception)
		{
			m_lastError = exception.what();
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AutoBuilder::StopService(const std::string& serviceName, std::ostream& logStream)
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AutoBuilder::StartService(const std::string& serviceName, std::ostream& logStream)
	{
		return true;
	}
}
