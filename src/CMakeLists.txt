
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCE_FILES main.cpp ../include/GeneralTypes.h ../include/CLI.h CLICommand.cpp ../include/CLICommand.h CLI.cpp logging_module.cpp ../include/logging_module.h)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${EXTERNAL_LIBS})