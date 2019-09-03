target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/API.h
  ${CMAKE_CURRENT_LIST_DIR}/Camera.h
  ${CMAKE_CURRENT_LIST_DIR}/DataId.h
  ${CMAKE_CURRENT_LIST_DIR}/DataInstance.h
  ${CMAKE_CURRENT_LIST_DIR}/DataUnit.h
  ${CMAKE_CURRENT_LIST_DIR}/GuiPanel.h
  ${CMAKE_CURRENT_LIST_DIR}/GuiGenerator.h
  ${CMAKE_CURRENT_LIST_DIR}/GuiTypes.h
  ${CMAKE_CURRENT_LIST_DIR}/HIRO.h
  ${CMAKE_CURRENT_LIST_DIR}/Version.h
  )

add_subdirectory(modules)
add_subdirectory(widgets)