#pragma once

#include "IAction.h"
#include "../Miscellaneous.h"
#include "../SharedState.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// InstallServiceAction
	//-------------------------------------------------------------------------------------------------
	class InstallServiceAction : public SharedStateRef, public IAction
	{
	private:
//		BuildMethod					m_buildMethod;

	public:
		InstallServiceAction(SharedState& sharedState);

		virtual	bool LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty) override;
		virtual bool Run() override;
		virtual bool IsInstallAction() const override;

	private:
	};
}
