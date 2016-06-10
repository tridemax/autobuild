#include "platform.h"
#include "Bundle.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	Bundle::Bundle()
	{
	}

	//-------------------------------------------------------------------------------------------------
	void Bundle::AddItem(const boost::filesystem::path& localPath, const boost::filesystem::path& itemPath)
	{
		Folder* folder = &m_rootFolder;

		boost::filesystem::path parentPath = itemPath.parent_path();

		if (parentPath.empty())
		{
			folder->ScanFileSystemRecursively(localPath, itemPath.filename().c_str());
		}
		else
		{
			for (auto pathComponentIt = parentPath.begin(); pathComponentIt != parentPath.end(); ++pathComponentIt)
			{
				folder = folder->EnsureFolder(pathComponentIt->c_str());
			}

			folder->ScanFileSystemRecursively(localPath / parentPath, itemPath.filename().c_str());
		}
	}

	//-------------------------------------------------------------------------------------------------
	void Bundle::Copy(const boost::filesystem::path& localPath, const boost::filesystem::path& destPath)
	{
	}
}
