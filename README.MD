# EOS Clean History Plugin

The `history_plugin` for EOS has been deprecated forever, yet it hasn't been removed as it's quite useful for some purposes. The plugin will probably stay for a while without a better alternative.

There's a serious performance issue with the plugin. As data grow, RAM consumption spikes and syncing slows down. Sometimes we only use the plugin for keeping track of the latest actions and don't care about history say older than one day, but that big chunk of history is hurting performance badly.

This is what the `clean_history_plugin` is for. When the `nodeos` process is run with this plugin turned on, the entire action history will be erased.

Without the plugin, you will have to do the following to achieve this:

1. Replay the blockchain with `history_plugin` turned off or `filter-on` cleared.

2. Shut down `nodeos`.

3. Run `nodeos` again with `history_plugin` turned on with `filter-on` set to interested accounts.

Either the process takes days or you will have to maintain regular snapshots without history action data.

## Build `nodeos` with the Plugin

First change directory to your `eos` source code directory:

    $ cd /path/to/eos/repository

Clone the plugin into the `plugins` folder:

    $ git clone https://github.com/xJonathanLEI/CleanHistoryPlugin.git plugins/clean_history_plugin

Add the following line to the `plugins/CMakeLists.txt` file to include the plugin:

    add_subdirectory(clean_history_plugin)

Open the file `programs/nodeos/CMakeLists.txt` and find the following command:

    target_link_libraries( ${NODE_EXECUTABLE_NAME}
            PRIVATE appbase
            PRIVATE -Wl,${whole_archive_flag} login_plugin               -Wl,${no_whole_archive_flag}
            PRIVATE -Wl,${whole_archive_flag} history_plugin             -Wl,${no_whole_archive_flag}
            ......

Add the following line:

    PRIVATE -Wl,${whole_archive_flag} clean_history_plugin        -Wl,${no_whole_archive_flag}

Build `eos` and install:

    $ sudo ./eosio_build.sh
    $ sudo ./eosio_install.sh

Now you have `nodeos` installed with `clean_history_plugin` available.

## Clean The History

**Don't turn off the `history_plugin` for this purpose! If you do you will get inconsistent history and will HAVE TO REPLAY THE ENTIRE BLOCKCHAIN.**

To clean the action history simply run your `nodeos` process with the following option:

    --plugin eosio::clean_history_plugin