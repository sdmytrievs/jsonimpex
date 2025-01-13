# The path where cmake config files are installed
set(JSONIMPEX_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/jsonimpex)

install(EXPORT jsonimpexTargets
    FILE jsonimpexTargets.cmake
    NAMESPACE jsonimpex::
    DESTINATION ${JSONIMPEX_INSTALL_CONFIGDIR}
    COMPONENT cmake)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpexConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/jsonimpexConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpexConfig.cmake
    INSTALL_DESTINATION ${JSONIMPEX_INSTALL_CONFIGDIR}
    PATH_VARS JSONIMPEX_INSTALL_CONFIGDIR)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpexConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/jsonimpexConfigVersion.cmake
    DESTINATION ${JSONIMPEX_INSTALL_CONFIGDIR})
