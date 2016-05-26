#pragma once

//#include "../../Auxiliary/IStream.h"


namespace AutoBuild
{
//	using namespace Auxiliary;

	//-------------------------------------------------------------------------------------------------
	/// AutoBuilder
	//-------------------------------------------------------------------------------------------------
	class AutoBuilder : public boost::noncopyable
	{
	private:
		std::string					m_lastError;

	public:
		AutoBuilder();
		~AutoBuilder();

		bool Run(boost::program_options& programOptions);
		const std::string& LastError();

	private:
	};
}
