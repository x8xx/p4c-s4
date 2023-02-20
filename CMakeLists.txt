message(STATUS "Start configuring S4 back end")

set(P4C_S4_SOURCES
    main.cpp
    ../../backends/bmv2/psa_switch/midend.cpp
    ../../backends/bmv2/psa_switch/options.cpp
)

set(P4C_S4_HEADERS
    options.h
)

set(S4_DIST_HEADERS p4include/s4.p4)

add_cpplint_FILES(${CMAKE_CURRENT_SOURCE_DIR} "${P4C_S4_SOURCES};${P4C_S4_HEADERS}")

add_executable(p4c-s4 ${P4C_S4_SOURCES})
target_link_libraries (p4c-s4 ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
add_dependencies(p4c-s4 genIR)


install (TARGETS p4c-s4
  RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/driver/p4c.s4.cfg
  DESTINATION ${P4C_ARTIFACTS_OUTPUT_DIRECTORY}/p4c_src)


add_custom_target(s4linkbinary
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4c-s4 ${P4C_BINARY_DIR}/p4c-s4
  COMMAND ${CMAKE_COMMAND} -E make_directory ${P4C_BINARY_DIR}/p4include &&
          ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${S4_DIST_HEADERS} ${P4C_BINARY_DIR}/p4include
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4include ${CMAKE_CURRENT_BINARY_DIR}/p4include
)
add_dependencies(p4c_driver s4linkbinary)
