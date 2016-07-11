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
		boost::filesystem::path		m_logFolder;
		boost::filesystem::path		m_cacheFolder;
		Bundle						m_bundle;
		aux::Clock					m_clock;
		std::ofstream				m_logStream;
	};

	//-------------------------------------------------------------------------------------------------
	/// SharedStateRef
	//-------------------------------------------------------------------------------------------------
	class SharedStateRef
	{
	public:
		boost::filesystem::path&	m_logFolder;
		boost::filesystem::path&	m_cacheFolder;
		Bundle&						m_bundle;
		aux::Clock&					m_clock;
		std::ofstream&				m_logStream;

	public:
		inline SharedStateRef(SharedState& sharedState) :
			m_logFolder(sharedState.m_logFolder),
			m_cacheFolder(sharedState.m_cacheFolder),
			m_bundle(sharedState.m_bundle),
			m_clock(sharedState.m_clock),
			m_logStream(sharedState.m_logStream)
		{
		}
	};
}
