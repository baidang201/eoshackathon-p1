#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <string>
#include <eosiolib/contract.hpp>

using namespace eosio;
using namespace std;


class charge : public eosio::contract {
	static const account_name charge_info_account = N(charge.acct);

	private:
    	//@abi table chargeinfos i64
        struct charge_info {
        	uint64_t paymentcode;
        	account_name payer;
            account_name charger;
            asset quantity;
            uint64_t state;

            uint64_t primary_key() const { return paymentcode; }

        };

        typedef eosio::multi_index <charge_info_account, charge_info> chargeinfos;

        

    public:
        void transfer(account_name from, account_name to, asset quantity, string memo){
            require_auth( from );

            print( "transfer from ", name{from}, " to ", name{to} );
    
            action(
                permission_level{from, N(active)},
                N(eosio.token), N(transfer),
                std::make_tuple(from, to, quantity, memo)
            ).send();
        }

        using eosio::contract::contract;
        void reqcharge(account_name charger, asset quantity, uint64_t payment_code, account_name payer) {
            eosio::print(charger);
            eosio::print(quantity);
            eosio::print(payment_code);
            eosio::print(payer);
            eosio_assert(quantity.is_valid(), "invalid quantity");
        	eosio_assert(quantity.amount > 0, "must issue positive quantity");
        	chargeinfos exist_charge_infos = chargeinfos(_self, payer);
        	exist_charge_infos.emplace(charger, [&](auto &d) {
            	d.charger = charger;
            	d.quantity = quantity;
            	d.paymentcode = payment_code;
                d.state = 0;
        	});
        }


        void printinfo(uint64_t payment_code, account_name payer) {
        	chargeinfos exist_charge_infos = chargeinfos(_self, payer);
        	charge_info info = exist_charge_infos.get(payment_code);
			eosio::print(info.paymentcode);
            eosio::print(info.state);
        }

        void confirm(uint64_t payment_code, account_name payer) {
            chargeinfos exist_charge_infos = chargeinfos(_self, payer);
            charge_info info = exist_charge_infos.get(payment_code);
            require_auth(info.payer);
            transfer(info.payer, info.charger, info.quantity, "");
            exist_charge_infos.modify(info, info.payer, [&]( auto& d) {
                d.state = 1;     // customer provided input
            });
        }

    


};

EOSIO_ABI( charge, (reqcharge)(printinfo)(transfer) )