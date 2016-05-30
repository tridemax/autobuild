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
		typedef std::vector<std::unique_ptr<Repository>> RepositoryList;

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
