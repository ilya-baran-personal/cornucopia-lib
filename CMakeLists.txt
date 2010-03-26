# Root Cornucopia CMakeLists.txt

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT( CORNUCOPIA )

ADD_SUBDIRECTORY( Cornucopia )
ADD_SUBDIRECTORY( DemoUI )

INCLUDE(InstallRequiredSystemLibraries)

INCLUDE( CPack )
