idf_component_register(SRCS "main.c" "./main.c"
                            "./scr/OLED.c"
                            "./scr/BMP.c"
                       INCLUDE_DIRS "./inc")

# CMakeLists.txt
include_directories(${CMAKE_SOURCE_DIR} /main)
