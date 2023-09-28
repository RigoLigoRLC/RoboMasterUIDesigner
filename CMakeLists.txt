cmake_minimum_required(VERSION 3.5)

project(RoboMasterUIDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES i18n/RoboMasterUIDesigner_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        res.qrc

        project/projectmanager.h project/projectmanager.cpp

        gridedgraphicsview.h gridedgraphicsview.cpp

        graphics/event/jighandlemoveevent.h
        graphics/event/jigeditingevent.h
        graphics/event/elementselectionevent.h

        graphics/jig/jighandle.h graphics/jig/jighandle.cpp
        graphics/jig/elementjig.h graphics/jig/elementjig.cpp
        graphics/jig/rectangularjig.h graphics/jig/rectangularjig.cpp
        graphics/jig/ellipticalarcjig.h graphics/jig/ellipticalarcjig.cpp
        graphics/jig/anchorjig.h graphics/jig/anchorjig.cpp

        graphics/element/uielement.h graphics/element/uielement.cpp
        graphics/element/rectangleelement.h graphics/element/rectangleelement.cpp
        graphics/element/ellipseelement.h graphics/element/ellipseelement.cpp
        graphics/element/ellipticalarcelement.h graphics/element/ellipticalarcelement.cpp

        graphics/designergraphicsscene.h graphics/designergraphicsscene.cpp
        ${TS_FILES}
)

include_directories(graphics/event graphics/jig graphics/element project)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RoboMasterUIDesigner
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RoboMasterUIDesigner APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(RoboMasterUIDesigner SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RoboMasterUIDesigner
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(RoboMasterUIDesigner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(MSVC)
    target_compile_options(RoboMasterUIDesigner PRIVATE /utf-8)
endif()

set_target_properties(RoboMasterUIDesigner PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS RoboMasterUIDesigner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RoboMasterUIDesigner)
endif()
