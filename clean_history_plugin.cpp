#include <eosio/chain/name.hpp>
#include <eosio/chain/types.hpp>
#include <eosio/chain/trace.hpp>
#include <eosio/chain/config.hpp>
#include <eosio/chain/controller.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>

#include <eosio/clean_history_plugin/clean_history_plugin.hpp>

namespace eosio
{
using namespace fc;
using namespace eosio::chain;
using boost::signals2::scoped_connection;

static appbase::abstract_plugin &_clean_history_plugin = app().register_plugin<clean_history_plugin>();

class clean_history_plugin_impl
{
}; // namespace eosio

clean_history_plugin::clean_history_plugin() : my(new clean_history_plugin_impl()) {}
clean_history_plugin::~clean_history_plugin() {}

void clean_history_plugin::set_program_options(options_description &, options_description &cfg) {}

void clean_history_plugin::plugin_initialize(const variables_map &options)
{
    try
    {
    }
    FC_LOG_AND_RETHROW()
}

void clean_history_plugin::plugin_startup()
{
    // Make the magic happen
}

void clean_history_plugin::plugin_shutdown()
{
    // OK, that's enough magic
}

} // namespace eosio
