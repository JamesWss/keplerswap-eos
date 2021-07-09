all:
	cd swap && eosio-cpp swap.cpp -o swap.wasm -I.
	cd token && eosio-cpp token.cpp -o token.wasm -I.
	cd inviter && eosio-cpp inviter.cpp -o inviter.wasm -I.
	cd destroy && eosio-cpp destroy.cpp -o destroy.wasm -I.
	cd miner && eosio-cpp miner.cpp -o miner.wasm -I.
	cd fomo && eosio-cpp fomo.cpp -o fomo.wasm -I.
	cd vote && eosio-cpp vote.cpp -o vote.wasm -I.
dev:
	cd swap && eosio-cpp swap.cpp -o swap.wasm -I. -DDEV
	cd token && eosio-cpp token.cpp -o token.wasm -I. -DDEV
	cd inviter && eosio-cpp inviter.cpp -o inviter.wasm -I. -DDEV
	cd destroy && eosio-cpp destroy.cpp -o destroy.wasm -I. -DDEV
	cd miner && eosio-cpp miner.cpp -o miner.wasm -I. -DDEV
	cd fomo && eosio-cpp fomo.cpp -o fomo.wasm -I. -DDEV
	cd vote && eosio-cpp vote.cpp -o vote.wasm -I. -DDEV
clean:
	cd swap && rm -f *.abi && rm -f *.wasm
	cd token && rm -f *.abi && rm -f *.wasm
	cd inviter && rm -f *.abi && rm -f *.wasm
	cd destroy && rm -f *.abi && rm -f *.wasm
	cd miner && rm -f *.abi && rm -f *.wasm
	cd fomo && rm -f *.abi && rm -f *.wasm
	cd vote && rm -f *.abi && rm -f *.wasm
