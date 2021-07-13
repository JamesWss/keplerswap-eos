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
