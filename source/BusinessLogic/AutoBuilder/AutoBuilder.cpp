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

		// Clear all logs
		try
		{
			boost::filesystem::remove_all(Repository::LogsFolder);
			boost::filesystem::create_directories(Repository::LogsFolder);
		}
		catch (std::exception& ex)
		{
			m_lastError = ex.what();
			return false;
		}

		// Update and build repositories
		tbb::task_group taskGroup;

		for (auto& repository : m_repositoryList)
		{
			if (!repository->IsValid())
			{
				continue;
			}

			taskGroup.run([&repository]()
			{
				if (repository->Update())
				{
					if (repository->HasUpdates())
					{
						repository->Build();
					}
				}
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

		// Load repositories
		for (const auto& repositoryConfig : propertyTree.get_child("repos"))
		{
			m_repositoryList.emplace_back(new Repository());

			m_repositoryList.back()->LoadConfiguration(repositoryConfig);
		}

		return true;
	}
}
