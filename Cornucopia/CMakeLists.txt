# Cornucopia library CMakeLists.txt

ADD_LIBRARY( Cornucopia STATIC libTest )

INSTALL( TARGETS Cornucopia ARCHIVE DESTINATION lib )

INSTALL( DIRECTORY . DESTINATION include FILES_MATCHING PATTERN "*.h"  )
