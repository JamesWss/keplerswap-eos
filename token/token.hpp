#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>
#include "../types.hpp"

namespace eosiosystem {
   class system_contract;
}

using namespace eosio;
using std::string;
/**
* eosio.token contract defines the structures and actions that allow users to create, issue, and manage
* tokens on EOSIO based blockchains.
*/
class [[eosio::contract("token")]] token : public contract {
  public:
     using contract::contract;
};
