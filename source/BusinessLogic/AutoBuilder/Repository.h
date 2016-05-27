#pragma once


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// Repository
	//-------------------------------------------------------------------------------------------------
	class Repository
	{
	private:
		friend class AutoBuilder;

	private:
		bool						m_hasConfig;
		std::string					m_sourceControl;
		std::string					m_sourceControlLogin;
		std::string					m_sourceControlPassword;
		std::string					m_sourceUrl;
		std::string					m_localPath;
		std::string					m_projectFile;
		std::string					m_projectConfiguration;
		std::string					m_deployPath;
		std::string					m_dependentDaemons;

		static const char			LogPath[];

	public:
		Repository();
		~Repository();

		bool Update();
		bool Build();
		bool Deploy();

	private:
	};
}
