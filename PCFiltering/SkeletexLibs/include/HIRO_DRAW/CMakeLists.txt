target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/API.h
  ${CMAKE_CURRENT_LIST_DIR}/VisualCamera.h
  ${CMAKE_CURRENT_LIST_DIR}/VisualCameraController.h
  ${CMAKE_CURRENT_LIST_DIR}/ElementRenderer.h
  ${CMAKE_CURRENT_LIST_DIR}/PrimitiveDatabase.h
  ${CMAKE_CURRENT_LIST_DIR}/Renderer.h
  ${CMAKE_CURRENT_LIST_DIR}/RenderSystem.h
  ${CMAKE_CURRENT_LIST_DIR}/Scene.h
  ${CMAKE_CURRENT_LIST_DIR}/ShaderApi.h
  ${CMAKE_CURRENT_LIST_DIR}/TextRenderer.h
  ${CMAKE_CURRENT_LIST_DIR}/TextureProcessing.h
  )

  add_subdirectory(renderers)