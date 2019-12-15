
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCE_FILES main.cpp logging_module.cpp udp_beacon.cpp udp_listener.cpp resource_database.cpp resource.cpp ../include/resource.h host.cpp ../include/host.h)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${EXTERNAL_LIBS})