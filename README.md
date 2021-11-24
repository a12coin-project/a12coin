A12coin Core staging tree
=========================

https://a12coin.org

What is <img src="http://images.a12coin.org/a12coin32.png" width="32" height="32"/>coin?
================
A12coin is an experimental digital currency that enables instant payments to anyone, anywhere in the world.
A12coin uses peer-to-peer technology to operate with no central authority: managing transactions and issuing money are carried out collectively by the network. A12coin Core is the name of the open source software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of the A12coin Core software, see
https://wwww.a12coin.org

Usage
-----
To get started with A12coin Core, see the installation guide.

Used ports
----------
A12coin Core by default uses port 7333 for peer-to-peer communication that is needed to synchronize the "mainnet" blockchain and stay informed of new transactions and blocks.
Additionally, a RPC port can be opened, which defaults to port 7332 for mainnet nodes. It is strongly recommended to not expose RPC ports to the public internet.
| Function | mainnet | testnet | regnet |
| :------- | ------: | ------: | -----: |
| P2P      |    7333 |   27335 |  27444 |
| RPC      |    7332 |   27332 |  27443 |

License
-------
A12coin Core is released under the terms of the MIT license.
See [COPYING](COPYING) for more information or see [opensource.org](https://opensource.org/licenses/MIT)
