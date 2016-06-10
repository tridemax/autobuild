#pragma once

#include "IAction.h"
#include "../Miscellaneous.h"
#include "../SharedState.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// BuildAction
	//-------------------------------------------------------------------------------------------------
	class BuildAction : public SharedStateRef, public IAction
	{
	private:
		BuildMethod					m_buildMethod;
		std::string					m_buildScript;
		std::string					m_buildConfiguration;
		std::string					m_buildPlatform;

	public:
		BuildAction(SharedState& sharedState);

		virtual	bool LoadConfiguration(const boost::property_tree::ptree& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty) override;
		virtual bool Run() override;
		virtual bool IsInstallAction() const override;

	private:
		bool RunMake();
		bool RunQmake();
		bool RunXbuild();
	};
}
