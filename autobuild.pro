
CONFIG(debug, debug|release) {
	message("autobuild_debug")

	TARGET = autobuild_debug

	DESTDIR = $$_PRO_FILE_PWD_/../.dist
	OBJECTS_DIR = $$_PRO_FILE_PWD_/../.int/autobuild_debug

} else {
	message("autobuild_release")

	TARGET = autobuild

	DESTDIR = $$_PRO_FILE_PWD_/../.dist
	OBJECTS_DIR = $$_PRO_FILE_PWD_/../.int/autobuild_release
}

TEMPLATE = app
CONFIG += console precompile_header c++14
CONFIG -= qt

#-------------------------------------------------------------------------------------------------
# warnings
#-------------------------------------------------------------------------------------------------
QMAKE_CXXFLAGS_WARN_ON += \
	-Wno-parentheses \
	-Wno-unused-variable \
	-Wno-unused-parameter \
	-Wno-unused-local-typedefs \
	-Wno-attributes \
	-Wno-unused-but-set-variable \
	-Wno-unused-result \
	-Wno-switch

#-------------------------------------------------------------------------------------------------
# compiler flags
#-------------------------------------------------------------------------------------------------
QMAKE_CXXFLAGS += \
	-m64 \
	-msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx -mf16c \
	-fpic \
	-fdata-sections \
	-ffunction-sections \
	-I$$_PRO_FILE_PWD_/../tbb/include \
	-I$$_PRO_FILE_PWD_/../boost \
	-I$$_PRO_FILE_PWD_/../auxiliary \
	-I$$_PRO_FILE_PWD_/platform/linux \

PRECOMPILED_HEADER = $$_PRO_FILE_PWD_/platform/linux/platform.h

CONFIG(debug, debug|release) {
	DEFINES += _DEBUG DEBUG

} else {
	DEFINES += NDEBUG

	QMAKE_CFLAGS_RELEASE -= -O0 -O1 -O2
	QMAKE_CFLAGS_RELEASE *= -O3

	QMAKE_CXXFLAGS_RELEASE -= -O0 -O1 -O2
	QMAKE_CXXFLAGS_RELEASE *= -O3
}

#-------------------------------------------------------------------------------------------------
# linker flags
#-------------------------------------------------------------------------------------------------
LIBS += \
	-L$$DESTDIR \
	-L$$_PRO_FILE_PWD_/../tbb/lib/intel64/gcc4.4 \
	-Wl,--unresolved-symbols=report-all \
	-Wl,--gc-sections \
	-Wl,-rpath,./

CONFIG(debug, debug|release) {
	LIBS += \
		-lauxiliary_debug \
		-ltbb_debug \
		-lpthread

} else {
	LIBS += \
		-lauxiliary \
		-ltbb \
		-lpthread \
		-O3
}

#-------------------------------------------------------------------------------------------------
# dependencies
#-------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
	copydata.commands = cp -f -v \
		$$_PRO_FILE_PWD_/../tbb/lib/intel64/gcc4.4/libtbb_debug.so \
		$$_PRO_FILE_PWD_/../tbb/lib/intel64/gcc4.4/libtbb_debug.so.2 \
		$$DESTDIR

} else {
	copydata.commands = cp -f -v \
		$$_PRO_FILE_PWD_/../tbb/lib/intel64/gcc4.4/libtbb.so \
		$$_PRO_FILE_PWD_/../tbb/lib/intel64/gcc4.4/libtbb.so.2 \
		$$DESTDIR
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata

#-------------------------------------------------------------------------------------------------
# files
#-------------------------------------------------------------------------------------------------
SOURCES += \
    platform/linux/main.cpp \
    source/BusinessLogic/AutoBuilder/AutoBuilder.cpp \
    source/BusinessLogic/AutoBuilder/Repository.cpp \
    source/BusinessLogic/AutoBuilder/Actions/BuildAction.cpp \
    source/BusinessLogic/AutoBuilder/Actions/BundleAction.cpp \
    source/BusinessLogic/AutoBuilder/Actions/InstallSiteAction.cpp \
    source/BusinessLogic/AutoBuilder/Actions/InstallServiceAction.cpp \
    source/BusinessLogic/Bundle/Bundle.cpp \
    source/BusinessLogic/Bundle/File.cpp \
    source/BusinessLogic/Bundle/Folder.cpp

HEADERS += \
    platform/linux/platform.h \
    source/BusinessLogic/AutoBuilder/AutoBuilder.h \
    source/BusinessLogic/AutoBuilder/Repository.h \
    source/BusinessLogic/AutoBuilder/Miscellaneous.h \
    source/BusinessLogic/AutoBuilder/Actions/BuildAction.h \
    source/BusinessLogic/AutoBuilder/Actions/BundleAction.h \
    source/BusinessLogic/AutoBuilder/Actions/IAction.h \
    source/BusinessLogic/AutoBuilder/Actions/InstallSiteAction.h \
    source/BusinessLogic/AutoBuilder/SharedState.h \
    source/BusinessLogic/AutoBuilder/Actions/InstallServiceAction.h \
    source/BusinessLogic/Bundle/Bundle.h \
    source/BusinessLogic/Bundle/File.h \
    source/BusinessLogic/Bundle/Folder.h
