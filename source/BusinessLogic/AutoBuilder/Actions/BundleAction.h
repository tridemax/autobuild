#pragma once

#include "IAction.h"
#include "../Miscellaneous.h"
#include "../SharedState.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// BundleAction
	//-------------------------------------------------------------------------------------------------
	class BundleAction : public SharedStateRef, public IAction
	{
	private:
//		BuildMethod					m_buildMethod;

	public:
		BundleAction(SharedState& sharedState);

		virtual	bool LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportNoProperty, ExceptionReporter reportInvalidProperty) override;
		virtual bool Run() override;
		virtual bool IsInstallAction() const override;
	};
}
