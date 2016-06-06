#pragma once

#include "Actions/IAction.h"
#include "Miscellaneous.h"
#include "SharedState.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// Repository
	//-------------------------------------------------------------------------------------------------
	class Repository : public SharedState, public boost::noncopyable
	{
	private:
		typedef std::vector<std::unique_ptr<IAction>> ActionList;

	private:
		SourceControl				m_sourceControl;
		std::string					m_sourceControlLogin;
		std::string					m_sourceControlPassword;
		std::string					m_sourceUrl;
		ActionList					m_actionList;
		bool						m_lastAttemptFailed;
		bool						m_hasUpdates;

	public:
		static const char			LogsFolder[];

	public:
		Repository();
		~Repository();

		bool LoadConfiguration(const boost::property_tree::ptree::value_type& repositoryConfig);
		bool UpdateAndBuild();
		bool Install();
		bool RequiresInstallation() const;

	private:
		bool OpenLogStream();
		void ExtractLastAttemptStatus(const char* logPath);
		bool Update();
		bool SubversionGetRevision(uint64_t& revisionValue);
		bool SubversionUpdate(uint64_t& revisionValue);
		bool SubversionLoad();
	};
}
