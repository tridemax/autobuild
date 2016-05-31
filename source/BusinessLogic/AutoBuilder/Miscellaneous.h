#pragma once


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// IdVector
	//-------------------------------------------------------------------------------------------------
	typedef std::vector<uint64_t> IdVector;

	//-------------------------------------------------------------------------------------------------
	/// SourceControl
	//-------------------------------------------------------------------------------------------------
	enum class SourceControl : uint32_t
	{
		Unknown = 0u,
		Subversion,
		Git
	};

	//-------------------------------------------------------------------------------------------------
	/// SourceControlStringifier
	//-------------------------------------------------------------------------------------------------
	class SourceControlStringifier
	{
	public:
		static const char			Subversion_String[];
		static const char			Git_String[];

	public:
		static inline SourceControl FromString(const std::string& sourceControl);
		static inline const char* ToString(SourceControl sourceControl);
	};

	//-------------------------------------------------------------------------------------------------
	selectany const char SourceControlStringifier::Subversion_String[] = "subversion";
	selectany const char SourceControlStringifier::Git_String[] = "git";

	//-------------------------------------------------------------------------------------------------
	inline SourceControl SourceControlStringifier::FromString(const std::string& sourceControl)
	{
		if (memcmp(sourceControl.data(), Subversion_String, std::min(sourceControl.size(), sizeof(Subversion_String) - 1u)) == 0)
		{
			return SourceControl::Subversion;
		}
		if (memcmp(sourceControl.data(), Git_String, std::min(sourceControl.size(), sizeof(Git_String) - 1u)) == 0)
		{
			return SourceControl::Git;
		}
		return SourceControl::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	inline const char* SourceControlStringifier::ToString(SourceControl sourceControl)
	{
		switch (sourceControl)
		{
		case SourceControl::Subversion:
			return Subversion_String;

		case SourceControl::Git:
			return Git_String;
		}
		return "";
	}

	//-------------------------------------------------------------------------------------------------
	/// BuildMethod
	//-------------------------------------------------------------------------------------------------
	enum class BuildMethod : uint32_t
	{
		Unknown = 0u,
		Xbuild,
		Qmake
	};

	//-------------------------------------------------------------------------------------------------
	/// BuildMethodStringifier
	//-------------------------------------------------------------------------------------------------
	class BuildMethodStringifier
	{
	public:
		static const char			Xbuild_String[];
		static const char			Qmake_String[];

	public:
		static inline BuildMethod FromString(const std::string& buildMethod);
		static inline const char* ToString(BuildMethod buildMethod);
	};

	//-------------------------------------------------------------------------------------------------
	selectany const char BuildMethodStringifier::Xbuild_String[] = "xbuild";
	selectany const char BuildMethodStringifier::Qmake_String[] = "qmake";

	//-------------------------------------------------------------------------------------------------
	inline BuildMethod BuildMethodStringifier::FromString(const std::string& buildMethod)
	{
		if (memcmp(buildMethod.data(), Xbuild_String, std::min(buildMethod.size(), sizeof(Xbuild_String) - 1u)) == 0)
		{
			return BuildMethod::Xbuild;
		}
		if (memcmp(buildMethod.data(), Qmake_String, std::min(buildMethod.size(), sizeof(Qmake_String) - 1u)) == 0)
		{
			return BuildMethod::Qmake;
		}
		return BuildMethod::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	inline const char* BuildMethodStringifier::ToString(BuildMethod buildMethod)
	{
		switch (buildMethod)
		{
		case BuildMethod::Xbuild:
			return Xbuild_String;

		case BuildMethod::Qmake:
			return Qmake_String;
		}
		return "";
	}
}
