/**
 * Assignment #4 COMP345, FALL 2019
 * Project: Risk Domination Game
 * Authors: Giselle Martel (26352936), Wayne Tam (21308688), Jeffrey Li (40017627), Rania Az (40041630)
 */

#include "../GameEngine/GameEngine.h"
#include "Player.h"


// Player class implementation ----------------------------------------------------------------------------
Player::Player(string player_name, int player_id, Map *game_map, GameEngine* game_engine)
: player_id_(player_id), game_map_(game_map), game_engine_(game_engine)
{
    player_name_ = new string(std::move(player_name));
    is_human_ = false;
    is_random_ = false;
    is_cheater_ = false;
    is_benevolant_ = false;
    is_aggressive_ = false;
    countries_ = new vector<Country*>;
    risk_cards_ = nullptr;
    dice_roll_ = nullptr;
    player_strategy_ = nullptr;
    reinforce_phase_ = nullptr;
    attack_phase_ = nullptr;
    fortify_phase_ = nullptr;
}

Player::Player(const Player &player) : player_id_(player.player_id_){
    *player_name_ = *player.player_name_;
    is_human_ = player.is_human_;
    is_random_ = player.is_random_;
    is_cheater_ = player.is_cheater_;
    is_benevolant_ = player.is_benevolant_;
    is_aggressive_ = player.is_aggressive_;

    *countries_ = *player.countries_;
    for(size_t i = 0; i < player.countries_->size(); ++i) {
        (*countries_)[i] = (*player.countries_)[i];
        delete (*player.countries_)[i];
        (*player.countries_)[i] = nullptr;
    }

    *risk_cards_ = *player.risk_cards_;
    *dice_roll_ = *player.dice_roll_;
    *game_map_ = *player.game_map_;
    *player_strategy_ = *player.player_strategy_;
    *reinforce_phase_ = *player.reinforce_phase_;
    *attack_phase_ = *player.attack_phase_;
    *fortify_phase_ = *player.fortify_phase_;
    *game_engine_ = *player.game_engine_;


    delete player.player_name_;
    delete player.risk_cards_;
    delete player.dice_roll_;
    delete player.game_map_;
    delete player.player_strategy_;
    delete player.reinforce_phase_;
    delete player.attack_phase_;
    delete player.fortify_phase_;
    delete player.game_engine_;
    delete player.countries_;
}

Player::~Player() {
    delete player_name_;
    delete risk_cards_;
    delete dice_roll_;
    delete player_strategy_;
    delete attack_phase_;
    delete reinforce_phase_;
    delete fortify_phase_;

    player_name_ = nullptr;
    risk_cards_ = nullptr;
    dice_roll_ = nullptr;
    player_strategy_ = nullptr;
    attack_phase_ = nullptr;
    reinforce_phase_ = nullptr;
    fortify_phase_ = nullptr;
}

Player& Player::operator=(const Player &player) {

    if(this != & player) {
        *player_name_ = *player.player_name_;
        is_human_ = player.is_human_;
        is_random_ = player.is_random_;
        is_cheater_ = player.is_cheater_;
        is_benevolant_ = player.is_benevolant_;
        is_aggressive_ = player.is_aggressive_;


        *countries_ = *player.countries_;
        for(size_t i = 0; i < player.countries_->size(); ++i) {
            (*countries_)[i] = (*player.countries_)[i];
            delete (*player.countries_)[i];
            (*player.countries_)[i] = nullptr;
        }

        *risk_cards_ = *player.risk_cards_;
        *dice_roll_ = *player.dice_roll_;
        *game_map_ = *player.game_map_;
        *player_strategy_ = *player.player_strategy_;
        *reinforce_phase_ = *player.reinforce_phase_;
        *attack_phase_ = *player.attack_phase_;
        *fortify_phase_ = *player.fortify_phase_;
        *game_engine_ = *player.game_engine_;


        delete player.player_name_;
        delete player.risk_cards_;
        delete player.dice_roll_;
        delete player.game_map_;
        delete player.player_strategy_;
        delete player.reinforce_phase_;
        delete player.attack_phase_;
        delete player.fortify_phase_;
        delete player.game_engine_;
        delete player.countries_;
    }


    return *this;
}

bool Player::operator==(const Player& player) {
    bool is_equal = true;
    is_equal &= player_id_ == player.player_id_
            && is_human_ == player.is_human_
            && is_random_ == player.is_random_
            && is_cheater_ == player.is_cheater_
            && is_aggressive_ == player.is_aggressive_
            && is_benevolant_ == player.is_benevolant_;

    return is_equal;
}

int Player::Find(Country* country) const {
    for(int i = 0; i < countries_->size(); ++i) {
        if(*(*countries_)[i] == *country) {
            return i;
        }
    }
    return -1;
}

bool Player::DoesPlayerOwnCountry(int id) const {
    for(const Country* country : *countries_) {
        int current_id = (country->GetCountryID());
        if (id == current_id) {
            return true;
        }
    }
    return false;
}

int Player::GetPlayerID() const {
    return player_id_;
}

vector<Country*>* Player::GetPlayersCountries() const {
    return countries_;
}

bool Player::IsHuman() const {
    return is_human_;
}

bool Player::IsRandom() const {
    return is_random_;
}

bool Player::IsCheater() const {
    return is_cheater_;
}

bool Player::IsAggressive() const {
    return is_aggressive_;
}

bool Player::IsBenevolant() const {
    return is_benevolant_;
}

Country* Player::GetCountryById(int id) const {
    for(Country* country : *countries_) {
        int current_id = (country->GetCountryID());
        if (id == current_id) {
            return country;
        }
    }
    return nullptr;
}

Country* Player::GetCountryInVectorById(vector<Country*>* countries, int country_id) {
    for(Country* country : *countries) {
        if(country->GetCountryID() == country_id) {
            return country;
        }
    }
    return nullptr;
}

Country* Player::PromptPlayerToSelectCountry() const {
    int country_id;
    while(!(cin >> country_id) || country_id < 1 || !DoesPlayerOwnCountry(country_id) ) {
        cin.clear();
        cin.ignore(132, '\n');
        cout << "Invalid entry entered! Please try again: ";
    }
    return GetCountryById(country_id);
}

GameEngine* Player::GetGameEngine() const {
    return game_engine_;
}

ReinforcePhase* Player::GetReinforcePhase() const {
    return reinforce_phase_;
}

AttackPhase* Player::GetAttackPhase() const {
    return attack_phase_;
}

FortifyPhase* Player::GetFortifyPhase() const {
    return fortify_phase_;
}

Dice* Player::GetPlayerDice() const {
    return dice_roll_;
}

Hand* Player::GetPlayersCards() const {
    return risk_cards_;
}

Map* Player::GetGameMap() const {
    return game_map_;
}

string* Player::GetPlayerName() const {
    return player_name_;
}

void Player::SetPlayerName(string* player_name) {
    player_name_ = player_name;
}

void Player::SetPlayerDice(Dice *dice) {
    dice_roll_ = dice;
}

void Player::SetPlayerHand(Hand* hand) {
    risk_cards_ = hand;
}

void Player::SetPlayerStrategy(ConcreteStrategies *player_strategy) {
    player_strategy_ = player_strategy;
}

void Player::SetGameMap(Map* map) {
    game_map_ = map;
}

void Player::SetAsHuman() {
    is_human_ = true;
}

void Player::SetAsRandom() {
    is_random_ = true;
}

void Player::SetAsCheater() {
    is_cheater_ = true;
}

void Player::SetAsAggressive() {
    is_aggressive_ = true;
}

void Player::SetAsBenevolent() {
    is_benevolant_ = true;
}
void Player::AddCountryToCollection(Country *country) {
    if(!countries_) {
        countries_ = new vector<Country*>;
    }
    countries_->push_back(country);
    country->SetCountryOwner(player_id_);
}

void Player::RemoveCountryFromCollection(Country* country) {
    if(!countries_ || countries_->empty() || Find(country) == -1){
        cout << *country->GetCountryName() << " not found among " << *player_name_ << "'s countries!\n";
        return;
    }
    countries_->erase (countries_->begin() + Find(country));
}

void Player::AddCardToCollection(Cards* card) {
    if(!risk_cards_) {
        risk_cards_ = new Hand;
    }
    risk_cards_->AddCardToHand(card);
}

void Player::DisplayPlayerStats() const {
    cout << "\n===================================================\n" << *player_name_ << endl;
    cout << "Number of countries owned: " << countries_->size();
    cout << endl;

    cout << "Number cards in hand: " << risk_cards_->GetNumberOfCardsInHand();
    risk_cards_->DisplayHand();
    cout << endl;

    cout << "Total dice rolls made: " << dice_roll_->GetTotalRolls();
    cout  << "\n===================================================\n";
}

void Player::DisplayCountries() const {
    cout << endl;
    cout << endl << setw(25)  << left << "Country ID" << setw(25)  << "Name" << setw(25) << "Number of Armies" << setw(10) << right << "Neighbouring Countries" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    for (Country *country : *countries_) {
        cout << setw(25)  << left << country->GetCountryID() << setw(25) <<  *country->GetCountryName() << setw(25) << country->GetNumberOfArmies() << setw(10) << right << game_map_->GenerateListOfNeighboringCountries(country) << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << endl;
    }
    cout << endl;
}

string Player::AttackerConquersDefeatedCountry() {
    Country* attacking_country = attack_phase_->GetAttackingCountry();
    Country* defending_country = attack_phase_->GetDefendingCountry();
    Player* defender = game_engine_->GetPlayerByID(defending_country->GetOwnerID());

    string msg;
    if(!is_cheater_) {
        msg = "Defending country " + *defending_country->GetCountryName() + " has no armies and has been defeated!\n";
        Notify(this, GamePhase::Attack, msg, false, false);
    } else {
        msg = "Defending country " + *defending_country->GetCountryName() + " is defeated automatically by Cheater player!\n";
        Notify(this, GamePhase::Attack, msg, false, false);
    }

    msg = "";


    string display_info = defending_country->GetDisplayInfo();
    defender->RemoveCountryFromCollection(defending_country);
    //defender has lost. Its country will now be transferred to the attacker
    AddCountryToCollection(defending_country);


    player_strategy_->MoveArmiesAfterAttack(this, attacking_country, defending_country);

    defending_country->SetCountryOwner(player_id_);
    attack_phase_->RemoveDefeatedCountryFromOpponents(defending_country);


    //GameStatisticObserver: notify that a player has lost a country
    if(!defender->GetPlayersCountries()->empty()) {
        Notify(*player_name_+" has conquered " + *defender->GetPlayerName() + "'s country " + *defending_country->GetCountryName(), *game_engine_->GetPlayers());
    }

    // Draw card once country has been conquered
    Deck* game_deck_ = game_engine_->GetCardsDeck();
    if(!game_deck_) {
        return "";
    }

    Cards* card = game_deck_->Draw();
    if(card) {
        risk_cards_->AddCardToHand(card);
    }

    //check to see if player has no more countries
    if(defender->GetPlayersCountries()->empty()) {
        RemoveDefeatedPlayerFromGame(defending_country, defender);
    }

    return display_info;
}

void Player::RemoveDefeatedPlayerFromGame(Country* defeated_country, Player* defender) {
    string defeated_name = *defender->GetPlayerName();
    string msg;
    //Player has been defeated they have no more armies
    msg = *player_name_+" has conquered " + defeated_name + "'s country " + *defeated_country->GetCountryName() + ". " + defeated_name + " has been eliminated from the game!";
    game_engine_->RemovePlayer(defender);

    //GameStatisticObserver: notify that a player has been defeated
    Notify(msg, *game_engine_->GetPlayers());

    //PhaseObserver
    Notify(this, GamePhase::Attack, msg, false, false);
}

void Player::Reinforce() {
    if(!player_strategy_) {
        Notify(this, GamePhase::Reinforce, "Player strategy undefined! Cannot Reinforce!\n", false, true);
        return;
    }


    string msg =  "Beginning Reinforce phase for " + *player_name_ + "\n\n";

    //PhaseObserver
    Notify(this, GamePhase::Reinforce, msg, true, false);

    reinforce_phase_ = new ReinforcePhase();

    if(reinforce_phase_->TotalReinforceArmy(this, game_engine_->GetLoadedMap()->GetParsedMap()->GetContinents()) < 1) {
        msg = *player_name_;
        msg.append(" currently has to armies to reinforce a country with. Please try again next round\n\n");
        Notify(this, GamePhase::Reinforce, msg, false, true);
        return;
    }

    player_strategy_->ReinforceStrategy(this);

    for(int i = 0; i < reinforce_phase_->GetReinforceValues()->size(); ++i) {
        Country* current_country = GetCountryById((*reinforce_phase_->GetCountriesToReinforce())[i]);

        if(!current_country) {
            continue;
        }

        int num_armies_to_add = (*reinforce_phase_->GetReinforceValues())[i];
        int current_num_armies = current_country->GetNumberOfArmies();

        if(num_armies_to_add > 0) {
            msg = *player_name_ + " reinforcing " + *current_country->GetCountryName() + " who currently has " + to_string(current_country->GetNumberOfArmies()) + " armies with " + to_string(num_armies_to_add) + " armies";
            Notify(this, GamePhase::Reinforce, msg, false,false);
        }

        current_country->SetNumberOfArmies(current_num_armies + num_armies_to_add);

        if(num_armies_to_add > 0) {
            msg = "Number of armies currently in " + *current_country->GetCountryName() + ": " + to_string(current_country->GetNumberOfArmies()) + "\n\n";
            Notify(this, GamePhase::Reinforce, msg, false,false);
        }

    }
    msg = "Reinforce Phase is now over\n";

    //PhaseObserver
    Notify(this, GamePhase::Reinforce, msg, false, true);
}

void Player::Attack() {

    if(!player_strategy_) {
        Notify(this, GamePhase::Attack, "Player strategy undefined! Cannot Attack!\n", false, true);
        return;
    }

    attack_phase_ = new AttackPhase(this);

    int countries_conquered = 0;

    string msg =  "Beginning Attack phase for " + *player_name_ + "\n";

    //PhaseObserver
    Notify(this, GamePhase::Attack, msg, true, false);

    player_strategy_->SetNumberOfTimesToAttack(this);

    while (player_strategy_->PromptPlayerToAttack(this) && countries_->size() < game_map_->GetNumCountries()) {
        attack_phase_->SetDefendingCountry(nullptr);
        attack_phase_->SetAttackingCountry(nullptr);
        attack_phase_->SetDefender(nullptr);

        //Check first if the player owns at least one country with enough armies to execute an attack
        if(!attack_phase_->PlayerHasCountryWithEnoughArmiesToAttack()) {
            msg = "Player currently has no country with enough armies to attack!\n";
            Notify(this, GamePhase::Attack, msg, false, false);
            break;
        }

        //check if there is at least one country that can be attacked
        if(!attack_phase_->DoesOpposingCountryExist()) {
            msg = "No opposing country exists that can be attacked!\n";
            Notify(this, GamePhase::Attack, msg, false, false);
            break;
        }

        //Prompt player to select an attacker
        if(!player_strategy_->SelectCountryToAttackFrom(this)) {
            msg = "Failed to Select valid country to attack from!\n";
            Notify(this, GamePhase::Attack, msg, false, false);
            break;
        }

        if(attack_phase_->GetAttackingCountry()->GetOwnerID() != player_id_) {
            continue;
        }

        msg =  *player_name_ + " has selected " + *attack_phase_->GetAttackingCountry()->GetCountryName() + " to attack with\n";
        Notify(this, GamePhase::Attack, msg, false, false);


        //Get the neighbours to the attacking country owned by other players
        attack_phase_->FindOpponentNeighboursToAttackingCountry();

        // if there are no opponent neighbours, the attacker cannot execute an attack
        bool can_attack = attack_phase_->GetOpponentNeighbours() && !attack_phase_->GetOpponentNeighbours()->empty();

        if(can_attack) {
            //prompt player to select a defending country
            can_attack &= player_strategy_->SelectCountryToAttack(this);
            can_attack &= attack_phase_->GetDefendingCountry()->GetOwnerID() != player_id_;

            if(!can_attack){
                msg = *player_name_ + " something went wrong when selecting a defending country. Cannot Attack!\n";
                Notify(this, GamePhase::Attack, msg, false, false);
                continue;
            }

            msg = *player_name_ + " has selected " + *attack_phase_->GetDefendingCountry()->GetCountryName() + " to attack\n";
            Notify(this, GamePhase::Attack, msg, false, false);

            Country* attacking_country = attack_phase_->GetAttackingCountry();
            Country* defending_country = attack_phase_->GetDefendingCountry();
            Player* defender = game_engine_->GetPlayerByID(defending_country->GetOwnerID());


            string defender_name = *defender->GetPlayerName();
            string defending_country_info = "";
            // if the defending country has no armies or if the current player is in cheat mode, they are defeated automatically
            if(defending_country->GetNumberOfArmies() == 0 || is_cheater_) {
                //function will return true when player is removed from game
                ++countries_conquered;
                 defending_country_info = AttackerConquersDefeatedCountry();
            } else { //otherwise the players will roll their dice and the attack will occur
                int attacker_num_dice = 0;
                int defender_num_dice = 0;
                //determine maximum number of rolls for each player based on rules of Risk
                int max_num_dice_attacker = (attacking_country->GetNumberOfArmies() - 1) < 3 ? attacking_country ->GetNumberOfArmies() : 3;

                const int MAX_NUM_OF_DICE_ATTACKER = (max_num_dice_attacker == 0) ? 1 : max_num_dice_attacker;
                const int MAX_NUM_OF_DICE_DEFENDER = (defending_country->GetNumberOfArmies()) < 2 ? defending_country ->GetNumberOfArmies() : 2;

                // prompt the attacker to select the number of dice
                player_strategy_->AttackerSelectNumberOfDice(this, MAX_NUM_OF_DICE_ATTACKER, attacker_num_dice);


                msg = "\nIt is " + *defender->GetPlayerName() + "'s turn to enter the number of dice they wish to roll (can roll max " + to_string(MAX_NUM_OF_DICE_DEFENDER) + ") dice: ";
                Notify(this, GamePhase::Attack, msg, false, false);
               // Notify(msg, *game_engine_->GetPlayers());

                //if the player is human then they select their own number of dice otherwise it is randomly generated for computer players
                if(defender->IsHuman()) {
                    while( !(cin >> defender_num_dice) || defender_num_dice < 1 || defender_num_dice > MAX_NUM_OF_DICE_DEFENDER) {
                        cout << "You have entered an invalid number of dice to roll. Please try again: ";
                        cin.clear();
                        cin.ignore(132, '\n');
                    }
                } else {
                    defender_num_dice = Utility::GenerateRandomNumInRange(1, MAX_NUM_OF_DICE_DEFENDER);
                }

                msg = *player_name_ + " has chosen to roll " + to_string(attacker_num_dice) + " dice\n";
                msg.append( *defender->GetPlayerName() + " has chosen to roll " + to_string(defender_num_dice) + " dice\n");
                Notify(this, GamePhase::Attack, msg, false, false);

                msg = "Carrying out attacks....\n";
                //Roll the dice!
                vector<int> attacker_dice_rolls = dice_roll_->Roll(attacker_num_dice);
                msg = "Attacker dice rolled:\n";

                for(int i : attacker_dice_rolls) {
                    msg.append(to_string(i) + "\n");
                }

                vector<int> defender_dice_rolls = defender->GetPlayerDice()->Roll(defender_num_dice);
                msg.append("Defender dice rolled:\n");
                for(int i : defender_dice_rolls) {
                    msg.append(to_string(i) + "\n");
                }

                msg.append("\n");

                Notify(this, GamePhase::Attack, msg, false, false);

                //sort the rolls from highest value to lowest value
                sort(attacker_dice_rolls.begin(), attacker_dice_rolls.end());
                reverse(attacker_dice_rolls.begin(), attacker_dice_rolls.end());
                sort(defender_dice_rolls.begin(), defender_dice_rolls.end());
                reverse(defender_dice_rolls.begin(), defender_dice_rolls.end());

                // Get the number of dice pairs we are evaluating
                int num_of_iterations = (attacker_dice_rolls.size() == defender_dice_rolls.size() || attacker_dice_rolls.size() < defender_dice_rolls.size()) ? attacker_dice_rolls.size() : defender_dice_rolls.size();
                msg = "";
                for(int i = 0; i < num_of_iterations; ++i) {
                    //attacker lose an army if the value on the dice is less than or equal to value on the dice of the defender
                    if(attacker_dice_rolls[i] <= defender_dice_rolls[i]) {
                        msg.append("Attacker has lost this roll. Loosing an army\n");
                        attacking_country->RemoveArmiesFromCountry(1);
                        if(attacking_country->GetNumberOfArmies() == 0) {
                            msg.append("Attacking country " + *attacking_country->GetCountryName() + " has run out of armies.\n");
                            Notify(this, GamePhase::Attack, msg, false, false);
                            break;
                        }
                        //defender lose an army if attacker's dice has a greater value
                    } else if (attacker_dice_rolls[i] > defender_dice_rolls[i]) {
                        msg.append("Defender has lost this roll. Loosing an army\n");
                        defending_country->RemoveArmiesFromCountry(1);

                        if(defending_country->GetNumberOfArmies() == 0) {
                            //function will return true when player is removed from game
                            ++countries_conquered;
                            defending_country_info = AttackerConquersDefeatedCountry();
                            break;
                        }
                    }
                }

                Notify(this, GamePhase::Attack, msg, false, false);
            }

            if(attacking_country) {
                msg = "Result:\n\n";
                msg.append(*player_name_ + "\n");
                msg.append(attacking_country->GetDisplayInfo());
                msg.append("\n" +  defender_name + "\n");
                msg.append(defending_country_info);

                Notify(this, GamePhase::Attack, msg, false, false);
            }
        } else {
            break;
        }

        Notify(this, GamePhase::Attack, *player_name_ + " is attacking again\n", false, true);
    }

    // Draw cards from deck after turn
    for(int i = 0; i < countries_conquered; ++i) {
        Deck* game_deck_ = game_engine_->GetCardsDeck();
        if(!game_deck_) {
            return;
        }
        if(game_deck_->GetNumberOfCardsInDeck() != 0 )
            risk_cards_->AddCardToHand(game_deck_->Draw());
    }

    //PhaseObserver
    msg =  *player_name_ + "'s Attack phase is over, going to next phase";
    Notify(this, GamePhase::Attack, msg, false, true);
}

void Player::Fortify() {
    if(!player_strategy_) {
        Notify(this, GamePhase::Fortify, "Player strategy undefined! Cannot Fortify!", false, true);
        return;
    }

    string msg =  "Beginning Fortify phase for " + *player_name_ + "\n\n";

    //PhaseObserver
    Notify(this, GamePhase::Fortify, msg, true, false);

    fortify_phase_ = new FortifyPhase(this);

    if(player_strategy_->PromptPlayerToFortify(this))  {


        if(!is_cheater_) {
            int count = 0;
            for (Country *country : *countries_) {
                if (country->GetNumberOfArmies() < 1) {
                    ++count;
                }
            }

            if (count == countries_->size()) {
                msg = "No countries with enough armies to assign to another country!\nEnding Fortify phase\n" +
                      *player_name_ + "'s turn end.\n";
                Notify(this, GamePhase::Fortify, msg, false, true);
                return;
            }


            player_strategy_->SelectSourceCountry(this);

            if (!fortify_phase_->GetSourceCountry()) {
                msg = "Ending Fortify phase\n" + *player_name_ + "'s turn end.\n";
                Notify(this, GamePhase::Fortify, msg, false, true);
                return;
            }

            Country* source_country = fortify_phase_->GetSourceCountry();


            cout << "Please select which target country you wish to fortify:\n";

            vector<Country *> *neighbours = game_map_->GetNeighbouringCountries(source_country);

            if (!neighbours || neighbours->empty()) {
                msg = *source_country->GetCountryName() + " has no neighbours!\nEnding Fortify phase\n" +
                      *player_name_ + "'s turn end.\n";
                Notify(this, GamePhase::Fortify, msg, false, true);
                return;
            }

            //find all the neighbours of the source country that belong to the current player
            for (Country *country : *neighbours) {
                if (DoesPlayerOwnCountry(country->GetCountryID()) && player_id_ == country->GetOwnerID()) {
                    fortify_phase_->GetNeighboursToFortify()->push_back(country);
                }
            }

            //only opponents are neighbours, skip phase
            if (fortify_phase_->GetNeighboursToFortify()->empty()) {
                msg = *source_country->GetCountryName() +
                      " has no neighbours to assign armies to!\nEnding Fortify phase\n" + *player_name_ +
                      "'s turn end.\n";
                Notify(this, GamePhase::Fortify, msg, false, true);
                return;
            }

            //player selects country to fortify from list of neighbours
            cout << "\nHere are the neighbouring countries to " << *source_country->GetCountryName() << endl;
            cout << endl << setw(25) << left << "Country ID" << setw(25) << "Name" << setw(25) << right
                 << "Number of Armies" << endl;
            for (const Country* country : *fortify_phase_->GetNeighboursToFortify()) {
                cout << setw(25) << left << country->GetCountryID() << setw(25) << *country->GetCountryName()
                     << setw(25) << right << country->GetNumberOfArmies() << endl;
                cout << endl;
            }


            player_strategy_->SelectTargetCountry(this);

            if (!fortify_phase_->GetTargetCountry()) {
                msg = "No target country selected. Ending Fortify phase\n" + *player_name_ + "'s turn end.\n";
                Notify(this, GamePhase::Fortify, msg, false, true);
                return;
            }

            Country* target_country = fortify_phase_->GetTargetCountry();

            msg = "There are " + to_string(source_country->GetNumberOfArmies()) + " armies in " +
                  *source_country->GetCountryName() + ".\n";
            msg.append("There are " + to_string(target_country->GetNumberOfArmies()) + " armies in " +
                       *target_country->GetCountryName() + ".\n");
            Notify(this, GamePhase::Fortify, msg, false, false);


            int num_of_armies;

            player_strategy_->FortifyStrategy(this, num_of_armies);


            msg = "Fortifying " + *target_country->GetCountryName() + " with " + to_string(num_of_armies) +
                  " armies.\n";
            Notify(this, GamePhase::Fortify, msg, false, false);

            source_country->RemoveArmiesFromCountry(num_of_armies);
            target_country->SetNumberOfArmies(target_country->GetNumberOfArmies() + num_of_armies);


            msg = "\nHere is the result after fortification: \nThere are " +
                  to_string(source_country->GetNumberOfArmies()) + " armies in " + *source_country->GetCountryName() +
                  ".\n";
            msg.append("There are " + to_string(target_country->GetNumberOfArmies()) + " armies in " +
                       *target_country->GetCountryName() + ".\n");
            Notify(this, GamePhase::Fortify, msg, false, false);

        } else {
            //cheater does not select source country, but instead fortifies a country if it has at least 1 enemy neighbour
            int num_of_armies;
            if(!player_strategy_->SelectTargetCountry(this)) {
                msg = "No target country selected. Ending Fortify phase\n" + *player_name_ + "'s turn end.\n";
                Notify(this, GamePhase::Fortify, msg, false, true);
                return;
            }

            msg = *player_name_ +  " has chosen to fortify " + *fortify_phase_->GetTargetCountry()->GetCountryName() + " who currently has " + to_string(fortify_phase_->GetTargetCountry()->GetNumberOfArmies()) +
                  " armies.\n";
            Notify(this, GamePhase::Fortify, msg, false, false);

            player_strategy_->FortifyStrategy(this, num_of_armies);

            msg = "Doubling number of armies in " + *fortify_phase_->GetTargetCountry()->GetCountryName() + " by Fortifying with " + to_string(num_of_armies) +
                  " armies.\n";

            // the cheater will double the armies on the target country
            fortify_phase_->GetTargetCountry()->SetNumberOfArmies(fortify_phase_->GetTargetCountry()->GetNumberOfArmies() + num_of_armies);

            msg.append("There are " + to_string(fortify_phase_->GetTargetCountry()->GetNumberOfArmies()) + " armies in " +
                       *fortify_phase_->GetTargetCountry()->GetCountryName() + ".\n");
            Notify(this, GamePhase::Fortify, msg, false, false);
        }
    }

    msg = "Ending Fortify phase\n" + *player_name_ + "'s turn end.\n";

    //PhaseObserver
    Notify(this, GamePhase::Fortify, msg, false, true);
}

//will be used to implicitly notify the game engine of phase changes
void Player::Notify(Player* current_player, int current_phase, const string& current_action_description, bool phase_start, bool phase_over) {
    game_engine_->Notify(current_player, current_phase, current_action_description, phase_start, phase_over);
}

//notify of statistic subject for GameStatisticObserver
void Player::Notify(string msg, const vector<Player*>& players){
    game_engine_->Notify(msg, players);
}