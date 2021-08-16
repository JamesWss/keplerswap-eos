#pragma once

#include <memory>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include <eosio/fixed_bytes.hpp>

#include "consts.hpp"
#include "types.hpp"

symbol create_lptoken_symbol(uint64_t id, uint16_t precision) {
	char str[LPTOKEN_SYMBOL_MAX + 1] = {0};
	unsigned unum = id - 1;
	int i = 0,j; 
	do { 
		str[i++] = LPTOKEN_SYMBOL[unum % (unsigned)LPTOKEN_SYMBOL_RADIX]; 
		unum /= LPTOKEN_SYMBOL_RADIX; 
	} while(unum); 
	str[i] = '\0'; 
	char temp; 
	for(j = 0; j <= (i-1)/2; j++) { 
		temp = str[j]; 
		str[j] = str[i-j-1]; 
		str[i-j-1] = temp; 
	} 
    char res[LPTOKEN_SYMBOL_MAX + 1] = {0};
    sprintf(res, "%s%s", LPTOKEN_SYMBOL_BASE, str);
	return symbol(res, precision);
}

fee_transfer_info  parse_fee_transfer_memo(const std::string& memo) {
    const char* str = memo.data();
    int pair_id_end_position = 0;
    for (int i = 0; i < memo.length(); i ++) {
        if (str[i] == ',') {
            pair_id_end_position = i;
            break;
        }
    }
    if (pair_id_end_position == 0) {
        return fee_transfer_info{0, ""_n};
    }
    char tmp[16] = {0};
    memcpy(tmp, str, pair_id_end_position);
    uint64_t pair_id = atoll(tmp);
    memset(tmp, 0, 16);
    memcpy(tmp, str + pair_id_end_position + 1, memo.length() - pair_id_end_position - 1);
    name owner(tmp);
    return fee_transfer_info{pair_id, owner};
}

checksum256 get_current_hash() {
    auto size = transaction_size();
    char buf[size];
    uint32_t read = read_transaction(buf, size);
    check (size == read, "read transaction failed");
    return sha256(buf, read);
}
