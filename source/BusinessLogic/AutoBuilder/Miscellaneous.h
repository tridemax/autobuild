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
		static const char			Git[];
		static const char			Subversion[];

	public:
		static inline SourceControl FromString(const std::string& sourceControl);
	};

	selectany const char SourceControlStringifier::Git[] = "git";
	selectany const char SourceControlStringifier::Subversion[] = "subversion";

	//-------------------------------------------------------------------------------------------------
	inline SourceControl SourceControlStringifier::FromString(const std::string& sourceControl)
	{
		if (EnumCompare(sourceControl, Git))
		{
			return SourceControl::Git;
		}
		if (EnumCompare(sourceControl, Subversion))
		{
			return SourceControl::Subversion;
		}
		return SourceControl::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	/// ActionKind
	//-------------------------------------------------------------------------------------------------
	enum class ActionKind : uint32_t
	{
		Unknown = 0u,
		Build,
		Bundle,
		Upload,
		InstallSite,
		InstallService
	};

	//-------------------------------------------------------------------------------------------------
	class ActionKindStringifier
	{
	private:
		static const char			Build[];
		static const char			Bundle[];
		static const char			Upload[];
		static const char			InstallSite[];
		static const char			InstallService[];

	public:
		static inline ActionKind FromString(const std::string& actionKind);
	};

	selectany const char ActionKindStringifier::Build[] = "build";
	selectany const char ActionKindStringifier::Bundle[] = "bundle";
	selectany const char ActionKindStringifier::Upload[] = "upload";
	selectany const char ActionKindStringifier::InstallSite[] = "install-site";
	selectany const char ActionKindStringifier::InstallService[] = "install-service";

	//-------------------------------------------------------------------------------------------------
	inline ActionKind ActionKindStringifier::FromString(const std::string& actionKind)
	{
		if (EnumCompare(actionKind, Build))
		{
			return ActionKind::Build;
		}
		if (EnumCompare(actionKind, Bundle))
		{
			return ActionKind::Bundle;
		}
		if (EnumCompare(actionKind, Upload))
		{
			return ActionKind::Upload;
		}
		if (EnumCompare(actionKind, InstallSite))
		{
			return ActionKind::InstallSite;
		}
		if (EnumCompare(actionKind, InstallService))
		{
			return ActionKind::InstallService;
		}
		return ActionKind::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	/// BuildMethod
	//-------------------------------------------------------------------------------------------------
	enum class BuildMethod : uint32_t
	{
		Unknown = 0u,
		Make,
		Qmake,
		Xbuild
	};

	//-------------------------------------------------------------------------------------------------
	class BuildMethodStringifier
	{
	private:
		static const char			Make[];
		static const char			Qmake[];
		static const char			Xbuild[];

	public:
		static inline BuildMethod FromString(const std::string& buildMethod);
	};

	selectany const char BuildMethodStringifier::Make[] = "make";
	selectany const char BuildMethodStringifier::Qmake[] = "qmake";
	selectany const char BuildMethodStringifier::Xbuild[] = "xbuild";

	//-------------------------------------------------------------------------------------------------
	inline BuildMethod BuildMethodStringifier::FromString(const std::string& buildMethod)
	{
		if (EnumCompare(buildMethod, Make))
		{
			return BuildMethod::Make;
		}
		if (EnumCompare(buildMethod, Qmake))
		{
			return BuildMethod::Qmake;
		}
		if (EnumCompare(buildMethod, Xbuild))
		{
			return BuildMethod::Xbuild;
		}
		return BuildMethod::Unknown;
	}

	//-------------------------------------------------------------------------------------------------
	/// Tags
	//-------------------------------------------------------------------------------------------------
	selectany extern const char StageTag[] = "Stage:\t\t";
	selectany extern const char DetailTag[] = "Detail:\t\t";
	selectany extern const char WarningTag[] = "Warning:\t";
	selectany extern const char ErrorTag[] = "Error:\t\t";
	selectany extern const char SuccessTag[] = "Success:\t";
}
