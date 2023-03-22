set(SOURCES "ssd1306.c" "ssd1306_i2c.c" "ssd1306_spi.c")

idf_component_register(SRCS ${SOURCES}
			PRIV_REQUIRES driver
            INCLUDE_DIRS ".")