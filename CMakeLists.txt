cmake_minimum_required(VERSION 3.10.0)
project(coffee-grounds-keeper VERSION 0.1.0 LANGUAGES C CXX)

add_executable(coffee-grounds-keeper 
    main.cpp
    mock_driver/mock_sensor/mock_sensor.cc
    mock_driver/terminal_display/terminal_display.cc
)

target_include_directories(coffee-grounds-keeper PRIVATE
    include
    mock_driver/mock_sensor
    mock_driver/terminal_display
)


