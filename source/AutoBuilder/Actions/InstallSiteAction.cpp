#include "platform.h"
#include "InstallSiteAction.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	InstallSiteAction::InstallSiteAction(SharedState& sharedState) : SharedStateRef(sharedState)
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallSiteAction::LoadConfiguration(const boost::property_tree::ptree& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty)
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallSiteAction::Run()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallSiteAction::IsInstallAction() const
	{
		return true;
	}
}
