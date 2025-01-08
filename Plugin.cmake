
# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/deeprey-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/deeprey-beta"
    CACHE STRING "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/deeprey-prod"
    CACHE STRING "Default repository for tagged builds not matching 'beta'"
)
option(DEEPREY_USE_SVG "Use SVG graphics" ON)


#
#
# -------  Plugin setup --------
#
set(PKG_NAME CleanPlugin_pi)
set(PKG_VERSION  0.0.0.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME [CleanPlugin])    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME [CleanPlugin]) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Clean OCPN plugin")
set(PKG_DESCRIPTION [=[
"Clean OCPN plugin for tests.
]=])



set(PKG_AUTHOR "BS")
set(PKG_IS_OPEN_SOURCE "no")
set(CPACK_PACKAGE_HOMEPAGE_URL https://github.com/)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/)

# ----------------------------------------------------------------------------------------------------------------#





# ----------------------------------------------------------------------------------------------------------------#


# -- Regroupement final
set(SRC
    src/CleanPlugin_pi.cpp
    # separated controls
)

set(HEADERS
    include/CleanPlugin_pi.h
    # separated headers

)

# Force files to appear at the root level instead of "Header Files" or "Source Files"
source_group("" FILES
    ${SRC}
    ${HEADERS}
)


add_definitions("-DocpnUSE_GL")

set(PKG_API_LIB api-19)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
  if (DEEPREY_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC DEEPREY_USE_SVG)
  endif ()
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugin_dc")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugin-dc)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

#  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugingl")
#  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

#  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/jsoncpp")
#  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

  # The wxsvg library enables SVG overall in the plugin
#  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxsvg")
#  target_link_libraries(${PACKAGE_NAME} ocpn::wxsvg)

endmacro ()
