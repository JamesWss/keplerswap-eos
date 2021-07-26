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

const int MAX_LOCK_TYPE = 4;

const int LOCK_SECONDS[] = {
    0,
    3 * 24 * 60 * 60,
    30 * 24 * 60 * 60,
    90 * 24 * 60 * 60,
    360 * 24 * 60 * 60
};

const int LOCK_TYPE_UNLOCK = 0;
const int LOCK_TYPE_360 = 4;

const int LOCK_RATIO[] = {
    3,
    30,
    10,
    15,
    30
};

const int UNLOCK_RATIO = LOCK_RATIO[LOCK_TYPE_UNLOCK];

const int MINER_USER_RATE = 8999;

const uint64_t SNAPSHOT_TYPE_VOTE = 1L;

#ifdef DEV
const name DEFAULT_FEE0_ACCOUNT = ""_n;
const name DEFAULT_FEE1_ACCOUNT = ""_n;
const name TOKEN_CONTRACT = "token"_n;
const name LPTOKEN_CONTRACT = "lp"_n;
const name SWAP_CONTRACT = "swap"_n;
const name MINER_CONTRACT = "miner"_n;
const name FEE_CONTRACT = "fee"_n;
const name DESTROY_CONTRACT = "destroy"_n;
const name VOTE_CONTRACT = "vote"_n;
const name EOSA_ISSUER = "eosio"_n;
const name FOMO_CONTRACT = "fomo"_n;
const name INVITER_CONTRACT = "inviter"_n;
const name SYSTEM_ACCOUNT = "system"_n;
#define DRAW_USER_PERCENT 100
#define EOSA_TOKEN_TRANSFER "token::transfer"
#define DRAW_CLOSE_SECONDS 10
#define VOTE_SECONDS 10
#define COUNT_SECONDS 10 
#define MAX_LUCKY_USER_NUM 11
#define MIN_LUCKY_USER_NUM 11
#else
const name INVITER_CONTRACT = "inviter"_n;
const name DEFAULT_FEE0_ACCOUNT = ""_n;
const name DEFAULT_FEE1_ACCOUNT = ""_n;
const name TOKEN_CONTRACT = "token"_n;
const name LPTOKEN_CONTRACT = "lp"_n;
const name SWAP_CONTRACT = "swap"_n;
const name MINER_CONTRACT = "miner"_n;
const name FEE_CONTRACT = "fee"_n;
const name DESTROY_CONTRACT = "destroy"_n;
const name VOTE_CONTRACT = "vote"_n;
const name EOSA_ISSUER = "eosio"_n;
const name FOMO_CONTRACT = "fomo"_n;
const name SYSTEM_ACCOUNT = "system"_n;
#define DRAW_USER_PERCENT 30
#define EOSA_TOKEN_TRANSFER "token::transfer"
#define DRAW_CLOSE_SECONDS 72 * 3600
#define VOTE_SECONDS 5 * 24 * 60 * 60 
#define COUNT_SECONDS 2 * 24 * 60 * 60
#define MAX_LUCKY_USER_NUM 11
#define MIN_LUCKY_USER_NUM 11
#endif

const char* const TOKEN = "SDS";
const int TOKEN_PRECISION = 4;
const uint64_t TOKEN_SCOPE = 5456979L;
const uint128_t TOKEN_TYPE_KEY = (uint128_t)TOKEN_SCOPE << 64 | (uint128_t)SNAPSHOT_TYPE_VOTE;
const uint64_t VOTE_TOKEN_EXCHANGE = 100L;

#define LIQUID_INFO_ID_KEY 1L
#define SWAP_INFO_ID_KEY 2L
#define LOCK_ORDER_ID_KEY 3L

#define SWAP_INFO_MAX_NUM 100L
