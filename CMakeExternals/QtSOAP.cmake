#
# QtSOAP
#

SET(QtSOAP_DEPENDS)
ctkMacroShouldAddExternalProject(QTSOAP_LIBRARY add_project)
IF(${add_project})
  
  SET(proj QtSOAP)
  MESSAGE(STATUS "Adding project:${proj}")
  SET(QtSOAP_DEPENDS ${proj})

  IF(WIN32)
    SET(_qtsoap_url "${CTK_SOURCE_DIR}/Utilities/QtSOAP/qtsoap-2.7_1-opensource-win32.tar.gz")
    SET(_make_cmd nmake)
    IF(MINGW)
      SET(_make_cmd mingw32-make)
    ENDIF()
  ELSE()
    SET(_qtsoap_url "http://get.qt.nokia.com/qt/solutions/lgpl/qtsoap-2.7_1-opensource.tar.gz")
    SET(_make_cmd make)
  ENDIF()

  
  # Patch program
  FIND_PROGRAM(CTK_PATCH_EXECUTABLE patch
    "C:/Program Files/GnuWin32/bin"
    "C:/Program Files (x86)/GnuWin32/bin")
  MARK_AS_ADVANCED(CTK_PATCH_EXECUTABLE)
  IF(NOT CTK_PATCH_EXECUTABLE)
    MESSAGE(FATAL_ERROR "error: Patch is required to build ${PROJECT_NAME}. Set CTK_PATCH_EXECUTABLE")
  ENDIF()

  # Configure patch script
  SET(qtsoap_src_dir ${ep_source_dir}/${proj}/src)
  SET(qtsoap_patch_dir ${CTK_SOURCE_DIR}/Utilities/QtSOAP)
  SET(qtsoap_configured_patch_dir ${CTK_BINARY_DIR}/Utilities/QtSOAP)
  SET(qtsoap_patchscript
    ${qtsoap_configured_patch_dir}/qtsoap-2.7_1-patch.cmake)
  CONFIGURE_FILE(
    ${CTK_SOURCE_DIR}/Utilities/QtSOAP/qtsoap-2.7_1-patch.cmake.in
    ${qtsoap_patchscript} @ONLY)
  
  SET(_qtsoap_build_script "${CTK_BINARY_DIR}/Utilities/QtSOAP/BuildScript.cmake")
  CONFIGURE_FILE("${CTK_SOURCE_DIR}/Utilities/QtSOAP/BuildScript.cmake.in" ${_qtsoap_build_script} @ONLY)

  SET(_qtsoap_install_script "${CTK_BINARY_DIR}/Utilities/QtSOAP/InstallScript.cmake")
  CONFIGURE_FILE("${CTK_SOURCE_DIR}/Utilities/QtSOAP/InstallScript.cmake.in" ${_qtsoap_install_script} @ONLY)

  ExternalProject_Add(${proj}
    URL ${_qtsoap_url}
    PATCH_COMMAND ${CMAKE_COMMAND} -P ${qtsoap_patchscript}
    CONFIGURE_COMMAND <SOURCE_DIR>/configure -library
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ${CMAKE_COMMAND} -P ${_qtsoap_build_script}
    INSTALL_COMMAND ${CMAKE_COMMAND} -D INTERMEDIATE_DIRECTORY:STRING=$(IntDir) -P ${_qtsoap_install_script}
  )

  SET(QtSOAP_DIR "${CTK_BINARY_DIR}/Utilities/QtSOAP/")

  # Since the full path of PythonQt library is used, there is not need to add 
  # its corresponding library output directory to CTK_EXTERNAL_LIBRARY_DIRS
    
ENDIF()
