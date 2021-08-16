#include <string>
#include <algorithm>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

#include "../types.hpp"

using namespace eosio;

class [[eosio::contract]] swap : public contract {
    public:
        using contract::contract;

        struct [[eosio::table]] token : public _token {};
        struct [[eosio::table]] pair : public _pair {};
        struct [[eosio::table]] pairinfo : public _pair_info {};
        struct [[eosio::table]] pairsinfo : public _pair_system_info {};
        struct [[eosio::table]] userinfo : public _user_info {};
        struct [[eosio::table]] transinfo : public _transfer_info {};
        struct [[eosio::table]] idinfo : public _id_info {};
        struct [[eosio::table]] swapinfo : public _swap_info {};
        struct [[eosio::table]] whitelist : public _whitelist {};
        struct [[eosio::table]] destinfo : public _destination_info {};

        swap(name receiver, name code, datastream<const char*> ds) : 
            contract(receiver, code, ds)
        {}

        [[eosio::on_notify("*::transfer")]] 
        void transfer(name from, name to, asset quantity, std::string memo);

        [[eosio::action]]
        void registe(name owner, name inviter);

        [[eosio::action]]
        void createpair(name creator, token token0, token token1);

        [[eosio::action]]
        void createlog(uint64_t pair_id, name creator, symbol lpToken, token token0, token token1) {
            require_auth(_self);
        }
    
        [[eosio::action]]
        void setfee(uint64_t pair_id, uint16_t fee0, name fee0_account, uint16_t fee1, name fee1_account);

        [[eosio::action]]
        void setsfee(uint64_t pair_id, uint16_t fee0, name fee0_account, uint16_t fee1, name fee1_account);

        [[eosio::action]]
        void deposit(name owner, uint64_t pair_id);

        [[eosio::action]]
        void depositlog(
            name owner, 
            uint64_t pair_id, 
            asset token0, 
            asset token1, 
            asset liquidity, 
            asset reserve0, 
            asset reserve1, 
            asset total_liquidity
        ) {
            require_auth(_self);
        }
    
        [[eosio::action]]
        void swaptoken(name owner, _token token, asset min_amount, uint64_t pair_id);

        [[eosio::action]]
        void swaplog(name owner, uint64_t pair_id, uint64_t info_id, asset from, asset system_fee, asset pair_fee, asset to, uint64_t price) {
            require_auth(_self);
        }

        [[eosio::action]]
        void withdraw(name owner, uint64_t pair_id);

        [[eosio::action]]
        void withdrawlog(
            name owner, 
            uint64_t pair_id, 
            asset token0, 
            asset token1, 
            asset liquidity, 
            asset reserve0, 
            asset reserve1, 
            asset total_liquidity
        ) {
            require_auth(_self);
        }
};
