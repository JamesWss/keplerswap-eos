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

struct _user_info {

    name owner;

    name inviter;

    int num;

    time_point_sec create_at;

    uint64_t primary_key() const { return owner.value; }
    uint64_t inviter_key() const { return inviter.value; }
};


uint128_t token2key(_token token) {
    return ((uint128_t)token.contract.value) << 64 | ((uint128_t)token.symbol.code().raw());
}

struct _transfer_info {

    uint64_t id;

    _token token;

    asset quantity;
    time_point_sec update_at;

    uint64_t primary_key() const { return id; }
    uint128_t token_key() const { return token2key(token); }
};

struct _id_info {

    uint64_t id;

    uint64_t current_id;

    uint64_t current_num;

    uint64_t primary_key() const { return id;}
};

struct _swap_info {

    uint64_t id;

    name owner;

    asset from_asset;

    asset pair_fee;

    asset system_fee;

    asset to_asset;

    uint64_t price;

    time_point_sec create_at;

    uint64_t primary_key() const { return id;}
    uint64_t owner_key() const { return owner.value;}
};

struct _destination_info {

    uint8_t id;

    _token token;

    name account;

    uint16_t rate;

    asset received;

    uint64_t primary_key() const { return id; }
    uint128_t token_key() const { return token2key(token); }
};


struct fee_transfer_info {

    uint64_t pair_id;

    name owner;
};

struct _lock_info {

    name owner;

    name inviter;

    uint64_t pair_id;

    asset liquidity;

    uint64_t shares;

    uint64_t debt0;

    uint64_t debt1;

    asset pending0;

    asset pending1;

    asset withdrawed0;

    asset withdrawed1;

    bool avaliable;

    uint64_t primary_key() const { return owner.value; }
    uint64_t owner_key() const { return owner.value; }
};


struct _lock_order {

    uint64_t id;

    uint64_t pair_id;

    name owner;

    name inviter;

    asset liquidity;

    uint8_t lock_type;

    uint8_t lock_ratio;

    uint32_t lock_seconds;

    time_point_sec lock_at;

    time_point_sec unlock_at;

    time_point_sec expire_at;

    uint64_t primary_key() const { return id; }
    uint64_t owner_key() const { return owner.value; }
    uint64_t inviter_key() const { return inviter.value; }
    uint64_t expire_key() const { return expire_at.utc_seconds; }
};

struct _pool_info {

    uint64_t pair_id;

    asset total_liquidity;

    uint64_t total_shares;

    uint128_t acc0;

    uint128_t acc1;

    uint64_t primary_key() const { return pair_id; }
};

struct _draw_info {

    uint64_t id;

    uint64_t pair_id;

    checksum256 first_hash;

    checksum256 last_hash;

    _token token0;

    _token token1;

    asset quantity0;

    asset quantity1;

    uint32_t mine_user_num;
    uint32_t invite_user_num;
    uint32_t total_user_num;

    uint64_t min_mine_shares;
    uint64_t min_invite_shares;

    int user_num;

    int claim_num;

    int legal_claim_num;

    int lucky_user_num;

    name best_user;

    int best_user_num;

    asset withdrawed_quantity0;

    asset withdrawed_quantity1;

    time_point_sec start_time;

    time_point_sec end_time;

    uint64_t primary_key() const { return id; }
    uint64_t pair_key() const { return pair_id; }
};

struct _draw_share {
    name owner;
    uint64_t shares;

    uint64_t primary_key() const { return owner.value; }
    uint64_t share_key() const { return shares; }
};

struct _draw_user_info {

    uint64_t draw_id;

    name owner;

    bool lucky;

    asset withdrawed0;
    asset withdrawed1;

    time_point_sec claim_at;

    uint64_t primary_key() const { return owner.value; }
};

struct _crycle_info {

    name owner;

    uint64_t pair_id;

    std::string title;

    std::string manifest;

    std::string telegram;
 
    uint64_t vote_id;
  
    uint64_t voted;
   
    uint32_t members;
    
    asset pending0;
    
    asset pending1;

    asset withdrawed0;

    asset withdrawed1;

    time_point_sec create_at;

    uint64_t primary_key() const { return owner.value; }
    uint64_t vote_key() const { return voted; }
    uint64_t create_key() const { return create_at.utc_seconds; }
};
