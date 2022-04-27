cmake_minimum_required(VERSION 2.8...3.20)
project(EyeOfRhianne)

add_subdirectory(Yathsou)

find_package(glm REQUIRED)
find_package(imgui REQUIRED)

############################################################################
############################################################################
############################################################################

set(Header_Files
    "Source/EyeOfRhianne/EyeOfRhianneConfiguration.h"
    "Source/EyeOfRhianne/EyeOfRhianne.h"
    "Source/EyeOfRhianne/GamedataPicker.h"
    "Source/EyeOfRhianne/GamedataLoader.h"
    "Source/EyeOfRhianne/EyeState.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Source/EyeOfRhianne/main.cpp"
    "Source/EyeOfRhianne/EyeOfRhianne.cpp"
    "Source/EyeOfRhianne/EyeOfRhianneConfiguration.cpp"
    "Source/EyeOfRhianne/GamedataPicker.cpp"
    "Source/EyeOfRhianne/GamedataLoader.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Header_Files__AssetMainMenu
    "Source/EyeOfRhianne/AssetMainMenu/AssetMainMenu.h"
    "Source/EyeOfRhianne/AssetMainMenu/Graphic.h"
    "Source/EyeOfRhianne/AssetMainMenu/GraphicOptionsMenuItem.h"
    "Source/EyeOfRhianne/AssetMainMenu/MapSelection.h"
    "Source/EyeOfRhianne/AssetMainMenu/ScriptSelection.h"
    "Source/EyeOfRhianne/AssetMainMenu/SkyBoxMenuItem.h"
    "Source/EyeOfRhianne/AssetMainMenu/SoundMenuItem.h"
    "Source/EyeOfRhianne/AssetMainMenu/UnitMenuItem.h"
)
source_group("Header Files\\AssetMainMenu" FILES ${Header_Files__AssetMainMenu})

set(Source_Files__AssetMainMenu
    "Source/EyeOfRhianne/AssetMainMenu/AssetMainMenu.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/Graphic.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/GraphicOptionsMenuItem.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/MapSelection.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/ScriptSelection.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/SkyBoxMenuItem.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/SoundMenuItem.cpp"
    "Source/EyeOfRhianne/AssetMainMenu/UnitMenuItem.cpp"
)
source_group("Source Files\\AssetMainMenu" FILES ${Source_Files__AssetMainMenu})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${Header_Files__AssetMainMenu}
    ${Source_Files__AssetMainMenu}
)

############################################################################
############################################################################
############################################################################

add_executable(EyeOfRhianne ${ALL_FILES})
set_property(TARGET EyeOfRhianne PROPERTY CXX_STANDARD 20)


target_link_libraries(EyeOfRhianne 
IyathuumCoreLib 
SelenNavigationLib
SuthanusPhysicLib
AhwassaGraphicsLib
HaasScriptingLib
AezeselFileIOLib
VishalaNetworkLib
AthanahCommonLib
glm::glm
imgui::imgui
)

target_include_directories(EyeOfRhianne PRIVATE 
"${IyathuumCoreLib_SOURCE_DIR}/Source"
"${SelenNavigationLib_SOURCE_DIR}/Source"
"${SuthanusPhysicLib_SOURCE_DIR}/Source"
"${AhwassaGraphicsLib_SOURCE_DIR}/Source"
"${HaasScriptingLib_SOURCE_DIR}/Source"
"${AezeselFileIOLib_SOURCE_DIR}/Source"
"${VishalaNetworkLib_SOURCE_DIR}/Source"
"${AthanahCommonLib_SOURCE_DIR}/Source"
"${EyeOfRhianne_SOURCE_DIR}/Source"

)

add_dependencies(EyeOfRhianne 
IyathuumCoreLib 
SelenNavigationLib
SuthanusPhysicLib
AhwassaGraphicsLib
HaasScriptingLib
AezeselFileIOLib
VishalaNetworkLib
AthanahCommonLib
)

add_custom_command(TARGET EyeOfRhianne POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               "${EyeOfRhianne_SOURCE_DIR}/Scripts"              
                $<TARGET_FILE_DIR:EyeOfRhianne>/Data/Scripts)
				
add_custom_command(TARGET EyeOfRhianne POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               "${AhwassaGraphicsLib_SOURCE_DIR}/Data/Fonts/arial.ttf"              
                $<TARGET_FILE_DIR:EyeOfRhianne>/Data/Fonts/arial.ttf)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT EyeOfRhianne)
