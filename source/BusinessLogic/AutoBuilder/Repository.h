#pragma once


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// Repository
	//-------------------------------------------------------------------------------------------------
	class Repository : public boost::noncopyable
	{
	private:
		friend class AutoBuilder;

	private:
		std::string					m_sourceControl;
		std::string					m_sourceControlLogin;
		std::string					m_sourceControlPassword;
		std::string					m_sourceUrl;
		boost::filesystem::path		m_localPath;
		std::string					m_projectFile;
		std::string					m_projectConfiguration;
		boost::filesystem::path		m_deployPath;
		std::string					m_dependentDaemons;

		bool						m_hasConfig;

		std::ofstream				m_logStream;

		static const char			LogsFolder[];

	public:
		Repository();
		~Repository();

		bool Update();
		bool Build();
		bool Deploy();

	private:
		bool OpenLogStream();
	};
}
