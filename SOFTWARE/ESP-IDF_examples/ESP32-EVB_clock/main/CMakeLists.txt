idf_component_register(SRCS "main.c" 
                    INCLUDE_DIRS "." 
                    REQUIRES hd44780 nvs_flash protocol_examples_common
)
