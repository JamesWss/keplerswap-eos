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
