#include "platform.h"
#include "Folder.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	Folder::Folder() : m_parent(nullptr)
	{
	}

	//-------------------------------------------------------------------------------------------------
	Folder::Folder(Folder* parent, const char* name) : m_parent(nullptr), m_name(name)
	{
	}

	//-------------------------------------------------------------------------------------------------
	Folder::Folder(Folder&& folder): m_parent(folder.m_parent), m_name(boost::move(folder.m_name))
	{
	}

	//-------------------------------------------------------------------------------------------------
	File* Folder::InsertFile(const char* name)
	{
		const uint64_t nameHash = Aux::Hash64(name, strlen(name));

		if (m_fileMap.find(nameHash) != m_fileMap.end())
		{
			throw std::runtime_error(std::string("file with name '") + name + "' already exists");
		}

		if (m_folderMap.find(nameHash) != m_folderMap.end())
		{
			throw std::runtime_error(std::string("folder with name '") + name + "' already exists");
		}

		auto insertionResult = m_fileMap.emplace(nameHash, File(this, name));
		assert(insertionResult.second);

		return &insertionResult.first->second;
	}

	//-------------------------------------------------------------------------------------------------
	Folder* Folder::InsertFolder(const char* name)
	{
		const uint64_t nameHash = Aux::Hash64(name, strlen(name));

		// Check for files with same name
		if (m_fileMap.find(nameHash) != m_fileMap.end())
		{
			throw std::runtime_error(std::string("file with name '") + name + "' already exists");
		}

		// Check for folders with same name
		if (m_folderMap.find(nameHash) != m_folderMap.end())
		{
			throw std::runtime_error(std::string("folder with name '") + name + "' already exists");
		}

		// Insert the folder
		auto insertionResult = m_folderMap.emplace(nameHash, Folder(this, name));
		assert(insertionResult.second);

		return &insertionResult.first->second;
	}

	//-------------------------------------------------------------------------------------------------
	Folder* Folder::EnsureFolder(const char* name)
	{
		const uint64_t nameHash = Aux::Hash64(name, strlen(name));

		auto folderIt = m_folderMap.find(nameHash);

		if (folderIt == m_folderMap.end())
		{
			// Check for files with same name
			if (m_fileMap.find(nameHash) != m_fileMap.end())
			{
				throw std::runtime_error(std::string("file with name '") + name + "' already exists");
			}

			// Insert the folder
			folderIt = m_folderMap.emplace(nameHash, Folder(this, name)).first;
		}

		return &folderIt->second;
	}

	//-------------------------------------------------------------------------------------------------
	void Folder::ScanFileSystemRecursively(const boost::filesystem::path& localPath, const char* name)
	{
		const boost::filesystem::path itemPath = localPath / name;
		struct stat itemStat;

		if (lstat(itemPath.c_str(), &itemStat) == -1)
		{
			throw std::runtime_error(std::string("unable to stat file '") + name + "' at " + localPath.c_str());
		}

		// Process only regular files and folders, skip all other ones
		if (S_ISREG(itemStat.st_mode))
		{
			InsertFile(name);
		}
		else if (S_ISDIR(itemStat.st_mode))
		{
			InsertFolder(name)->ScanFileSystemInternal(itemPath);
		}
	}

	//-------------------------------------------------------------------------------------------------
	void Folder::ScanFileSystemInternal(const boost::filesystem::path& localPath)
	{
		Aux::DirectoryStream dirStream(localPath.c_str());

		if (!dirStream)
		{
			throw std::runtime_error(std::string("unable to open folder '") + m_name + "' at " + localPath.c_str());
		}

		// Scan this directory
		dirent* dirEntry = nullptr;

		while (dirEntry = readdir(dirStream))
		{
			// Skip folders like '.', '..'
			if (dirEntry->d_name[0] == '.')
			{
				if (dirEntry->d_name[1] == '\0' || (dirEntry->d_name[1] == '.' && dirEntry->d_name[2] == '\0'))
				{
					continue;
				}
			}

			// Process only regular files and folders, skip all other ones
			if (dirEntry->d_type & DT_REG)
			{
				const uint64_t nameHash = Aux::Hash64(dirEntry->d_name, strlen(dirEntry->d_name));

				auto fileIt = m_fileMap.find(nameHash);

				if (fileIt == m_fileMap.end())
				{
					fileIt = m_fileMap.emplace(nameHash, File(this, dirEntry->d_name)).first;
				}
			}
			else if (dirEntry->d_type & DT_DIR)
			{
				const uint64_t nameHash = Aux::Hash64(dirEntry->d_name, strlen(dirEntry->d_name));

				auto folderIt = m_folderMap.find(nameHash);

				if (folderIt == m_folderMap.end())
				{
					folderIt = m_folderMap.emplace(nameHash, Folder(this, dirEntry->d_name)).first;
				}

				folderIt->second.ScanFileSystemInternal(localPath / dirEntry->d_name);
			}
		}
	}
}
