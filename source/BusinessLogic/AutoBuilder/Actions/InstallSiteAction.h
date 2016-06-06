#pragma once

#include "IAction.h"
#include "../Miscellaneous.h"
#include "../SharedState.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// InstallSiteAction
	//-------------------------------------------------------------------------------------------------
	class InstallSiteAction : public SharedStateRef, public IAction
	{
	private:
//		BuildMethod					m_buildMethod;

	public:
		InstallSiteAction(SharedState& sharedState);

		virtual	bool LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportInvalidProperty, ExceptionReporter reportNoProperty) override;
		virtual bool Run() override;
		virtual bool IsInstallAction() const override;

	private:
	};
}
