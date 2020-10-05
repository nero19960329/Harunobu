# - Try to find RapidXML
# Once done this will define
#  RAPIDXML_FOUND - System has RapidXML
#  RapidXML_INCLUDE_DIR - The RapidXML include directories

include(FindPackageHandleStandardArgs)

find_path(
    RapidXML_INCLUDE_DIR
    NAMES rapidxml/rapidxml.hpp 
    PATHS
    ${PROJECT_SOURCE_DIR}/include/thirdparty/
)

# handle the QUIETLY and REQUIRED arguments and set RAPIDXML_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(RapidXML DEFAULT_MSG RapidXML_INCLUDE_DIR)

if(RAPIDXML_FOUND)
    # provide import target:
	 add_library(RapidXML::RapidXML INTERFACE IMPORTED)
	 set_target_properties(RapidXML::RapidXML PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${RapidXML_INCLUDE_DIR})
endif()

mark_as_advanced(RapidXML_INCLUDE_DIR)