#pragma once

#define EnumCompare(inputString, comparedLiteral) \
	(inputString.size() == sizeof(comparedLiteral) - 1u && memcmp(inputString.data(), comparedLiteral, sizeof(comparedLiteral) - 1u) == 0)


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	/// SourceControl
	//-------------------------------------------------------------------------------------------------
	enum class SourceControl : uint32_t
	{
		Unknown = 0u,
		Git,
		Subversion
	};

	//-------------------------------------------------------------------------------------------------
	class SourceControlStringifier
	{
	private:
		static const char			Git_String[];
		static const char			Subversion_String[];

	public:
		static inline SourceControl FromString(const std::string& sourceControl);
		static inline const char* ToString(SourceControl sourceControl);
	};

	selectany const char SourceControlStringifier::Git_String[] = "git";
	selectany const char SourceControlStringifier::Subversion_String[] = "subversion";

	//-------------------------------------------------------------------------------------------------
	inline SourceControl SourceControlStringifier::FromString(const std::string& sourceControl)
	{
		if (EnumCompare(sourceControl, Git_String))
		{
			return SourceControl::Git;
		}
		if (EnumCompare(sourceControl, Subversion_String))
		{
			return SourceControl::Subversion;
		}
		return SourceControl::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	inline const char* SourceControlStringifier::ToString(SourceControl sourceControl)
	{
		switch (sourceControl)
		{
		case SourceControl::Git:
			return Git_String;

		case SourceControl::Subversion:
			return Subversion_String;
		}
		return "";
	}

	//-------------------------------------------------------------------------------------------------
	/// BuildMethod
	//-------------------------------------------------------------------------------------------------
	enum class BuildMethod : uint32_t
	{
		Unknown = 0u,
		Qmake,
		Xbuild
	};

	//-------------------------------------------------------------------------------------------------
	class BuildMethodStringifier
	{
	private:
		static const char			Qmake_String[];
		static const char			Xbuild_String[];

	public:
		static inline BuildMethod FromString(const std::string& buildMethod);
		static inline const char* ToString(BuildMethod buildMethod);
	};

	selectany const char BuildMethodStringifier::Qmake_String[] = "qmake";
	selectany const char BuildMethodStringifier::Xbuild_String[] = "xbuild";

	//-------------------------------------------------------------------------------------------------
	inline BuildMethod BuildMethodStringifier::FromString(const std::string& buildMethod)
	{
		if (EnumCompare(buildMethod, Qmake_String))
		{
			return BuildMethod::Qmake;
		}
		if (EnumCompare(buildMethod, Xbuild_String))
		{
			return BuildMethod::Xbuild;
		}
		return BuildMethod::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	inline const char* BuildMethodStringifier::ToString(BuildMethod buildMethod)
	{
		switch (buildMethod)
		{
		case BuildMethod::Qmake:
			return Qmake_String;

		case BuildMethod::Xbuild:
			return Xbuild_String;
		}
		return "";
	}

	//-------------------------------------------------------------------------------------------------
	/// BuildStatus
	//-------------------------------------------------------------------------------------------------
	enum class BuildStatus : uint32_t
	{
		Unknown = 0u,
		Success,
		UpdateFailed,
		BuildFailed,
		DeployFailed
	};
}
