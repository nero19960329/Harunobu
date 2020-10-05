# - Try to find spdlog
# Once done this will define
#  SPDLOG_FOUND - System has spdlog
#  SPDLOG_INCLUDE_DIR - The spdlog include directories

include(FindPackageHandleStandardArgs)

find_path(
    SPDLOG_INCLUDE_DIR
    NAMES spdlog/spdlog.h
    PATHS
    ${PROJECT_SOURCE_DIR}/include/thirdparty/
)

# handle the QUIETLY and REQUIRED arguments and set SPDLOG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(spdlog DEFAULT_MSG SPDLOG_INCLUDE_DIR)

if(SPDLOG_FOUND)
    # provide import target:
	 add_library(spdlog::spdlog INTERFACE IMPORTED)
	 set_target_properties(spdlog::spdlog PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SPDLOG_INCLUDE_DIR})
endif()

mark_as_advanced(SPDLOG_INCLUDE_DIR)