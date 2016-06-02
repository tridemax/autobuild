#pragma once

#include "Miscellaneous.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// Repository
	//-------------------------------------------------------------------------------------------------
	class Repository : public boost::noncopyable
	{
	private:
		SourceControl				m_sourceControl;
		std::string					m_sourceControlLogin;
		std::string					m_sourceControlPassword;
		std::string					m_sourceUrl;
		boost::filesystem::path		m_localPath;
		BuildMethod					m_buildMethod;
		std::string					m_projectFile;
		std::string					m_projectConfiguration;
		boost::filesystem::path		m_deployPath;
		std::string					m_dependentDaemons;

		std::ofstream				m_logStream;
		BuildStatus					m_lastBuildStatus;
		bool						m_hasUpdates;
		bool						m_wasFinalized;

	public:
		static const char			LogsFolder[];

	public:
		Repository();
		~Repository();

		bool LoadConfiguration(const boost::property_tree::ptree::value_type& repositoryConfig);
		bool Update();
		bool Build();
		bool Deploy();
		void Finalize(BuildStatus buildStatus);

		inline BuildStatus LastBuildStatus() const
		{
			return m_lastBuildStatus;
		}

		inline bool HasUpdates() const
		{
			return m_hasUpdates;
		}

		inline bool WasFinalized() const
		{
			return m_wasFinalized;
		}

	private:
		bool OpenLogStream();
		void ReadLastBuildStatus(const char* logPath);
		bool SubversionGetRevision(uint64_t& revisionValue);
		bool SubversionUpdate(uint64_t& revisionValue);
		bool SubversionLoad();
		bool QmakeRun();
		bool XbuildRun();
	};
}
