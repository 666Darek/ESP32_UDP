set(SOURCES "wifi_lib.c")

idf_component_register(SRCS "udp_server.c" "${SOURCES}"
                       INCLUDE_DIRS ".")
