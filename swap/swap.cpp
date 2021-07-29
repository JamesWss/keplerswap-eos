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
