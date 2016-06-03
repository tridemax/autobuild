#pragma once

#include "Repository.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// AutoBuilder
	//-------------------------------------------------------------------------------------------------
	class AutoBuilder : public boost::noncopyable
	{
	private:
		class RepositoryEntry : public Repository
		{
		public:
			bool						m_readyToPublish;

		public:
			inline RepositoryEntry() : m_readyToPublish(false) {}
		};

		typedef std::vector<std::unique_ptr<RepositoryEntry>> RepositoryList;

		class DependentDaemon
		{
		public:
			typedef std::vector<RepositoryEntry*> AssociatedRepositories;

		public:
			AssociatedRepositories		m_associatedRepositories;
			bool						m_stopped;
			bool						m_started;

		public:
			inline DependentDaemon() : m_stopped(false), m_started(false) {}
		};

	private:
		RepositoryList				m_repositoryList;
		std::string					m_lastError;

	public:
		AutoBuilder();
		~AutoBuilder();

		bool Run(const char* confPath);
		const std::string& LastError();

	private:
		bool LoadConfiguration(const char* confPath);
		bool StopDaemon(const std::string& daemonName, std::ostream& logStream);
		bool StartDaemon(const std::string& daemonName, std::ostream& logStream);
	};
}
