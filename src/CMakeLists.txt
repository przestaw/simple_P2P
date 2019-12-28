#TO DELETE

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCE_FILES main.cpp ../include/GeneralTypes.h ../include/CLI.h ../include/CLICommand.h CLI.cpp logging_module.cpp ../include/logging_module.h resource_database.cpp resource.cpp ../include/resource.h host.cpp ../include/host.h ../include/udp_server.cpp ../include/udp_server.h ../include/udp_module.cpp ../include/udp_module.h ../include/udp_client.cpp ../include/udp_client.h)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${EXTERNAL_LIBS})
