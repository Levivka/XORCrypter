cmake_minimum_required(VERSION 3.16)

project(XorQtQuick VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appXorQtQuick
    main.cpp
    xorqtquick.cpp
    worker.cpp
)
target_sources(appXorQtQuick
    PRIVATE
    xorqtquick.h
        worker.h
)

qt_add_qml_module(appXorQtQuick
    URI XorQtQuick
    VERSION 1.0
    QML_FILES
    Main.qml
    ./components/CustomButton.qml
    ./components/CustomDialog.qml
    ./components/CustomFrame.qml
    ./components/CustomTextField.qml
    ./components/CustomLabel.qml
    ./components/CustomCheckBox.qml
    ./components/CustomProgressBar.qml
    ./components/SpinSlider.qml
    ./components/HistoryTable.qml
    SOURCES
    SOURCES tablemodel.h tablemodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appXorQtQuick PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appXorQtQuick
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appXorQtQuick
    PRIVATE
    Qt6::Quick
    Qt6::Sql
    Qt6::Concurrent
)

include(GNUInstallDirs)
install(TARGETS appXorQtQuick
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
