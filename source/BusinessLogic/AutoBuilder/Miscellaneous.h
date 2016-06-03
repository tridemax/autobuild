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
	/// PublishMethod
	//-------------------------------------------------------------------------------------------------
	enum class PublishMethod : uint32_t
	{
		Unknown = 0u,
		Install,
		Upload,
		Both
	};

	//-------------------------------------------------------------------------------------------------
	class PublishMethodStringifier
	{
	private:
		static const char			Install_String[];
		static const char			Upload_String[];
		static const char			Both_String[];

	public:
		static inline PublishMethod FromString(const std::string& publishMethod);
		static inline const char* ToString(PublishMethod publishMethod);
	};

	selectany const char PublishMethodStringifier::Install_String[] = "install";
	selectany const char PublishMethodStringifier::Upload_String[] = "upload";
	selectany const char PublishMethodStringifier::Both_String[] = "both";

	//-------------------------------------------------------------------------------------------------
	inline PublishMethod PublishMethodStringifier::FromString(const std::string& publishMethod)
	{
		if (EnumCompare(publishMethod, Install_String))
		{
			return PublishMethod::Install;
		}
		if (EnumCompare(publishMethod, Upload_String))
		{
			return PublishMethod::Upload;
		}
		if (EnumCompare(publishMethod, Both_String))
		{
			return PublishMethod::Both;
		}
		return PublishMethod::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	inline const char* PublishMethodStringifier::ToString(PublishMethod publishMethod)
	{
		switch (publishMethod)
		{
		case PublishMethod::Install:
			return Install_String;

		case PublishMethod::Upload:
			return Upload_String;

		case PublishMethod::Both:
			return Both_String;
		}
		return "";
	}
}
