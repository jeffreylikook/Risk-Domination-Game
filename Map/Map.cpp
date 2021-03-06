
#include "Map.h"

using namespace std;

//START OF MAP CLASS FUNCTIONS ---------------------------------------------------------------------------------

Map::Map(const string& name) {
    map_name_ = new string(name);
    continents_ = new vector<Continent*>;
    countries_ = new vector<Country*>;
    num_countries_ = 0;
    num_continents_ = 0;
    adjacency_matrix_ = nullptr;
}

Map::Map(const Map &map) {
    *map_name_ = *map.map_name_;
    num_countries_ = map.num_countries_;
    num_continents_ = map.num_continents_;

    continents_ = new vector<Continent*>;
    countries_ = new vector<Country*>;

    if(continents_ && map.continents_) {
        *continents_ = *map.continents_;
        for(int i = 0; i < map.continents_->size(); ++i) {
            (*continents_)[i] = (*map.continents_)[i];
            delete (*map.continents_)[i];
            (*map.continents_)[i] = nullptr;
        }
        delete map.continents_;
    }


    if(countries_ && map.countries_) {
        *countries_ = *map.countries_;
        for(int i = 0; i < map.countries_->size(); ++i) {
            (*countries_)[i] = (*map.countries_)[i];
        }

        adjacency_matrix_ = map.adjacency_matrix_;
        for(size_t i = 0; i < map.num_countries_; ++i) {
            adjacency_matrix_[i] = map.adjacency_matrix_[i];

            for(size_t j = 0; j < map.num_countries_; ++j) {
                adjacency_matrix_[i][j] = map.adjacency_matrix_[i][j];
            }
        }
        delete map.countries_;
    }

    delete map.map_name_;
}


Map::~Map() {
    //delete all the continent objects
    for (Continent* continent : *continents_) {
        delete continent;
        continent = nullptr;
    }

    //delete all the country objects
    for (Country* country : *countries_) {
        delete country;
        country = nullptr;
    }

    //delete rows of adjacency matrix
    for (int i = 0; i < num_countries_; ++i) {
        delete[] adjacency_matrix_[i];
    }

    delete[] adjacency_matrix_;
    delete continents_;
    delete countries_;
    countries_ = nullptr;
    continents_ = nullptr;
}


Map& Map::operator=(const Map &map) {
    if(this != &map) {
        *map_name_ = *map.map_name_;
        num_countries_ = map.num_countries_;
        num_continents_ = map.num_continents_;

        continents_ = new vector<Continent*>;
        countries_ = new vector<Country*>;

        if(continents_ && map.continents_) {
            *continents_ = *map.continents_;
            for(int i = 0; i < map.continents_->size(); ++i) {
                (*continents_)[i] = (*map.continents_)[i];
                delete (*map.continents_)[i];
                (*map.continents_)[i] = nullptr;
            }
            delete map.continents_;
        }


        if(countries_ && map.countries_) {
            *countries_ = *map.countries_;
            for(int i = 0; i < map.countries_->size(); ++i) {
                (*countries_)[i] = (*map.countries_)[i];
                delete  (*map.countries_)[i];
                (*map.countries_)[i] = nullptr;
            }

            adjacency_matrix_ = map.adjacency_matrix_;
            for(size_t i = 0; i < map.num_countries_; ++i) {
                adjacency_matrix_[i] = map.adjacency_matrix_[i];

                for(size_t j = 0; j < map.num_countries_; ++j) {
                    adjacency_matrix_[i][j] = map.adjacency_matrix_[i][j];
                }
            }
            delete map.countries_;
        }

        delete map.map_name_;
    }

    return *this;
}

int Map::GetNumCountries() const {
    return num_countries_;
}

int Map::GetNumContinents() const {
    return num_continents_;
}

string* Map::GetMapName() const {
    return map_name_;
}

Country* Map::GetCountryById(int id) const {
    if(id < 1 || id > countries_->size()) {
        cout << "Invalid Id given. Countries start from 1" << endl;
        return nullptr;
    }

    for(Country* country : *countries_) {
        if(country->GetCountryID() == id)  {
            return country;
        }
    }
    return nullptr;
}

Country* Map::GetCountryByName(string name) const{

    if(name.length()==0) {
        cout << "Invalid name given. " << endl;
        return nullptr;
    }

    for(Country* country : *countries_) {
        if(*country->GetCountryName()==name)  {
            return country;
        }
    }
    return nullptr;
}

Continent* Map::GetContinentById(int id) const {
    if(id < 1 || id > continents_->size() + 1) {
        cout << "Invalid Id given. Continents start from 1" << endl;
        return nullptr;
    }
    for(Continent* continent : *continents_) {
        if(continent->GetContinentID() == id)  {
            return continent;
        }
    }
    return nullptr;
}

Continent* Map::GetContinentByName(string name) const {
    if(name.length()==0 ) {
        cout << "Invalid Name given. " << endl;
        return nullptr;
    }
    for(Continent* continent : *continents_) {
        if(*(continent->GetContinentName()) == name)  {
            return continent;
        }
    }
    return nullptr;
}

vector<Country*>* Map::GetCountries() const {
    return countries_;
}

vector<Continent*>* Map::GetContinents() const {
    return continents_;
}

void Map::SetTwoCountriesAsNeighbours(bool value, int country_index, int border_index) {

    if(country_index >= num_countries_ || border_index >= num_countries_ || country_index < 0 || border_index < 0) {
        cout << "error, invalid index given" << endl;
        return;
    }
    adjacency_matrix_[country_index][border_index] = value;
    adjacency_matrix_[border_index][country_index] = value;
}

//Methods--------------------------------------------------------------------------------------------

void Map::AddCountryToMap(int country_num, const string& continent_name, int continent_index, int x_coordinate, int y_coordinate){

    auto* country_to_add = new Country(country_num, new string(continent_name), continent_index, x_coordinate, y_coordinate);
    for(Country* country : *countries_){
        if(IsCountryDuplicate(country_to_add, country)){
            return;
        }
    }

    countries_->push_back(country_to_add);

    ++num_countries_;

    Continent* continent = GetContinentById(continent_index);
    country_to_add->SetContinentID(continent->GetContinentID());
    continent->AddCountryToContinent(country_to_add);
}


void Map::AddContinentToMap(const string& continent_name, int army_value, int id){
    if(army_value == 0) {
        cout << "Invalid Continent Army Value" << endl;
        return;
    }
    Continent* continent_to_add = new Continent(new string(continent_name), army_value, id);

    //check for continent duplicate
    try {
        if (!continents_->empty()) {
            for (auto continent_to_check : *continents_) {
                if (!continent_to_check) {
                    continue;
                }

                if (IsContinentDuplicate(continent_to_add, continent_to_check)) {
                    continue;
                }
            }
        }
    } catch (const exception&){
        cout << "error" << endl;
    }
    continents_->push_back(continent_to_add);
    ++num_continents_;
}

void Map::CreateAdjacencyMatrix() {
    adjacency_matrix_ = new bool*[num_countries_];

    /**create an array for each country in the map, which will have true/false value for adjacency for each other country
    * example:
    *
    *       [
    *           Canada [ true, true, false ],
    *           US     [ true, true, true ],
    *           Mexico  [false, true, true]
    *       ]
    */

    for (size_t i = 0; i < num_countries_; ++i) {

        adjacency_matrix_[i] = new bool[num_countries_];

        for (int j = 0; j < num_countries_; ++j) {
            //a country is adjacent to itself
            adjacency_matrix_[i][j] = j == i;
        }
    }
}


void Map::DisplayContinents() const {
    for(Continent* continent : *continents_){
        continent->DisplayInfo();
    }
}

void Map::DisplayCountries() const {
    for(Country* country : *countries_){
        country->DisplayInfo();
    }
}

void Map::DisplayAdjacencyMatrix() const {
    cout << "\t\t\tNeighbours " << endl;
    cout << "\t\t\t";
    for(size_t i = 0; i < num_countries_; ++i) {
        cout << (i+1) << "\t";
    }
    cout << endl;

    for(size_t i = 0; i < num_countries_; ++i) {
        cout << "country " << (i + 1) << ":\t";

        for(size_t j = 0; j < num_countries_; ++j) {
            cout << adjacency_matrix_[i][j] << '\t';
        }

        cout << endl;
    }

    cout << endl;
}

void Map::DisplayGraphTraversal(Country* origin_country) const {
    static vector<bool> visited(countries_->size(), false);
    if(visited[origin_country->GetCountryID()-1] == false){
        visited[origin_country->GetCountryID()-1] = true;
        cout<< *origin_country->GetCountryName() <<endl;
        for(int i=0; i<countries_->size();i++){
            if(((i+1) != origin_country->GetCountryID()) && AreCountriesNeighbors(origin_country, countries_->at(i)))
                DisplayGraphTraversal(countries_->at(i));
        }
    }

}

bool Map::AreCountriesNeighbors(Country* country_a, Country* country_b) const {
    int index_a = country_a->GetCountryID() - 1;
    int index_b = country_b->GetCountryID() - 1;
    if(index_a < 0 || index_a > countries_->size() || index_b < 0 || index_a > countries_->size()) {
        return false;
    }
    if(adjacency_matrix_[index_a][index_b] && adjacency_matrix_[index_b][index_a]) {
        return true;
    }
    return false;
}

//static functions
bool Map::IsContinentDuplicate(Continent* continent_a, Continent* continent_b){
    return(continent_a->GetContinentID() == continent_b->GetContinentID());
}

bool Map::IsCountryDuplicate(Country* country_a, Country* country_b){
    return (country_a->GetCountryID() == country_b->GetCountryID());
}

vector<Country*>* Map::GetNeighbouringCountriesWithArmies(Country* country) const {
    int country_index = country->GetCountryID() - 1;
    vector<Country*>* neighbouring_countries = new vector<Country*>;
    vector<int> neighbouring_countries_indices;

    //need access to the map object
    if(country_index < 0 || country_index > num_countries_) {
        cout << "Country " << *country->GetCountryName() << " has no neighbours!\n";
    } else {
        //iterate over adjacency matrix to obtain the list of neighbours
        for(int row = 0; row < num_countries_; ++row) {
            //find the current country's neighbour list
            if(row == country_index) {
                for(int col = 0; col < num_countries_; ++col) {
                    //find the countries that are neighbours
                    if(adjacency_matrix_[row][col] && col != country_index) {
                        int id_of_neighbour = col + 1;
                        if(id_of_neighbour == country->GetCountryID()) {
                            continue;
                        }
                        Country* neighbouring_country = GetCountryById(id_of_neighbour);

                        if(*neighbouring_country == *country) {
                            continue;
                        }

                        if(neighbouring_country && neighbouring_country->GetNumberOfArmies() > 0) {
                            neighbouring_countries->push_back(neighbouring_country);
                        }
                    }
                }
            }
        }
    }

    return neighbouring_countries;
}

vector<Country*>* Map::GetNeighbouringCountries(Country* country) const {
    int country_index = country->GetCountryID() - 1;
    vector<Country*>* neighbouring_countries = new vector<Country*>;
    vector<int> neighbouring_countries_indices;

    //need access to the map object
    if(country_index < 0 || country_index > num_countries_) {
        cout << "Country " << *country->GetCountryName() << " has no neighbours!\n";
    } else {
        //iterate over adjacency matrix to obtain the list of neighbours
        for(int row = 0; row < num_countries_; ++row) {
            //find the current country's neighbour list
            if(row == country_index) {
                for(int col = 0; col < num_countries_; ++col) {
                    //find the countries that are neighbours
                    if(adjacency_matrix_[row][col] && col != country_index) {
                        int id_of_neighbour = col + 1;

                        if(id_of_neighbour == country->GetCountryID()) {
                            continue;
                        }
                        Country* neighbouring_country = GetCountryById(id_of_neighbour);

                        if(*neighbouring_country == *country) {
                            continue;
                        }

                        neighbouring_countries->push_back(neighbouring_country);
                    }
                }
            }
        }
    }

    return neighbouring_countries;
}

string Map::GenerateListOfNeighboringCountries(Country *country) const {
    vector<Country*>* neighbours = GetNeighbouringCountries(country);
    if(neighbours->empty()) {
        return "";
    }

    string neighbour_list;

    for(int neighbour = 0; neighbour < neighbours->size(); ++neighbour) {
        string name;
        int num_armies = (*neighbours)[neighbour]->GetNumberOfArmies();
        string armies = " | #armies: ";
        armies.append(std::to_string(num_armies));

        if(neighbour == neighbours->size() - 1) {
            name = *(*neighbours)[neighbour]->GetCountryName() + armies;
        } else {
            name = *(*neighbours)[neighbour]->GetCountryName() + armies + ", " ;
        }

        neighbour_list.append(name);
    }

    return neighbour_list;
}
//--------------------------------------------------------------------------------------------