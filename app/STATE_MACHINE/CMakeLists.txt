set(SOURCES state_machine.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .)