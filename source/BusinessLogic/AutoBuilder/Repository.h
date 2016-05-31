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

		bool						m_configIsValid;
		bool						m_hasUpdates;
		std::ofstream				m_logStream;

	public:
		static const char			LogsFolder[];

	public:
		Repository();
		~Repository();

		void LoadConfiguration(const boost::property_tree::ptree::value_type& repositoryConfig);
		bool Update();
		bool Build();
		bool Deploy();

		inline bool IsValid() const
		{
			return m_configIsValid;
		}

		inline bool HasUpdates() const
		{
			return m_hasUpdates;
		}

	private:
		bool OpenLogStream();
		bool SubversionUpdate();
		bool SubversionLoad();
		bool GitUpdate();
		bool GitLoad();
	};
}
