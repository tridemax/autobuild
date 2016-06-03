#include "platform.h"
#include "../../source/BusinessLogic/AutoBuilder/AutoBuilder.h"


//-------------------------------------------------------------------------------------------------
/// main
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
	// Open the lock
	const auto lockDescriptor = open("/var/lock/autobuild.lock", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (lockDescriptor == -1)
	{
		std::cerr << "Another instance of autobuild is currently running." << std::endl;
		return 2;
	}

	// Build path to configuration file
	std::string configPath;

	if (argc >= 2 && argv[1] && argv[1][0])
	{
		configPath = argv[1];
	}
	else
	{
		boost::filesystem::path elfPath = argv[0];
		elfPath.remove_filename();
		elfPath /= "autobuild.conf";

		configPath = elfPath.string();
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
	close(lockDescriptor);

	return exitCode;
}
