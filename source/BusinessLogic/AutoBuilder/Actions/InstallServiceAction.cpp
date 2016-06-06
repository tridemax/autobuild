#include "platform.h"
#include "InstallServiceAction.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	InstallServiceAction::InstallServiceAction(SharedState& sharedState) : SharedStateRef(sharedState)
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallServiceAction::LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportInvalidProperty, ExceptionReporter reportNoProperty)
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallServiceAction::Run()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool InstallServiceAction::IsInstallAction() const
	{
		return true;
	}
}
