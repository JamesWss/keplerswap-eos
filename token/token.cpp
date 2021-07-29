#include "token.hpp"
#include "../consts.hpp"

void token::create( const name&   issuer,
                    const asset&  maximum_supply )
{
    require_auth( get_self() );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( get_self(), [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}

void token::issue( const name& to, const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;
    check( to == st.issuer, "tokens can only be issued to issuer account" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );
}

void token::retire( const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}

void token::transferall( const name&    owner,
                      const name&    to,
                      const symbol& symbol,
                      const string&  memo )
{
   accounts from_acnts( get_self(), owner.value );
   const auto& from = from_acnts.get( symbol.code().raw(), "no balance object found" );
    action(
        permission_level{owner, "active"_n},
        _self,
        "transfer"_n,
        std::make_tuple(owner, to, from.balance, memo)
    ).send();
}

void token::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable( get_self(), sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );
}

void token::sub_balance( const name& owner, const asset& value ) {
   accounts from_acnts( get_self(), owner.value );

   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
         if (a.balance.amount == 0 && value.symbol == symbol(TOKEN, TOKEN_PRECISION)) {
            stats statstable( get_self(), value.symbol.code().raw() );
            auto existing = statstable.find( value.symbol.code().raw() );
            statstable.modify(existing, _self, [&](auto& row) {
              row.accounts -= 1;
            });
          }
          if (value.symbol == symbol(TOKEN, TOKEN_PRECISION)) {
            
            snapshot_index snapshot_table(_self, value.symbol.code().raw());
            auto snapshot_idx = snapshot_table.begin();
            while (snapshot_idx != snapshot_table.end()) {
                usersnapshot_index usersnapshot_table(_self, owner.value);
                auto idx = usersnapshot_table.get_index<"symboltype"_n>();
                auto usersnapshot_idx = idx.find(symbolAndType2key(value.symbol, snapshot_idx->type));
                if (usersnapshot_idx == idx.end() || usersnapshot_idx->symb != value.symbol || usersnapshot_idx->type != snapshot_idx->type) {
                    usersnapshot_table.emplace(owner, [&](auto& row) {
                        row.owner = owner;
                        row.symb = value.symbol;
                        row.type = snapshot_idx->type;
                        row.snapshot = a.balance + value;
                        row.update_at = publication_time();
                    });
                } else if (usersnapshot_idx->update_at < snapshot_idx->create_at) { 
                    idx.modify(usersnapshot_idx, owner, [&](auto& row) {
                        row.snapshot = a.balance + value;
                        row.update_at = publication_time();
                    });
                }
                snapshot_idx ++;
            }
        }
      });
}

void token::add_balance( const name& owner, const asset& value, const name& ram_payer )
{
   accounts to_acnts( get_self(), owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      if (value.symbol == symbol(TOKEN, TOKEN_PRECISION)) {
        stats statstable( get_self(), value.symbol.code().raw() );
        auto existing = statstable.find( value.symbol.code().raw() );
        statstable.modify(existing, _self, [&](auto& row) {
          row.accounts += 1;
        });
        
        snapshot_index snapshot_table(_self, value.symbol.code().raw());
        auto snapshot_idx = snapshot_table.begin();
        while (snapshot_idx != snapshot_table.end()) {
            usersnapshot_index usersnapshot_table(_self, owner.value);
            auto idx = usersnapshot_table.get_index<"symboltype"_n>();
            auto usersnapshot_idx = idx.find(symbolAndType2key(value.symbol, snapshot_idx->type));
            if (usersnapshot_idx == idx.end() || usersnapshot_idx->symb != value.symbol || usersnapshot_idx->type != snapshot_idx->type) { 
                usersnapshot_table.emplace(ram_payer, [&](auto& row) {
                    row.owner = owner;
                    row.symb = value.symbol;
                    row.type = snapshot_idx->type;
                    row.snapshot = asset(0, value.symbol);
                    row.update_at = publication_time();
                });
            } else if (usersnapshot_idx->update_at < snapshot_idx->create_at) { 
                idx.modify(usersnapshot_idx, ram_payer, [&](auto& row) {
                    row.snapshot = asset(0, value.symbol);
                    row.update_at = publication_time();
                });
                
            }
            snapshot_idx ++;
        }
      }
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        if (a.balance.amount == 0 && value.symbol == symbol(TOKEN, TOKEN_PRECISION)) {
          stats statstable( get_self(), value.symbol.code().raw() );
          auto existing = statstable.find( value.symbol.code().raw() );
          statstable.modify(existing, _self, [&](auto& row) {
            row.accounts += 1;
          });
        }
        if (value.symbol == symbol(TOKEN, TOKEN_PRECISION)) {
            
            snapshot_index snapshot_table(_self, value.symbol.code().raw());
            auto snapshot_idx = snapshot_table.begin();
            while (snapshot_idx != snapshot_table.end()) {
                usersnapshot_index usersnapshot_table(_self, owner.value);
                auto idx = usersnapshot_table.get_index<"symboltype"_n>();
                auto usersnapshot_idx = idx.find(symbolAndType2key(value.symbol, snapshot_idx->type));
                if (usersnapshot_idx == idx.end() || usersnapshot_idx->symb != value.symbol || usersnapshot_idx->type != snapshot_idx->type) { 
                    usersnapshot_table.emplace(ram_payer, [&](auto& row) {
                        row.owner = owner;
                        row.symb = value.symbol;
                        row.type = snapshot_idx->type;
                        row.snapshot = a.balance;
                        row.update_at = publication_time();
                    });
                } else if (usersnapshot_idx->update_at < snapshot_idx->create_at) { 
                    idx.modify(usersnapshot_idx, ram_payer, [&](auto& row) {
                        row.snapshot = a.balance;
                        row.update_at = publication_time();
                    });
                }
                snapshot_idx ++;
            }
        }
        a.balance += value;
      });
   }
}
