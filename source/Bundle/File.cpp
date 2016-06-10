#include "platform.h"
#include "File.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	File::File(Folder* parent, const char* name) : m_parent(parent), m_name(name)
	{
	}

	//-------------------------------------------------------------------------------------------------
	File::File(File&& file) : m_parent(file.m_parent), m_name(boost::move(file.m_name))
	{
	}
}
