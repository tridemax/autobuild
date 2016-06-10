#pragma once

#include "../Bundle/Bundle.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// SharedState
	//-------------------------------------------------------------------------------------------------
	class SharedState
	{
	public:
		boost::filesystem::path		m_localPath;
		Bundle						m_bundle;
		Aux::Clock					m_clock;
		std::ofstream				m_logStream;
	};

	//-------------------------------------------------------------------------------------------------
	/// SharedStateRef
	//-------------------------------------------------------------------------------------------------
	class SharedStateRef
	{
	public:
		boost::filesystem::path&	m_localPath;
		Bundle&						m_bundle;
		Aux::Clock&					m_clock;
		std::ofstream&				m_logStream;

	public:
		inline SharedStateRef(SharedState& sharedState) :
			m_localPath(sharedState.m_localPath),
			m_bundle(sharedState.m_bundle),
			m_clock(sharedState.m_clock),
			m_logStream(sharedState.m_logStream)
		{
		}
	};
}
