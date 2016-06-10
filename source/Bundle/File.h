#pragma once


namespace AutoBuild
{
	class Folder;

	//-------------------------------------------------------------------------------------------------
	/// File
	//-------------------------------------------------------------------------------------------------
	class File : public boost::noncopyable
	{
	private:
		Folder*						m_parent;
		std::string					m_name;

	public:
		File(Folder* parent, const char* name);
		File(File&& file);
	};
}
