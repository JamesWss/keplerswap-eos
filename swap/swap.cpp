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
