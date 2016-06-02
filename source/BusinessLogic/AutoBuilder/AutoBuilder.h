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
			bool						m_attemptSuccessful;

		public:
			inline RepositoryEntry() : m_attemptSuccessful(true) {}
		};

		typedef std::vector<std::unique_ptr<RepositoryEntry>> RepositoryList;

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
	};
}
