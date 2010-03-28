# CmakeLists.txt in Tools

FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTSVG TRUE)
INCLUDE(${QT_USE_FILE})

FILE(GLOB Tools_CPP "*.cpp")
FILE(GLOB Tools_H "*.h")
FILE(GLOB Tools_UI "*.ui")

QT4_WRAP_UI(Tools_H ${Tools_UI})

LIST(APPEND Tools_Sources ${Tools_CPP} ${Tools_H})

QT4_AUTOMOC(${Tools_Sources})

#This is for moc files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(CornucopiaTools ${Tools_Sources})

TARGET_LINK_LIBRARIES(CornucopiaTools ${QT_LIBRARIES})

CONFIGURE_FILE(config.h.cmake config.h )