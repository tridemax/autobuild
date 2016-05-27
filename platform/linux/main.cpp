#include "platform.h"
#include "../../source/BusinessLogic/AutoBuilder/AutoBuilder.h"


//-------------------------------------------------------------------------------------------------
/// main
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
	AutoBuild::AutoBuilder autoBuilder;

	if (!autoBuilder.Run(argc >= 2 && argv[1] && argv[1][0] ? argv[1] : "./autobuild.conf"))
	{
		std::cout << "Error: " << autoBuilder.LastError() << std::endl;
		return 1;
	}

	std::cout << "Build successfully completed." << std::endl;

	return 0;
}
