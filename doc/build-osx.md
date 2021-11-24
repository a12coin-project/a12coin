Mac OS X Build Instructions and Notes
====================================
The commands in this guide should be executed in a Terminal application.
The built-in one is located in `/Applications/Utilities/Terminal.app`.

Preparation
-----------
Install the OS X command line tools:

`xcode-select --install`

When the popup appears, click `Install`.

Then install [Homebrew](https://brew.sh).

Dependencies
----------------------

    brew install automake berkeley-db4 libtool boost --c++11 miniupnpc openssl pkg-config protobuf qt libevent

If you want to build the disk image with `make deploy` (.dmg / optional), you need RSVG

    brew install librsvg

If you want to build with ZeroMQ support
    
    brew install zeromq

NOTE: Building with Qt4 is still supported, however, could result in a broken UI. Building with Qt5 is recommended.

Build A12coin Core
------------------------

1. Clone the a12coin source code and cd into `a12coin`

        git clone https://github.com/a12coin-project/a12coin
        cd a12coin

2.  Build a12coin-core:

    Configure and build the headless a12coin binaries as well as the GUI (if Qt is found).

    You can disable the GUI build by passing `--without-gui` to configure.

        ./autogen.sh
        ./configure
        make

3.  It is recommended to build and run the unit tests:

        make check

4.  You can also create a .dmg that contains the .app bundle (optional):

        make deploy

Running
-------

A12coin Core is now available at `./src/a12coind`

Before running, it's recommended you create an RPC configuration file.

    echo -e "rpcuser=a12coinrpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > "/Users/${USER}/Library/Application Support/A12coin/a12coin.conf"

    chmod 600 "/Users/${USER}/Library/Application Support/A12coin/a12coin.conf"

The first time you run a12coind, it will start downloading the blockchain. This process could take several hours.

You can monitor the download process by looking at the debug.log file:

    tail -f $HOME/Library/Application\ Support/A12coin/debug.log

Other commands:
-------

    ./src/a12coind -daemon # Starts the a12coin daemon.
    ./src/a12coin-cli --help # Outputs a list of command-line options.
    ./src/a12coin-cli help # Outputs a list of RPC commands when the daemon is running.

Using Qt Creator as IDE
------------------------
You can use Qt Creator as an IDE, for a12coin development.
Download and install the community edition of [Qt Creator](https://www.qt.io/download/).
Uncheck everything except Qt Creator during the installation process.

1. Make sure you installed everything through Homebrew mentioned above
2. Do a proper ./configure --enable-debug
3. In Qt Creator do "New Project" -> Import Project -> Import Existing Project
4. Enter "a12coin-qt" as project name, enter src/qt as location
5. Leave the file selection as it is
6. Confirm the "summary page"
7. In the "Projects" tab select "Manage Kits..."
8. Select the default "Desktop" kit and select "Clang (x86 64bit in /usr/bin)" as compiler
9. Select LLDB as debugger (you might need to set the path to your installation)
10. Start debugging with Qt Creator

Notes
-----

* Tested on OS X 10.8 through 10.12 on 64-bit Intel processors only.

* Building with downloaded Qt binaries is not officially supported. See the notes in [#7714](https://github.com/bitcoin/bitcoin/issues/7714)
