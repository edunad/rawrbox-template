
set(output_target my-game)
project(${output_target})

# Grab source files
file(GLOB_RECURSE MY_GAME_SOURCES "include/*.hpp" "src/*.cpp")

# Project setup
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE})

add_executable(${output_target} ${MY_GAME_SOURCES})
copy_resources(TARGET ${output_target})

target_include_directories(${output_target} PRIVATE "include")
target_compile_features(${output_target} PRIVATE cxx_std_${CMAKE_CXX_STANDARD})
target_link_libraries(${output_target} PRIVATE RAWRBOX.RENDER)
target_compile_definitions(${output_target} PRIVATE NOMINMAX)

if(NOT WIN32)
    set_target_properties(${output_target} PROPERTIES LINK_FLAGS -Wl,-rpath,'\$ORIGIN')
else()
    add_compile_options("/EHsc")
    copy_required_dlls(${output_target}) # Required by diligent
endif()
