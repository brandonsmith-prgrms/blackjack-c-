#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <ctime>

namespace N {
	std::string get_card_name(int val, int index) {
		std::string name;
		switch (val) {
		case 2:
			name = "Two of ";
			break;
		case 3:
			name = "Three of ";
			break;
		case 4:
			name = "Four of ";
			break;
		case 5:
			name = "Five of ";
			break;
		case 6:
			name = "Six of ";
			break;
		case 7:
			name = "Seven of ";
			break;
		case 8:
			name = "Eight of ";
			break;
		case 9:
			name = "Nine of ";
			break;
		case 10:
			if (index % 13 == 10) {
				name = "Ten of ";
			}
			else if (index % 13 == 11) {
				name = "Jack of ";
			}
			else if (index % 13 == 12) {
				name = "Queen of ";
			}
			break;
		case 11:
			name = "Ace of ";
		}
		if (name == "") {
			name = "King of ";
		}
		if (index % 4 == 0) {
			name = name + "Spades";
		}
		else if (index % 4 == 1) {
			name = name + "Hearts";
		}
		else if (index % 4 == 2) {
			name = name + "Diamonds";
		}
		else if (index % 4 == 3) {
			name = name + "Clubs";
		}
		return name;
	}


	class Card {
	public:
		//Setters:
		void set_raw(int val) {
			raw_value = val;
		}
		void set_name(std::string str) {
			name = str;
		}
		void set_state(bool val) {
			dealt = val;
		}
		void set_place(int num) {
			place = num;
		}

		//Getters:
		int get_raw() {
			return raw_value;
		}
		std::string get_name() {
			return name;
		}
		bool get_state() {
			return dealt;
		}
		int get_place() {
			return place;
		}
	private:
		int raw_value;
		int place;
		std::string name;
		bool dealt = false;
	};

	class Deck {
	public:
		Deck() {
			for (int index = 0; index < 52; index++) {
				int val = ((index + 1) % 13) <= 10 ? (index + 1) % 13 : 10;
				if (val == 0) {
					val = 10;
				}
				else if (val == 1) {
					val = 11;
				}
				deck[index].set_raw(val);
				deck[index].set_name(get_card_name(val, index));
				deck[index].set_place(index);
			}
		}
		Card draw_card() {
			int num;
			do {
				num = rand() % 52;
			} while (deck[num].get_state() == true);
			deck[num].set_state(true);
			return deck[num];
		}
		void shuffle_deck() {
			for (int i = 0; i < 52; i++) {
				deck[i].set_state(false);
			}
		}
	private:
		Card deck[52];
	};

	class Hand {
	public:
		Hand() {
			cards = new Card[20];
		}
		void clear_hand() {
			delete[] cards;
			cards = new Card[20];
			value = 0;
			hand_size = 0;
		}
		void set_owner(int num) {
			owner = num;
		}
		void resize_hand() {
			int max_size = hand_size * 2;
			Card* temp = new Card[max_size];
			for (int i = 0; i < hand_size; i++) {
				temp[i] = cards[i];
			}
			clear_hand();
			cards = temp;
		}
		void add_card(Card to_add) {
			cards[hand_size++] = to_add;
		}
		bool has_bust() {
			return (value > 21);
		}
		void update_val() {
			int sum = 0;
			for (int i = 0; i < hand_size; i++) {
				if (cards[i].get_state()) {
					int val = cards[i].get_raw();
					if (val != 11) {
						sum += val;
					}
					else {
						if (sum + 11 < 22) {
							sum += val;
						}
						else {
							sum += 1;
						}
					}
				}
			}
			value = sum;
		}
		int get_value() {
			return value;
		}
		void print_hand() {
			if (owner == 0) {
				for (int i = 0; i < hand_size; i++) {
					if (i > 1) {
						std::cout << " and a " << cards[i].get_name();
					}
					else if (i == 1) {
						std::cout << "The dealer has a " << cards[i].get_name();
					}
				}
				std::cout << " showing." << std::endl;
			}
			else {
				std::cout << "You have a ";
				for (int i = 0; i < hand_size; i++) {
					if (i != 0) {
						std::cout << " and a " << cards[i].get_name();
					}
					else {
						std::cout << cards[i].get_name();
					}
				}
				std::cout << std::endl;
			}
		}
	private:
		int owner;
		int value;
		int hand_size = 0;
		Card* cards;
	};

	class Game {
	public:
		Game() {
			srand(time(NULL));
			std::cout << "Press any key and then the enter key to begin." << std::endl;
			char input;
			std::cin >> input;
			std::cin.get();
			player.set_owner(1);
			dealer.set_owner(0);
		}

		bool continue_game() {
			std::cout << "Would you like to hit or stay? (h/s)" << std::endl;
			char input;
			std::cin >> input;
			return (input == 'h');
		}

		void run_game() {
			bool res;
			do {
				player.clear_hand();
				dealer.clear_hand();
				res = deal_init();
			} while (res == true);
			std::cout << "Thanks for playing!" << std::endl;
		}

		bool deal_init() {
			if (cards_dealt > 38) {
				game_deck.shuffle_deck();
				cards_dealt = 0;
			}
			for (int i = 0; i < 2; i++) {
				Card card = game_deck.draw_card();
				dealer.add_card(card);
				cards_dealt++;
			}
			for (int i = 0; i < 2; i++) {
				Card card = game_deck.draw_card();
				player.add_card(card);
				cards_dealt++;
			}
			dealer.update_val();
			player.update_val();
			dealer.print_hand();
			player.print_hand();
			std::cout << "For a value of: " << player.get_value() << std::endl;
			bool ended = false;
			if (player.get_value() == 21) {
				ended = true;
				if (dealer.get_value() == 21) {
					std::cout << "The dealer has won with 21." << std::endl;
				}
				else {
					std::cout << "You got blackjack, you won." << std::endl;
					std::cout << "The dealer had: " << dealer.get_value() << std::endl;
				}
			}
			else if (dealer.get_value() == 21) {
				ended = true;
				std::cout << "The dealer won with blackjack." << std::endl;
			}
			if (!ended) {
				if (continue_game()) {
					do {
						Card card = game_deck.draw_card();
						player.add_card(card);
						cards_dealt++;
						player.update_val();
						std::cout << "You were dealt a: " << card.get_name() << std::endl;
						std::cout << "You now have: " << player.get_value() << std::endl;
						if (player.get_value() == 21) {
							ended = true;
							std::cout << "You won with 21!" << std::endl;
							std::cout << "The dealer had: " << dealer.get_value() << std::endl;
						}
						else if (player.get_value() > 21) {
							ended = true;
							std::cout << "You bust with: " << player.get_value() << std::endl;
							std::cout << "The dealer had: " << dealer.get_value() << std::endl;
						}
						if (!ended) {
							if (!continue_game()) {
								ended = true;
							}
						}
					} while (!ended);
					if (player.get_value() < 21) {
						do {
							if (dealer.get_value() < 17) {
								Card card = game_deck.draw_card();
								dealer.add_card(card);
								dealer.update_val();
								cards_dealt++;
								std::cout << "The dealer drew a: " << card.get_name() << std::endl;
								if (dealer.get_value() == 21) {
									ended = true;
									std::cout << "The dealer won with 21 :(" << std::endl;
								}
								else if (dealer.get_value() > 21) {
									ended = true;
									std::cout << "The dealer bust with: " << dealer.get_value() << std::endl;
									std::cout << "You won with: " << player.get_value() << std::endl;
								}
								else if (dealer.get_value() > 17) {
									ended = true;
									if (dealer.get_value() > player.get_value()) {
										std::cout << "The dealer has won with: " << dealer.get_value() << std::endl;
									}
									else if (dealer.get_value() == player.get_value()) {
										std::cout << "You've tied the dealer with: " << dealer.get_value() << ". Ties go to the house." << std::endl;
									}
									else if (dealer.get_value() < player.get_value()) {
										std::cout << "You've won with: " << player.get_value() << "! The dealer had: " << dealer.get_value() << std::endl;
									}
								}
							}
						} while (!ended);
					}
				}
				else {
					if (player.get_value() == 21) {
						std::cout << "You've won with 21! The dealer had: " << dealer.get_value() << "." << std::endl;
						ended = true;
					}
					else if (dealer.get_value() < 17) {
						do {
							Card card = game_deck.draw_card();
							dealer.add_card(card);
							std::cout << "The dealer drew a: " << card.get_name() << "." << std::endl;
							cards_dealt++;
							dealer.update_val();
							if (dealer.get_value() == 21) {
								ended = true;
								std::cout << "The dealer won with 21 :(" << std::endl;
							}
							else if (dealer.get_value() > 21) {
								ended = true;
								std::cout << "The dealer bust with: " << dealer.get_value() << std::endl;
								std::cout << "You won with: " << player.get_value() << std::endl;
							}
							else if (dealer.get_value() > 17) {
								ended = true;
								if (dealer.get_value() > player.get_value()) {
									std::cout << "The dealer has won with: " << dealer.get_value() << std::endl;
								}
								else if (dealer.get_value() == player.get_value()) {
									std::cout << "You've tied the dealer with: " << dealer.get_value() << ". Ties go to the house." << std::endl;
								}
								else if (dealer.get_value() < player.get_value()) {
									std::cout << "You've won with: " << player.get_value() << "! The dealer had: " << dealer.get_value() << std::endl;
								}
							}
						} while (!ended);
					}
					else {
						if (dealer.get_value() > player.get_value()) {
							std::cout << "The dealer has won with: " << dealer.get_value() << std::endl;
						}
						else if (dealer.get_value() == player.get_value()) {
							std::cout << "You've tied the dealer. Ties go to the house." << std::endl;
						}
						else {
							std::cout << "You've won with: " << player.get_value() << ". The dealer had: " << dealer.get_value() << std::endl;
						}
					}
				}
			}
			char inp;
			std::cout << "Would you like to play another hand? (y/n)" << std::endl;
			std::cin >> inp;
			return (inp == 'y');
		}
	private:
		bool ended = false;
		int cards_dealt = 0;
		Deck game_deck;
		Hand player;
		Hand dealer;
	};
}













/*
Game() {
			std::cout << "Press any key to begin." << std::endl;
			std::cin.get();
			char input;
			srand(time(NULL));
			Deck game_deck;
			Hand dealer(0);
			Hand player(1);
			for (int i = 0; i < 2; i++) {
				Card card = game_deck.draw_card();
				dealer.add_card(card);
			}
			for (int i = 0; i < 2; i++) {
				Card card = game_deck.draw_card();
				player.add_card(card);
			}
			dealer.update_val();
			player.update_val();
			dealer.print_hand();
			player.print_hand();
			std::cout << "Would you like to hit or stay? (h/s)" << std::endl;
			std::cin >> input;
			if (input == 'h') {
				Card card = game_deck.draw_card();
				player.add_card(card);
				std::cout << "You were dealt a " << card.get_name() << std::endl;
				player.update_val();
				int cur = player.get_value();
				if (cur > 21) {
					std::cout << "You bust with a " << cur << std::endl;
				}
				else {
					std::cout << "You now have: " << cur << std::endl;
				}
				std::cout << "Would you like to hit or stay? (h/s)" << std::endl;
				std::cin >> input;
				if (input = 'h') {
					card = game_deck.draw_card();
					player.add_card(card);
					std::cout << "You were dealt a " << card.get_name() << std::endl;
					player.update_val();
					cur = player.get_value();
					if (cur > 21) {
						std::cout << "You bust with a " << cur << std::endl;
					}
					else {
						std::cout << "You now have: " << cur << std::endl;
					}
				}
			}
		}
*/