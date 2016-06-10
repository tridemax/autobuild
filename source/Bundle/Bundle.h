#pragma once

#include "Folder.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// Bundle
	//-------------------------------------------------------------------------------------------------
	class Bundle : public boost::noncopyable
	{
	public:
//		typedef boost::unordered_map<uint64_t, File*, uint64_t> FileMap;

	public:
		Folder						m_rootFolder;
//		FileMap						m_fileMap;

	public:
		Bundle();

		void AddItem(const boost::filesystem::path& localPath, const boost::filesystem::path& itemPath);
		void Copy(const boost::filesystem::path& localPath, const boost::filesystem::path& destPath);
	};
}
