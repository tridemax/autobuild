#pragma once

#include "File.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// Folder
	//-------------------------------------------------------------------------------------------------
	class Folder : public boost::noncopyable
	{
	private:
		typedef boost::unordered_map<uint64_t, Folder, aux::DummyHash<uint64_t> > FolderMap;
		typedef boost::unordered_map<uint64_t, File, aux::DummyHash<uint64_t> > FileMap;

	private:
		Folder*						m_parent;
		std::string					m_name;
		FolderMap					m_folderMap;
		FileMap						m_fileMap;

	public:
		Folder();
		Folder(Folder* parent, const char* name);
		Folder(Folder&& folder);

		File* InsertFile(const char* name);
		Folder* InsertFolder(const char* name);
		Folder* EnsureFolder(const char* name);
		void ScanFileSystemRecursively(const boost::filesystem::path& localPath, const char* name);

	private:
		void ScanFileSystemInternal(const boost::filesystem::path& folderPath);
	};
}
