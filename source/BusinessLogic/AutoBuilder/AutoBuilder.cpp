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
		tbb::task_group buildingTaskGroup;

		for (auto& repository : m_repositoryList)
		{
			buildingTaskGroup.run([&repository]()
			{
				if (repository->Update())
				{
					if (repository->HasUpdates() || repository->LastAttemptFailed())
					{
						repository->m_readyToPublish = repository->Build();
					}
				}
			});
		}

		buildingTaskGroup.wait();

		// Build dependencies tree
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

		// Stop all dependent daemons
		for (auto& dependentDaemon : allDependentDaemons)
		{
			std::ostringstream outputStream;

			if (!StopDaemon(dependentDaemon.first, outputStream))
			{
				for (auto* associatedRepository : dependentDaemon.second.m_associatedRepositories)
				{
//					associatedRepository->ReportIssue
				}
			}
		}

		// Publish repositories
		tbb::task_group publishingTaskGroup;

		for (auto& repository : m_repositoryList)
		{
			if (repository->m_readyToPublish)
			{
				repository->Publish();
			}
		}

		publishingTaskGroup.wait();

		// Start all dependent daemons
		for (auto& dependentDaemon : allDependentDaemons)
		{
			std::ostringstream outputStream;

			if (!StartDaemon(dependentDaemon.first, outputStream))
			{

			}
		}

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
			m_lastError = "Failed to parse the configuration.";
			return false;
		}

		// Load repositories
		for (const auto& repositoryConfig : propertyTree.get_child("repos"))
		{
			m_repositoryList.emplace_back(new RepositoryEntry());

			if (!m_repositoryList.back()->LoadConfiguration(repositoryConfig))
			{
				m_repositoryList.pop_back();
			}
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AutoBuilder::StopDaemon(const std::string& daemonName, std::ostream& logStream)
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AutoBuilder::StartDaemon(const std::string& daemonName, std::ostream& logStream)
	{
		return true;
	}
}
