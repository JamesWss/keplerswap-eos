#include <string>

#include "swap.h"
#include "../methods.hpp"

void swap::transfer(name from, name to, asset quantity, std::string memo) {
    if (from == _self) return;
    if(to != _self) return;
    name contract = get_first_receiver();
    transinfo_index transinfo_table(_self, from.value);
    auto idx = transinfo_table.get_index<"token"_n>();
    _token token{contract, quantity.symbol};
    auto transinfo_idx = idx.find(token2key(token));
    uint64_t current_id = 1;
    if (transinfo_table.begin() != transinfo_table.end()) {
        current_id = transinfo_table.rbegin()->id + 1;
    }
    if (transinfo_idx == idx.end()) {
        transinfo_table.emplace(_self, [&](auto& row) {
            row.id = current_id;
            row.token = token;
            row.quantity = quantity;
            row.update_at = publication_time();
        });
    } else {
        idx.modify(transinfo_idx, _self, [&](auto& row) {
            row.quantity += quantity;
            row.update_at = publication_time();
        });
    }
}

void swap::registe(name owner, name inviter) {
    require_auth(owner);
    whitelist_index whitelist_table(_self, _self.value);
    auto whitelist_idx = whitelist_table.find(_self.value);
    if (whitelist_idx != whitelist_table.end()) { //白名单功能打开了,需要判断是否在白名单中
        whitelist_idx = whitelist_table.find(owner.value);
        check (whitelist_idx != whitelist_table.end(), "user not avaliable for not in whitelist");
    }
    userinfo_index userinfo_table(_self, _self.value);    
    auto userinfo_idx = userinfo_table.find(owner.value);
    check (userinfo_idx == userinfo_table.end(), "user already registed");
    if (userinfo_table.begin() == userinfo_table.end()) {
        check (inviter == _self, "first user's inviter need to be contract");
    } else {
        auto inviter_idx = userinfo_table.find(inviter.value);
        check (inviter_idx != userinfo_table.end(), "inviter not registed");
        userinfo_table.modify(inviter_idx, _self, [&](auto& row) {
            row.num += 1;
        });
    }
    userinfo_table.emplace(_self, [&](auto& row) {
        row.owner = owner;
        row.inviter = inviter;
        row.num = 0;
        row.create_at = publication_time();
    });
}

void swap::createpair(name creator, token token0, token token1) {
    //require_auth(_self); //目前只允许本合约创建交易对
    require_auth(creator);
    userinfo_index userinfo_table(_self, _self.value);
    auto userinfo_index = userinfo_table.find(creator.value);
    check (userinfo_index != userinfo_table.end(), "user not registed");
    pairs_index pairs_table(_self, _self.value);
    uint64_t pair_id = 0;
    if (pairs_table.begin() == pairs_table.end()) {
        pair_id = INIT_PAIR_ID;
    } else {
        pair_id = pairs_table.rbegin()->id + 1;
    }
    uint16_t lptoken_precision = std::max(token0.symbol.precision(), token1.symbol.precision());
    symbol lptoken_symbol = create_lptoken_symbol(pair_id, lptoken_precision);
    //生成pair
    pairs_table.emplace(creator, [&](auto& row) {
        row.id = pair_id;
        row.creator = creator;
        row.token0 = token0;
        row.token1 = token1;
        row.reserve0 = asset(0, token0.symbol);
        row.reserve1 = asset(0, token1.symbol);
        row.liquidity = asset(0, lptoken_symbol);
        row.price0_last = 0;
        row.price1_last = 0;
        row.update_at = publication_time();
    });
    //生成pair配置信息:用户配置
    pairinfo_index pairinfo_table(_self, _self.value);
    auto pairinfo_idx = pairinfo_table.find(pair_id);
    if (pairinfo_idx == pairinfo_table.end()) {
        pairinfo_table.emplace(creator, [&](auto& row) {
            row.id = pair_id;
            row.fee0 = 0;
            row.fee1 = 0;
        });
    } else {
        pairinfo_table.modify(pairinfo_idx, creator, [&](auto& row) {
            row.fee0 = 0;
            row.fee1 = 0;
        });
    }
    //生成pair配置信息:系统配置
    pairsinfo_index pairsinfo_table(_self, _self.value);
    auto pairsinfo_idx = pairsinfo_table.find(pair_id);
    if (pairsinfo_idx == pairsinfo_table.end()) {
        pairsinfo_table.emplace(_self, [&](auto& row) {
            row.id = pair_id;
            row.fee0 = DEFAULT_SYSTEM_FEE0;
            row.fee0_account = DEFAULT_FEE0_ACCOUNT;
            row.fee1 = DEFAULT_SYSTEM_FEE1;
            row.fee1_account = DEFAULT_FEE1_ACCOUNT;
        });
    } else {
        pairsinfo_table.modify(pairsinfo_idx, _self, [&](auto& row) {
            row.fee0 = DEFAULT_SYSTEM_FEE0;
            row.fee0_account = DEFAULT_FEE0_ACCOUNT;
            row.fee1 = DEFAULT_SYSTEM_FEE1;
            row.fee1_account = DEFAULT_FEE1_ACCOUNT;
        });
    }
    //创建LP
    action(
        permission_level{LPTOKEN_CONTRACT, "active"_n},
        LPTOKEN_CONTRACT,
        "create"_n,
        std::make_tuple(_self, asset(LPTOKEN_MAXIMUM_SUPPLY, lptoken_symbol))
    ).send();
    //记录创建结果
    action(
        permission_level{_self, "active"_n},
         _self,
        "createlog"_n,
        std::make_tuple(pair_id, creator, lptoken_symbol, token0, token1)
    ).send();
}
