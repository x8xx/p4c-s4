message(STATUS "Start configuring S4 back end")

set(P4C_S4_SOURCES
    main.cpp
    )

add_executable(p4c-s4 ${P4C_S4_SOURCES})
target_link_libraries (p4c-s4 ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
add_dependencies(p4c-s4 genIR)


install (TARGETS p4c-s4
  RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/driver/p4c.s4.cfg
  DESTINATION ${P4C_ARTIFACTS_OUTPUT_DIRECTORY}/p4c_src)


add_custom_target(s4linkbinary
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4c-s4 ${P4C_BINARY_DIR}/p4c-s4
)
add_dependencies(p4c_driver s4linkbinary)
