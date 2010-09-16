PROJECT(org_commontk_eventbus)

SET(PLUGIN_export_directive "org_commontk_eventbus_EXPORT")

FILE(GLOB include_mafEventBus_file_list "${PROJECT_SOURCE_DIR}/mafEventBus/*.h")
FILE(GLOB implementation_mafEventBus_file_list "${PROJECT_SOURCE_DIR}/mafEventBus/*.cpp")
FILE(GLOB templete_mafEventBus_file_list1 "${PROJECT_SOURCE_DIR}/mafEventBus/*.txx")
FILE(GLOB templete_mafEventBus_file_list2 "${PROJECT_SOURCE_DIR}/mafEventBus/*.tpp")

FIND_PACKAGE(QtSOAP)
IF(NOT QtSOAP_FOUND)
  MESSAGE(FATAL_ERROR "error: QtSOAP package is required to build ${PROJECT_NAME}" )
ENDIF()

# TODO: Fix this after discussing include dependencies with Jc
SET(CTK_BASE_INCLUDE_DIRS ${CTK_BASE_INCLUDE_DIRS} ${QTSOAP_INCLUDE_DIR} CACHE INTERNAL "CTK includes" FORCE)

# Additional directories to include
SET(PLUGIN_include_directories
    
)


SET(PLUGIN_SRCS
  ctkEventBusImpl.cpp
  ctkEventBusPlugin.cpp
  
  ${implementation_mafEventBus_file_list}
  ${templete_mafEventBus_file_list1}
  ${templete_mafEventBus_file_list2}
)

SET(PLUGIN_MOC_SRCS
  ctkEventBusImpl_p.h
  ctkEventBusPlugin_p.h
  ctkEventHandlerWrapper_p.h
  
  ${include_mafEventBus_file_list}
)

SET(PLUGIN_UI_FORMS
 
 )

SET(PLUGIN_resources
     org_commontk_eventbus.qrc
 )

ctkMacroGetTargetLibraries(PLUGIN_target_libraries)

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  INCLUDE_DIRECTORIES ${PLUGIN_include_directories}
  SRCS ${PLUGIN_SRCS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries}
)
