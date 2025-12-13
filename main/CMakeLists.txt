idf_component_register(
    SRCS
        "tiny_turtle_app.cpp"
        "tiny_turtle/arduino_compat.cpp"
        "tiny_turtle/neopixel_compat.cpp"
        "tiny_turtle/Tiny_Turtle_Plotter_Hello_World_10.cpp"
        "tiny_turtle/light_sound.cpp"
        "tiny_turtle/low_level_hardware_functions.cpp"
        "tiny_turtle/moving_functions.cpp"
        "tiny_turtle/text_writing_functions.cpp"
        "tiny_turtle/trigonometry.cpp"
        "tiny_turtle/coordinates.cpp"
        "tiny_turtle/stepper_timer.cpp"
    INCLUDE_DIRS
        "."
        "tiny_turtle"
    REQUIRES
        driver
        esp_adc
        esp_timer
)

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_17)
