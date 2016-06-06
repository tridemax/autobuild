#include "platform.h"
#include "BundleAction.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	BundleAction::BundleAction(SharedState& sharedState) : SharedStateRef(sharedState)
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool BundleAction::LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportInvalidProperty, ExceptionReporter reportNoProperty)
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool BundleAction::Run()
	{
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool BundleAction::IsInstallAction() const
	{
		return false;
	}
}
