#include "network/types.h"
#include "network/trade_packet.h"
#include "network/register_packet.h"
#include "network/property_packet.h"
#include "network/jail_packet.h"
#include "network/GameState.h"

#include "network/packet_decoder.h"

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace net;

void print_data(const std::string& data) {

	std::string::const_iterator i = data.begin();
	std::string::const_iterator e = data.end();

	std::cout << "~~~ DATA ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	int x = 0;
	while(i!=e) {
		unsigned char c = *(i++);
		std::cout << std::setw(4) << std::left << x++;
		std::cout << "0x";
		std::cout << std::hex;
		std::cout << std::setw(12) << (uint)c;
		std::cout << std::dec;

		if(std::isprint(c)) {
			std::cout << '\'' << c << "'  ";
		} else {
			std::cout << "BIN  ";
		}

		if(x-1 == 0) {
			std::cout << "Start length field [" << sizeof(Length_t) << " bytes]";
		}

		if(x-1 == sizeof(Length_t)) {
			std::cout << "Start action field [" << sizeof(Action_t) << " bytes]";
		}

		if(x-1 == sizeof(Length_t) + sizeof(Action_t)) {
			std::cout << "Start data segment [" << data.size() - ( sizeof(Length_t) + sizeof(Action_t) ) << " bytes]";
		}

		std::cout << std::endl;
	}
	std::cout << "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/" << std::endl;
}

void test_register() {
	std::cout << "test_register ==============================================" << std::endl;
	const char* str = "The cow likes to be milked";
	Register reg(str);

	const std::string & data = reg.encode();
	std::cout << "data-size=" << data.size() << std::endl;
	print_data(data);

	Decoder d(data);

	Register reg2(&d);
	std::cout << "register-name("<< str <<")=" << reg2.get_name() << std::endl;
	std::cout << "============================================================" << std::endl;
}

void test_jail() {
	State_t j = JS_CARD;

	std::cout << "test_jail ==================================================" << std::endl;
	Jail jail(j);

	const std::string & data = jail.encode();
	std::cout << "data-size=" << data.size() << std::endl;
	print_data(data);

	Decoder d(data);

	Jail jail2(&d);
	
	std::cout << "jail-state(" << (int)j << ")=" << (int)jail2.get_jail_state() << std::endl;
	std::cout << "============================================================" << std::endl;
}


void test_property() {
	Property p(25, 10);
	std::cout << "test_property ==============================================" << std::endl;

	const std::string & data = p.encode();
	std::cout << "data-size=" << data.size() << std::endl;
	print_data(data);

	Decoder d(data);
	Property p2(&d);	

	std::cout << "p-prop(25)=" << (int)p2.get_number() << std::endl;
	std::cout << "p-level(10)=" << (int)p2.get_level() << std::endl;

	std::cout << "============================================================" << std::endl;
}

void test_trade1() {
	Trade p(1, 2, 3, 69, 100);
	std::cout << "test_trade1 =================================================" << std::endl;

	const std::string & data = p.encode();
	std::cout << "data-size=" << data.size() << std::endl;
	print_data(data);

	Decoder d(data);
	Trade p2(&d);
	
	std::cout << "srcplay("<< (int)p.get_source_player() <<")=" << (int)p2.get_source_player() << std::endl;
	std::cout << "dstplay("<< (int)p.get_destin_player() <<")=" << (int)p2.get_destin_player() << std::endl;
	std::cout << "state("<< (int)p.get_trade_state() <<")=" << (int)p2.get_trade_state() << std::endl;
	std::cout << "offmoney("<< (int)p.get_offered_money() <<")=" << (int)p2.get_offered_money() << std::endl;
	std::cout << "reqmoney("<< (int)p.get_request_money() <<")=" << (int)p2.get_request_money() << std::endl;

	const PropertyList_t & p2offlist = p2.get_offered_properties();
	std::cout << "offlist.size("<< p.get_offered_properties().size() <<")=" << p2offlist.size() << std::endl;
	
	const PropertyList_t & p2reqlist = p2.get_requested_properties();
	std::cout << "reqlist.size("<< p.get_requested_properties().size() <<")=" << p2reqlist.size() << std::endl;


	std::cout << "============================================================" << std::endl;
}

void test_trade2() {
	Trade p(13, 23, 33, 693, 1003);
	p.offer_property(40);
	p.offer_property(41);
	p.offer_property(50);
	p.offer_property(45);

	p.request_property(12);
	p.request_property(23);
	p.request_property(77);
	
	std::cout << "test_trade2 =================================================" << std::endl;

	const std::string & data = p.encode();
	std::cout << "data-size=" << data.size() << std::endl;
	print_data(data);

	Decoder d(data);
	Trade p2(&d);

	std::cout << "srcplay("<< (int)p.get_source_player() <<")=" << (int)p2.get_source_player() << std::endl;
	std::cout << "dstplay("<< (int)p.get_destin_player() <<")=" << (int)p2.get_destin_player() << std::endl;
	std::cout << "state("<< (int)p.get_trade_state() <<")=" << (int)p2.get_trade_state() << std::endl;
	std::cout << "offmoney("<< (int)p.get_offered_money() <<")=" << (int)p2.get_offered_money() << std::endl;
	std::cout << "reqmoney("<< (int)p.get_request_money() <<")=" << (int)p2.get_request_money() << std::endl;

	const PropertyList_t & p2offlist = p2.get_offered_properties();
	std::cout << "offlist.size("<< p.get_offered_properties().size() <<")=" << p2offlist.size() << std::endl;
	
	const PropertyList_t & p2reqlist = p2.get_requested_properties();
	std::cout << "reqlist.size("<< p.get_requested_properties().size() <<")=" << p2reqlist.size() << std::endl;


	std::cout << "============================================================" << std::endl;
}

void test_gamestate1() {
	// create a gamestate
	GameState state;

	{
		PropertyList_t owned;
		owned.push_back(10);
		owned.push_back(15);

		PlayerState player(1, "Mike", 3, 24, owned);
		state.add_player(player);
	}

	{
		PropertyList_t owned;
		owned.push_back(5);
		owned.push_back(96);
		owned.push_back(12);

		PlayerState player(2, "Kyle", 13, 4, owned);
		std::cout << "kyle has id = " << (int)player.get_id() << std::endl;
		state.add_player(player);
	}

	{
		PropertyList_t owned;
		owned.push_back(5);
		owned.push_back(100);
		owned.push_back(12);

		PlayerState player(3,"John", 13, 4, owned);
		state.add_player(player);
	}

	state.add_property( PropertyState(20, 4, 1) );
	state.add_property( PropertyState(21, 5, 7) );

	state.set_current_turn("Mike");

	const std::string & data = state.encode();
	std::cout << "data-size=" << data.size() << std::endl;
	print_data(data);





	Decoder d(data);
	GameState state2(&d);

	if(state2.player_exists("Mike")) {
		std::cout << "Has Mike (good)" << std::endl;
	}

	if(state2.player_exists("Kyle")) {
		std::cout << "Has Kyle (good)" << std::endl;
	}

	if(!state2.player_exists("Dave")) {
		std::cout << "Does not have Dave (good)" << std::endl;
	}

	std::cout << "current turn (Mike) = " << state2.get_current_turn() << std::endl << std::endl;
	std::cout << "going to next turn..." << std::endl;
	state2.next_turn();
	std::cout << "current turn = " << state2.get_current_turn() << std::endl << std::endl;
	std::cout << "going to next turn..." << std::endl;
	state2.next_turn();
	std::cout << "current turn = " << state2.get_current_turn() << std::endl << std::endl;
	std::cout << "going to next turn..." << std::endl;
	state2.next_turn();
	std::cout << "current turn (Mike) = " << state2.get_current_turn() << std::endl << std::endl;

	std::cout << "player count (3) = " << state2.get_player_count() << std::endl;

	const PlayerState & kyle = state2.get_player("Kyle");

	std::cout << "Kyle.id(2) = " << (int)kyle.get_id() << std::endl;
	std::cout << "Kyle.money(13) = " << (int)kyle.get_money() << std::endl;
	std::cout << "Kyle.pos(4) = " << (int)kyle.get_position() << std::endl;

	const PropertyList_t & kyleprops = kyle.get_owned_properties();

	std::cout << "Kyle.props.size (3) = " << kyleprops.size() << std::endl;
	PropertyList_t::const_iterator i = kyleprops.begin();
	std::cout << "kyle.props (12,96,5) = " << (int)*(i++)
									 << "," << (int)*(i++)
										<< "," << (int)*(i++) << std::endl;

	
}

void test_gamestate2() {

	// create a gamestate
	GameState state;

	std::cout << "find_unused_id(0) == " << (int)state.find_unused_id(8) << std::endl;
	{
		PlayerState player(0, "Mike", 3, 24);
		state.add_player(player);
	}
	std::cout << "find_unused_id(1) == " << (int)state.find_unused_id(8) << std::endl;
	{
		PlayerState player(1, "Bob", 3, 24);
		state.add_player(player);
	}
	{
		PlayerState player(2, "Tom", 3, 24);
		state.add_player(player);
	}
	std::cout << "find_unused_id(3) == " << (int)state.find_unused_id(8) << std::endl;
	std::cout << "find_unused_id(0)[max = 0] == " << (int)state.find_unused_id(0) << std::endl;
	std::cout << "find_unused_id(1)[max = 1] == " << (int)state.find_unused_id(1) << std::endl;
	std::cout << "find_unused_id(2)[max = 2] == " << (int)state.find_unused_id(2) << std::endl;
	std::cout << "find_unused_id(3)[max = 3] == " << (int)state.find_unused_id(3) << std::endl;
	std::cout << "find_unused_id(3)[max = 4] == " << (int)state.find_unused_id(4) << std::endl;
	std::cout << "find_unused_id(3)[max = 5] == " << (int)state.find_unused_id(5) << std::endl;

	std::cout << "deleting bob, id = 1" << std::endl;
	state.del_player("Bob");
	std::cout << "find_unused_id(1)[max = 5] == " << (int)state.find_unused_id(5) << std::endl;
	std::cout << "deleting Tom, id = 2" << std::endl;
	state.del_player("Tom");
	std::cout << "find_unused_id(1)[max = 5] == " << (int)state.find_unused_id(5) << std::endl;
	std::cout << "deleting Mike, id = 0" << std::endl;
	state.del_player("Mike");
	std::cout << "find_unused_id(0)[max = 5] == " << (int)state.find_unused_id(5) << std::endl;

}

void test_trade() {
	test_trade1();
	std::cout << std::endl;
	test_trade2();
}

void test_gamestate() {
	test_gamestate1();
	std::cout << std::endl;
	test_gamestate2();
}

int main(int argc, char*argv[])
{
	test_register();
	std::cout << std::endl;
	test_jail();
	std::cout << std::endl;
	test_property();
	std::cout << std::endl;
	test_trade();
	std::cout << std::endl;
	test_gamestate();
	std::cout << std::endl;
	

	return 0;
}
