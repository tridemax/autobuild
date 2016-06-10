#include "platform.h"
#include "../../source/AutoBuilder/AutoBuilder.h"


//-------------------------------------------------------------------------------------------------
/// main
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
	// Open the lock
	static const char lockPath[] = "/var/lock/autobuild.lock";

	const auto lockDescriptor = open(lockPath, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (lockDescriptor == -1)
	{
		std::cerr << "Another instance of autobuild is currently running." << std::endl;
		return 2;
	}

	// Build path to configuration file
	boost::filesystem::path configPath;

	for (int32_t argumentIndex = 1; argumentIndex < argc; ++argumentIndex)
	{
		if (boost::starts_with(argv[argumentIndex], "--config"))
		{
			if (argumentIndex + 1 != argc)
			{
				configPath = argv[++argumentIndex];
			}
		}
	}

	if (configPath.empty())
	{
		configPath = argv[0];
		configPath.remove_filename();
		configPath /= "autobuild.conf";
	}

	// Run the autobuild
	AutoBuild::AutoBuilder autoBuilder;

	int exitCode = 0;

	if (!autoBuilder.Run(configPath.c_str()))
	{
		std::cerr << autoBuilder.LastError() << std::endl;
		exitCode = 1;
	}

	// Close the lock
	unlink(lockPath);
	close(lockDescriptor);

	return exitCode;
}
