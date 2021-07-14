#pragma once

#include <cmath>

using namespace eosio;

#define E18 1000000000000000000L
#define E10 10000000000L
#define E8 100000000L
#define E4 10000L

const uint128_t MINE_ACC_RATIO = E18;

#define INIT_PAIR_ID 1L


const char LPTOKEN_SYMBOL_BASE[] = "LP";

const int LPTOKEN_SYMBOL_BASE_LENGTH = sizeof(LPTOKEN_SYMBOL_BASE) - 1; 

const int LPTOKEN_SYMBOL_MAX  = 4;

const char LPTOKEN_SYMBOL[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const int LPTOKEN_SYMBOL_RADIX = sizeof(LPTOKEN_SYMBOL) - 1;

const uint64_t LPTOKEN_MAXIMUM_SUPPLY = 1000000000000000000l;

const uint16_t DEFAULT_SYSTEM_FEE0 = 0;
const uint16_t DEFAULT_SYSTEM_FEE1 = 0;
