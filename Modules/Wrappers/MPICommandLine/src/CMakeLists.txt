add_library(OTBMPICommandLine otbMPIWrapperCommandLineLauncher.cxx)
target_link_libraries(OTBMPICommandLine 
  ${OTBMPIApplicationEngine_LIBRARIES}
  ${OTBCommandLine_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  )
otb_module_target(OTBMPICommandLine)

add_executable(otbMPIApplicationLauncherCommandLine otbMPIApplicationLauncherCommandLine.cxx)
target_link_libraries(otbMPIApplicationLauncherCommandLine OTBMPICommandLine)
otb_module_target(otbMPIApplicationLauncherCommandLine)

# Where we will install the script in the build tree
get_target_property(CLI_OUTPUT_DIR otbMPIApplicationLauncherCommandLine RUNTIME_OUTPUT_DIRECTORY)

# Generate a script in the build dir, next to the cli launcher
# Need a two-step process since configure_file don't support permissions
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts)

if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbclimpi.bat.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbclimpi.bat
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbclimpi.bat
     DESTINATION ${CLI_OUTPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# TODO : test if NO_INSTALL is set
install(PROGRAMS ${CLI_OUTPUT_DIR}/otbclimpi.bat
        DESTINATION ${OTBMPICommandLine_INSTALL_RUNTIME_DIR}
        COMPONENT Runtime)

else()
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbclimpi.sh.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbclimpi
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbclimpi
     DESTINATION ${CLI_OUTPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# TODO : test if NO_INSTALL is set
install(PROGRAMS ${CLI_OUTPUT_DIR}/otbclimpi
        DESTINATION ${OTBMPICommandLine_INSTALL_RUNTIME_DIR}
        COMPONENT Runtime)

endif()
