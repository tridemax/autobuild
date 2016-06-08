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
		typedef std::list<boost::filesystem::path> ItemList;

	private:
		ItemList					m_itemList;

	public:
		BundleAction(SharedState& sharedState);

		virtual	bool LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty) override;
		virtual bool Run() override;
		virtual bool IsInstallAction() const override;
	};
}
