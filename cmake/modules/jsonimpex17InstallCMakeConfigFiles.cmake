# The path where cmake config files are installed
set(JSONIMPEX_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/jsonimpex17)

install(EXPORT jsonimpex17Targets
    FILE jsonimpex17Targets.cmake
    NAMESPACE jsonimpex17::
    DESTINATION ${JSONIMPEX_INSTALL_CONFIGDIR}
    COMPONENT cmake)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpex17ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/jsonimpex17Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpex17Config.cmake
    INSTALL_DESTINATION ${JSONIMPEX_INSTALL_CONFIGDIR}
    PATH_VARS JSONIMPEX_INSTALL_CONFIGDIR)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpex17Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpex17ConfigVersion.cmake
    DESTINATION ${JSONIMPEX_INSTALL_CONFIGDIR})
