cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ConfigReaderProject)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавление исполняемого файла
add_executable(config_reader src/main.cpp src/ConfigReader.cpp)

# Включение заголовочных файлов
target_include_directories(config_reader PRIVATE include)
