#include "platform.h"
#include "InstallSiteAction.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	InstallSiteAction::InstallSiteAction(SharedState& sharedState) : SharedStateRef(sharedState)
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallSiteAction::LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportInvalidProperty, ExceptionReporter reportNoProperty)
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
