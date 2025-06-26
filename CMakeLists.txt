cmake_minimum_required(VERSION 3.30)
project(milestone1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        milestone1 week11.cpp

        diamond_algorithm/Grid.cpp diamond_algorithm/Grid.hpp
        diamond_algorithm/Cycle.cpp diamond_algorithm/Cycle.hpp
        diamond_algorithm/Encryptor.cpp diamond_algorithm/Encryptor.hpp
        diamond_algorithm/Decryptor.cpp diamond_algorithm/Decryptor.hpp

        controller/Interface.cpp controller/Interface.hpp
        controller/Menu.cpp controller/Menu.hpp
        controller/Action.cpp controller/Action.hpp
        )
