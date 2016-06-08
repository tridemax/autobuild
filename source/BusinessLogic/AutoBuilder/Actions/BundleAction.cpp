#include "platform.h"
#include "BundleAction.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	BundleAction::BundleAction(SharedState& sharedState) : SharedStateRef(sharedState)
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool BundleAction::LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty)
	{
		// Assume all properties are valid
		bool successFlag = true;

		try
		{
			// Load items
			for (const auto& itemConfig : actionConfig.second.get_child("items"))
			{
				m_itemList.emplace_back(itemConfig.second.data());
			}
		}
		catch (std::exception& exception)
		{
			successFlag = false;
			reportException(exception.what());
		}

		return successFlag;
	}

	//-------------------------------------------------------------------------------------------------
	bool BundleAction::Run()
	{
		// Collecting the bundle
		m_logStream << StageTag << "Bundling..." << std::endl;
		m_clock.Update();

		try
		{
			for (const auto& item : m_itemList)
			{
				m_bundle.AddItem(m_localPath, item);
			}
		}
		catch (std::exception& exception)
		{
			m_logStream << ErrorTag << "Failed to collect the bundle because of: " << exception.what() << std::endl;
			return false;
		}

		m_logStream << SuccessTag << "Bundling completed (" << m_clock.DeltaMilliseconds() << "ms)." << std::endl;

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool BundleAction::IsInstallAction() const
	{
		return false;
	}
}
