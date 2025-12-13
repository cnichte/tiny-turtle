idf_component_register(
    SRCS
        # App
        "tiny_turtle_app.cpp"
        
        # Core Module
        "tiny_turtle/core/globals.cpp"
        
        # HAL Module
        "tiny_turtle/hal/gpio_hal.cpp"
        "tiny_turtle/hal/neopixel.cpp"
        "tiny_turtle/hal/stepper.cpp"
        "tiny_turtle/hal/servo.cpp"
        "tiny_turtle/hal/sensors.cpp"
        "tiny_turtle/hal/audio.cpp"
        "tiny_turtle/hal/led.cpp"
        
        # Motion Module
        "tiny_turtle/motion/motion.cpp"
        "tiny_turtle/motion/spiral.cpp"
        "tiny_turtle/motion/coordinates.cpp"
        
        # Drawing Module
        "tiny_turtle/drawing/text.cpp"
        "tiny_turtle/drawing/fonts.cpp"
        
        # Math Module
        "tiny_turtle/math/trigonometry.cpp"
        
        # Demos
        "tiny_turtle/demos/demo_hello_world.cpp"
        "tiny_turtle/demos/demo_motor_test.cpp"
        "tiny_turtle/demos/demo_shapes.cpp"
        
        # Main API
        "tiny_turtle/tiny_turtle.cpp"
        
    INCLUDE_DIRS
        "."
        "tiny_turtle"
        "tiny_turtle/core"
        "tiny_turtle/hal"
        "tiny_turtle/motion"
        "tiny_turtle/drawing"
        "tiny_turtle/math"
        "tiny_turtle/demos"
    REQUIRES
        driver
        esp_adc
        esp_timer
        esp_driver_rmt
)

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_17)
