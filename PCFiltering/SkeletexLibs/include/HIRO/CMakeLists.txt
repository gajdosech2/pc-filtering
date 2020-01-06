target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/API.h
  ${CMAKE_CURRENT_LIST_DIR}/Camera.h
  ${CMAKE_CURRENT_LIST_DIR}/Debugger.h
  ${CMAKE_CURRENT_LIST_DIR}/Exportable.h
  ${CMAKE_CURRENT_LIST_DIR}/FileStorage.h
  ${CMAKE_CURRENT_LIST_DIR}/Gadget.h
  ${CMAKE_CURRENT_LIST_DIR}/gui/GuiGenerator.h
  ${CMAKE_CURRENT_LIST_DIR}/gui/GuiHelpers.h
  ${CMAKE_CURRENT_LIST_DIR}/gui/GuiPanel.h
  ${CMAKE_CURRENT_LIST_DIR}/gui/GuiTypes.h
  ${CMAKE_CURRENT_LIST_DIR}/HIRO.h
  ${CMAKE_CURRENT_LIST_DIR}/Input.h
  ${CMAKE_CURRENT_LIST_DIR}/Resource.h
  ${CMAKE_CURRENT_LIST_DIR}/ResourceId.h
  ${CMAKE_CURRENT_LIST_DIR}/Version.h
  )

add_subdirectory(modules)
