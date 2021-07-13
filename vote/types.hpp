#pragma once

#include <eosio/eosio.hpp>
#include "consts.hpp"

using namespace eosio;

struct _token {

    name contract;

    symbol symbol;

    bool operator == (const _token token) const {
        return this->contract == token.contract && this->symbol == token.symbol;
    }

    bool operator != (const _token token) const {
        return !(*this == token);
    }
};

struct _token_asset {

    _token token;

    asset quantity;

    _token_asset() {
        check (token.symbol == quantity.symbol, "illegal symbol");
    }

    _token_asset(_token token, asset quantity) {
        this->token = token;
        this->quantity = quantity;
    }

    bool operator == (const _token_asset asset) const {
        return this->token == asset.token && this->quantity == quantity;
    }

    bool operator != (const _token_asset asset) const {
        return !(*this == asset);
    }
};

struct _pair {

    uint64_t id;
    name creator;

    _token token0;

    _token token1;

    asset reserve0;

    asset reserve1;

    asset liquidity;

    uint64_t price0_last;

    uint64_t price1_last;

    time_point_sec update_at;

    uint64_t primary_key() const { return id;}
};

struct _pair_info {

    uint64_t id;

    uint16_t fee0;

    name fee0_account;

    uint16_t fee1;

    name fee1_account;

    uint64_t primary_key() const { return id;}
};

struct _pair_system_info {
    uint64_t id;

    uint16_t fee0;

    name fee0_account;

    uint16_t fee1;

    name fee1_account;

    uint64_t primary_key() const { return id;}
};


