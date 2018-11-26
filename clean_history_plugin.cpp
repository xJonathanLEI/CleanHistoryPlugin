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

struct account_history_object : public chainbase::object<account_history_object_type, account_history_object>
{
    OBJECT_CTOR(account_history_object);

    id_type id;
    account_name account;             ///< the name of the account which has this action in its history
    uint64_t action_sequence_num = 0; ///< the sequence number of the relevant action (global)
    int32_t account_sequence_num = 0; ///< the sequence number for this account (per-account)
};

struct action_history_object : public chainbase::object<action_history_object_type, action_history_object>
{

    OBJECT_CTOR(action_history_object, (packed_action_trace));

    id_type id;
    uint64_t action_sequence_num; ///< the sequence number of the relevant action

    shared_string packed_action_trace;
    uint32_t block_num;
    block_timestamp_type block_time;
    transaction_id_type trx_id;
};
using account_history_id_type = account_history_object::id_type;
using action_history_id_type = action_history_object::id_type;

struct by_action_sequence_num;
struct by_account_action_seq;
struct by_trx_id;

using action_history_index = chainbase::shared_multi_index_container<
    action_history_object,
    indexed_by<
        ordered_unique<tag<by_id>, member<action_history_object, action_history_object::id_type, &action_history_object::id>>,
        ordered_unique<tag<by_action_sequence_num>, member<action_history_object, uint64_t, &action_history_object::action_sequence_num>>,
        ordered_unique<tag<by_trx_id>,
                       composite_key<action_history_object,
                                     member<action_history_object, transaction_id_type, &action_history_object::trx_id>,
                                     member<action_history_object, uint64_t, &action_history_object::action_sequence_num>>>>>;

using account_history_index = chainbase::shared_multi_index_container<
    account_history_object,
    indexed_by<
        ordered_unique<tag<by_id>, member<account_history_object, account_history_object::id_type, &account_history_object::id>>,
        ordered_unique<tag<by_account_action_seq>,
                       composite_key<account_history_object,
                                     member<account_history_object, account_name, &account_history_object::account>,
                                     member<account_history_object, int32_t, &account_history_object::account_sequence_num>>>>>;

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
        auto *chain_plug = app().find_plugin<chain_plugin>();
        EOS_ASSERT(chain_plug, chain::missing_chain_plugin_exception, "");
        auto &chain = chain_plug->chain();
        chainbase::database &db = const_cast<chainbase::database &>(chain.db()); // Override read-only access to state DB (highly unrecommended practice!)

        uint64_t rows_erased = 0;

        // Cleans action
        std::cout << "Cleaning action history..." << std::endl;
        const auto &action_idx = db.get_index<action_history_index, by_id>();
        auto &mutable_action_idx = db.get_mutable_index<action_history_index>();
        while (!action_idx.empty())
        {
            mutable_action_idx.remove(*action_idx.begin());
            rows_erased++;
        }
        std::cout << "Action history rows removed: " << rows_erased << std::endl;

        // Cleans account
        std::cout << "Cleaning account history..." << std::endl;
        const auto &account_idx = db.get_index<account_history_index, by_id>();
        auto &mutable_account_idx = db.get_mutable_index<account_history_index>();
        while (!account_idx.empty())
        {
            mutable_account_idx.remove(*account_idx.begin());
            rows_erased++;
        }
        std::cout << "Account history rows removed: " << rows_erased << std::endl;
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
