#pragma once


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// IAction
	//-------------------------------------------------------------------------------------------------
	class IAction : public boost::noncopyable
	{
	protected:
		typedef void(* ExceptionReporter)(const char*);

	public:
		virtual	bool LoadConfiguration(const boost::property_tree::ptree::value_type& actionConfig, ExceptionReporter reportException, ExceptionReporter reportInvalidProperty) = 0;
		virtual bool Run() = 0;
		virtual bool IsInstallAction() const = 0;
	};
}
