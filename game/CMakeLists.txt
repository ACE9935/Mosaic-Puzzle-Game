cmake_minimum_required(VERSION 3.3)
project(GameText C)

include(CTest)
enable_testing()

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "-std=c99 -Wall --coverage")


set(SOURCE_DIR ${CMAKE_SOURCE_DIR})
include(sdl2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

## compilation rules
include_directories(${SDL2_ALL_INC})

set(GAME_SOURCES
    game.c
    game_aux.c
    game_ext.c
    queue.c
    game_tools.c
    game_sdl.c
    
    
)

add_library(game STATIC ${GAME_SOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/solution.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(game_sdl main.c ${GAME_SOURCES})
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m)
add_executable(game_text game_text.c)
target_link_libraries(game_text game)
add_executable(game_test_aelmouden game_test_aelmouden.c)
target_link_libraries(game_test_aelmouden game)
add_executable(game_test_mrabih game_test_mrabih.c)
target_link_libraries(game_test_mrabih game)
add_executable(game_test_imohammi game_test_imohammi.c)
target_link_libraries(game_test_imohammi game)
add_executable(game_solve game_solve.c)
target_link_libraries(game_solve game)




add_test(test_aelmouden_dummy ./game_test_aelmouden dummy)
add_test(test_aelmouden_game_new ./game_test_aelmouden test_game_new)
add_test(test_aelmouden_game_new_empty ./game_test_aelmouden test_game_new_empty)
add_test(test_aelmouden_game_copy ./game_test_aelmouden test_game_copy)
add_test(test_aelmouden_game_equal ./game_test_aelmouden test_game_equal)
add_test(test_aelmouden_game_delete ./game_test_aelmouden test_game_delete)
add_test(test_aelmouden_game_solve ./game_test_aelmouden test_game_solve)




add_test(test_mrabih_dummy ./game_test_mrabih dummy)
add_test(test_mrabih_game_default ./game_test_mrabih test_game_default)
add_test(test_mrabih_game_default_solution ./game_test_mrabih test_game_default_solution)
add_test(test_mrabih_game_print ./game_test_mrabih test_game_print)
add_test(test_mrabih_game_set_constraint ./game_test_mrabih test_game_set_constraint)
add_test(test_mrabih_game_set_color ./game_test_mrabih test_game_set_color)
add_test(test_mrabih_game_get_constraint ./game_test_mrabih test_game_get_constraint)
add_test(test_mrabih_game_get_color ./game_test_mrabih test_game_get_color)
add_test(test_mrabih_game_get_next_square ./game_test_mrabih test_game_get_next_square)
add_test(test_mrabih_game_save ./game_test_mrabih test_game_save)







add_test(test_imohammi_dummy ./game_test_imohammi dummy)
add_test(test_imohammi_game_set_color ./game_test_imohammi test_game_set_color)
add_test(test_imohammi_game_get_status ./game_test_imohammi test_game_get_status)
add_test(test_imohammi_game_nb_neighbors ./game_test_imohammi test_game_nb_neighbors)
add_test(test_imohammi_game_play_move ./game_test_imohammi test_game_play_move)
add_test(test_imohammi_game_won ./game_test_imohammi test_game_won)
add_test(test_imohammi_game_restart ./game_test_imohammi test_game_restart)
add_test(test_imohammi_game_new_ext ./game_test_imohammi test_game_new_ext)
add_test(test_imohammi_game_new_empty_ext ./game_test_imohammi test_game_new_empty_ext)
add_test(test_imohammi_game_nb_rows ./game_test_imohammi test_game_nb_rows)
add_test(test_imohammi_game_nb_cols ./game_test_imohammi test_game_nb_cols)
add_test(test_imohammi_game_is_wrapping ./game_test_imohammi test_game_is_wrapping)
add_test(test_imohammi_game_get_neighbourhood ./game_test_imohammi test_game_get_neighbourhood)
add_test(test_imohammi_game_undo ./game_test_imohammi test_game_undo)
add_test(test_imohammi_game_redo ./game_test_imohammi test_game_redo)
add_test(test_imohammi_game_load ./game_test_imohammi test_game_load)