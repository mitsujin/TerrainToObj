set(BINARY ${CMAKE_PROJECT_NAME})

set (SOURCE 
    main.cpp
    game_representation.cpp
    game_moves.cpp
    game.cpp
    game_player.cpp
    nnet.cpp
    ucb.cpp
    mcts.cpp
    mcts_node.cpp
    model.cpp
    game_history.cpp
    random_utils.cpp
    target_model.cpp
    thread_pool.cpp
    train_threaded.cpp
    game_renderer.cpp
    game_bot.cpp
    dirichlet_random.cpp
    negamax.cpp
    mcts_player.cpp
    human_player.cpp
    model_server.cpp
    one_cycle_lr.cpp
)

add_executable(${BINARY} ${SOURCE})
target_link_libraries("${BINARY}"
    ${TORCH_LIBRARIES}
    ftxui::screen
    ftxui::dom
    ftxui::component)

add_library(${BINARY}_lib STATIC ${SOURCE})

target_compile_features(${BINARY} PRIVATE cxx_std_17)
target_compile_features(${BINARY}_lib PRIVATE cxx_std_17)

