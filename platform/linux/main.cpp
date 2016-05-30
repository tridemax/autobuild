#include "platform.h"
#include "../../source/BusinessLogic/AutoBuilder/AutoBuilder.h"


//-------------------------------------------------------------------------------------------------
/// main
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
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

	std::cout << configPath << std::endl;

	// Run the autobuild
	AutoBuild::AutoBuilder autoBuilder;

	if (!autoBuilder.Run(configPath.c_str()))
	{
		std::cout << "Error: " << autoBuilder.LastError() << std::endl;
		return 1;
	}

	std::cout << "Build successfully completed." << std::endl;

	return 0;
}
