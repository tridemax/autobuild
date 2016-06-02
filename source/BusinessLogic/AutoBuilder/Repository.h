﻿#pragma once

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
		std::string					m_buildScript;
		std::string					m_buildConfiguration;
		std::string					m_buildPlatform;
		boost::filesystem::path		m_deployPath;
		std::string					m_dependentDaemons;

		std::ofstream				m_logStream;
		bool						m_lastAttemptFailed;
		bool						m_hasUpdates;

	public:
		static const char			LogsFolder[];
		static const char			StageTag[];
		static const char			DetailTag[];
		static const char			WarningTag[];
		static const char			ErrorTag[];
		static const char			SuccessTag[];

	public:
		Repository();
		~Repository();

		bool LoadConfiguration(const boost::property_tree::ptree::value_type& repositoryConfig);
		bool Update();
		bool Build();
		bool Deploy();

		inline bool LastAttemptFailed() const
		{
			return m_lastAttemptFailed;
		}

		inline bool HasUpdates() const
		{
			return m_hasUpdates;
		}

	private:
		bool OpenLogStream();
		void CheckLastAttemptStatus(const char* logPath);
		bool SubversionGetRevision(uint64_t& revisionValue);
		bool SubversionUpdate(uint64_t& revisionValue);
		bool SubversionLoad();
		bool QmakeRun();
		bool MakeRun();
		bool XbuildRun();
	};
}
