#include "../include/landscape.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

namespace game {
Landscape::Landscape() : map_(generate_map(OBSTACLES_COUNT)) {
    std::string name_right = "RIGHT PLAYER";
    squad::Lord *right = new squad::Lord(name_right, player_type::RIGHT);

    std::string name_left = "LEFT PLAYER";
    squad::Lord *left = new squad::Lord(name_left, player_type::LEFT);

    set_squad(map_[MAP_SIZE_VERTICAL / 2][0], left);
    set_squad(map_[MAP_SIZE_VERTICAL / 2][MAP_SIZE_HORIZONTAL - 1], right);
    units_list_.push_back(right);
    units_list_.push_back(left);
    right_player_ = right;
    left_player_ = left;
}

Landscape::Landscape(const map_type &map) : map_(map) {
    std::string name_right = "RIGHT PLAYER";
    squad::Lord *right = new squad::Lord(name_right, player_type::RIGHT);

    std::string name_left = "LEFT PLAYER";
    squad::Lord *left = new squad::Lord(name_left, player_type::LEFT);

    set_squad(map_[MAP_SIZE_VERTICAL / 2][0], left);
    set_squad(map_[MAP_SIZE_VERTICAL / 2][MAP_SIZE_HORIZONTAL - 1], right);
    units_list_.push_back(right);
    units_list_.push_back(left);
    right_player_ = right;
    left_player_ = left;
}

Landscape::Landscape(const map_type &map, const queue &units_list, squad::Lord *left_player, squad::Lord *right_player)
    : map_(map),
      units_list_(units_list),
      left_player_(left_player),
      right_player_(right_player) {}

Landscape::Landscape(const Landscape &game) : Landscape(game.map_,
                                                        game.units_list_,
                                                        game.left_player_,
                                                        game.right_player_) {}

Landscape::Landscape(Landscape &&game) : map_(game.map_),
                                         units_list_(game.units_list_),
                                         left_player_(game.left_player_),
                                         right_player_(game.right_player_) {
    game.units_list_ = {};
}

void Landscape::free_squad(squad::Squad *squad) {
    auto name = squad->get_name();
    std::string moral_name = "M";
    std::string amoral_name = "A";
    std::string im_moral_name = "IM";
    std::string im_amoral_name = "IA";

    if (constant::unit_type[name] == moral_name) {
        squad::Moral *moral = static_cast<squad::Moral *>(squad);
        delete moral;
    } else if (constant::unit_type[name] == amoral_name) {
        squad::Amoral *amoral = static_cast<squad::Amoral *>(squad);
        delete amoral;
    } else if (constant::unit_type[name] == im_moral_name) {
        squad::Immortal_moral *im_moral =
            static_cast<squad::Immortal_moral *>(squad);
        delete im_moral;
    } else if (constant::unit_type[name] == im_amoral_name) {
        squad::Immortal_amoral *im_amoral =
            static_cast<squad::Immortal_amoral *>(squad);
        delete im_amoral;
    } else {
        squad::Lord *lord = static_cast<squad::Lord *>(squad);
        delete lord;
    }
}

Landscape::~Landscape() {
    for (auto &row : map_)
        for (auto &cell : row)
            if (cell.get_squad() != nullptr)
                cell.free_squad();

    while (!(units_list_.empty())) {
        auto *squad = units_list_.front();
        free_squad(squad);
        units_list_.pop_front();
    }
}

void Landscape::set_map(const map_type &map) { map_ = map; }

void Landscape::set_units_list(const queue &units_list) {
    units_list_ = units_list;
}

Landscape::map_type &Landscape::get_map() { return map_; }

Landscape::queue &Landscape::get_units_list() { return units_list_; }

void Landscape::move_squad(field::Cell &cell_from, field::Cell &cell_to) {
    cell_to.set_squad(cell_from.get_squad());
    cell_from.free_squad();
}

void Landscape::set_squad(field::Cell &cell, squad::Squad *squad) {
    cell.set_squad(squad);
}

void Landscape::play_next(char command, std::vector<unsigned> args) {
    auto current_squad = get_next();
    auto name = current_squad->get_name();
    auto type = constant::unit_type[name];

    std::string moral_name = "M";
    std::string amoral_name = "A";
    std::string im_moral_name = "IM";
    std::string im_amoral_name = "IA";
    std::string lord_name = "L";

    switch (command) {
        case 'm': {  // move
            if (constant::unit_type[name] == lord_name)
                throw std::invalid_argument("Lord cannot move");
            else if (args[0] != args[2] || args[1] != args[3]) {
                move_squad(map_[args[0]][args[1]], map_[args[2]][args[3]]);
            }
        } break;
        case 'a': {  // attack
            if (args[0] != args[2] || args[1] != args[3]) {
                move_squad(map_[args[0]][args[1]], map_[args[2]][args[3]]);
            }

            auto squad_to_attack = map_[args[4]][args[5]].get_squad();
            unsigned exp = 0;

            if (squad_to_attack == nullptr)
                throw std::invalid_argument("no squad here");
            if (constant::unit_type[name] == moral_name) {
                auto moral = static_cast<squad::Moral *>(current_squad);

                if (squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL) % UNITS_COUNT ||
                    squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL * 2) % UNITS_COUNT) {
                    moral->modify_damage(5);
                    exp = moral->attack(*squad_to_attack);
                    moral->modify_damage(-5);
                } else {
                    exp = moral->attack(*squad_to_attack);
                }

                if (exp > 0)
                    moral->modify_moral(5);

            } else if (constant::unit_type[name] == amoral_name) {
                auto amoral = static_cast<squad::Amoral *>(current_squad);

                if (squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL) % UNITS_COUNT ||
                    squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL * 2) % UNITS_COUNT) {
                    amoral->modify_damage(5);
                    exp = amoral->attack(*squad_to_attack);
                    amoral->modify_damage(-5);
                } else {
                    exp = amoral->attack(*squad_to_attack);
                }

            } else if (constant::unit_type[name] == im_moral_name) {
                auto im_moral =
                    static_cast<squad::Immortal_moral *>(current_squad);

                if (squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL) % UNITS_COUNT ||
                    squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL * 2) % UNITS_COUNT) {
                    im_moral->modify_damage(5);
                    exp = im_moral->attack(*squad_to_attack);
                    im_moral->modify_damage(-5);
                } else {
                    exp = im_moral->attack(*squad_to_attack);
                }

                if (exp > 0)
                    im_moral->modify_moral(5);

            } else if (constant::unit_type[name] == im_amoral_name) {
                auto im_amoral =
                    static_cast<squad::Immortal_amoral *>(current_squad);

                if (squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL) % UNITS_COUNT ||
                    squad_to_attack->get_name() ==
                        (name + UNITS_PER_SCHOOL * 2) % UNITS_COUNT) {
                    im_amoral->modify_damage(5);
                    exp = im_amoral->attack(*squad_to_attack);
                    im_amoral->modify_damage(-5);
                } else {
                    exp = im_amoral->attack(*squad_to_attack);
                }

            } else
                throw std::invalid_argument("You cannot attack");

            if (static_cast<player_type>(current_squad->get_team()) == player_type::LEFT) {
                left_player_->modify_experience(exp);
                left_player_->modify_energy(exp);
            } else {
                right_player_->modify_experience(exp);
                right_player_->modify_energy(exp);
            }

        } break;
        case 'u': {  // upgrade school
            if (constant::unit_type[name] == lord_name) {
                auto lord = static_cast<squad::Lord *>(current_squad);
                auto school = static_cast<constant::school_type>(args[0]);
                lord->upgrade_school(school);
            } else
                throw std::invalid_argument("You cannot upgrade school");
        } break;
        case 'c': {  // call squad
            if (constant::unit_type[name] == lord_name) {
                auto lord = static_cast<squad::Lord *>(current_squad);
                auto squad = static_cast<constant::unit>(args[0]);
                auto *squad_called = lord->call_squad(squad);
                units_list_.push_back(squad_called);
                set_squad(map_[args[1]][args[2]], squad_called);
            } else
                throw std::invalid_argument("You cannot call squad");
        } break;
        case 'h': {  // heal
            if (constant::unit_type[name] == im_moral_name) {
                auto im_moral =
                    static_cast<squad::Immortal_moral *>(current_squad);
                im_moral->heal();
            } else if (constant::unit_type[name] == im_amoral_name) {
                auto im_amoral =
                    static_cast<squad::Immortal_amoral *>(current_squad);
                im_amoral->heal();
            } else
                throw std::invalid_argument("You cannot heal");
        } break;
        default:
            break;
    }

    for (auto *unit : units_list_) {
        if (constant::unit_type[unit->get_name()] == moral_name) {
            auto moral = static_cast<squad::Moral *>(unit);
            moral->balance_moral();
        } else if (constant::unit_type[unit->get_name()] == im_moral_name) {
            auto im_moral = static_cast<squad::Immortal_moral *>(unit);
            im_moral->balance_moral();
        }
    }

    units_list_.pop_front();
    units_list_.push_back(current_squad);

    for (int i = 0; i < units_list_.size(); i++) {
        if (units_list_[i]->get_health() <= 0) {
            for (auto &row : map_)
                for (auto &cell : row)
                    if (cell.get_squad() == units_list_[i]) {
                        cell.free_squad();
                        break;
                    }
            if (constant::unit_type[units_list_[i]->get_name()] != lord_name)
                free_squad(units_list_[i]);
            units_list_.erase(units_list_.begin() + i);
        }
    }
}

squad::Squad *Landscape::get_next() { return units_list_.front(); }

Landscape::map_type Landscape::generate_map(const size_t &obstacles_count) {
    std::srand(std::time(nullptr));
    Landscape::map_type map;
    map.reserve(MAP_SIZE_VERTICAL);
    size_t count = 0;
    auto type = field::cell_type::FREE;

    for (size_t i = 0; i < MAP_SIZE_VERTICAL; i++) {
        map.emplace_back();
        map[i].reserve(MAP_SIZE_HORIZONTAL);

        for (size_t j = 0; j < MAP_SIZE_HORIZONTAL; j++) {
            if ((std::rand() % 2 == 1) &&
                count < obstacles_count &&
                j % 3 != 0 &&
                abs(MAP_SIZE_VERTICAL / 2 - i) < (MAP_SIZE_VERTICAL / 2) &&
                abs(MAP_SIZE_HORIZONTAL / 2 - j) < (MAP_SIZE_HORIZONTAL / 5)) {
                type = field::cell_type::OBSTAClE;
                count++;
            } else
                type = field::cell_type::FREE;

            map[i].push_back(field::Cell(i, j, type));
        }
    }

    return map;
}

field::Point Landscape::find_squad(squad::Squad *squad) {
    for (auto &row : map_)
        for (auto &cell : row)
            if (cell.get_squad() == squad)
                return cell.get_coordinates();

    throw std::invalid_argument("No such squad on map");
}

void Landscape::save_game() const {
    // TODO
}

void Landscape::load_game() {
    // TODO
}

squad::Lord *Landscape::get_left_player() { return left_player_; }

squad::Lord *Landscape::get_right_player() { return right_player_; }

Landscape &Landscape::operator=(const Landscape &game) {
    if (this == &game)
        return *this;

    while (!(units_list_.empty())) {
        auto squad = units_list_.front();
        delete squad;
        units_list_.pop_front();
    }

    map_ = game.map_;

    while (units_list_.size() != game.units_list_.size()) {
        auto squad = game.units_list_.front();
        using namespace squad;
        constant::unit name = squad->get_name();
        if (constant::unit_type[name] == "M") {
            moral_type type = convert_to_moral(name);
            Moral *squad = new Moral(type);
            units_list_.push_back(squad);
        } else if (constant::unit_type[name] == "A") {
            amoral_type type = convert_to_amoral(name);
            Amoral *squad = new Amoral(type);
            units_list_.push_back(squad);
        } else if (constant::unit_type[name] == "IM") {
            immortal_moral_type type = convert_to_immortal_moral(name);
            Immortal_moral *squad = new Immortal_moral(type);
            units_list_.push_back(squad);
        } else if (constant::unit_type[name] == "IA") {
            immortal_amoral_type type = convert_to_immortal_amoral(name);
            Immortal_amoral *squad = new Immortal_amoral(type);
            units_list_.push_back(squad);
        }
    }

    return *this;
}

Landscape &Landscape::operator=(Landscape &&game) {
    std::swap(map_, game.map_);
    std::swap(units_list_, game.units_list_);
    return *this;
}

bool Landscape::operator==(const Landscape &game) const {
    if (map_ == game.map_ && units_list_ == game.units_list_)
        return true;
    else
        return false;
}

bool Landscape::operator!=(const Landscape &game) const {
    return !(*this == game);
}

}  // namespace game