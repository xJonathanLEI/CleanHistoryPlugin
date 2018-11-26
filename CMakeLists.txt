file(GLOB HEADERS "include/eosio/clean_history_plugin/*.hpp")
add_library( clean_history_plugin
             clean_history_plugin.cpp
             ${HEADERS} )

target_link_libraries( clean_history_plugin appbase chain_plugin fc eosio_chain )
target_include_directories( clean_history_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
