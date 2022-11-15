#ifndef AMORAL_HPP
#define AMORAL_HPP

#include "unit.hpp"

namespace squad
{
    enum class amoral_type
    {
        CENTRY,
        INFESTOR,
        GNOME,
        CYCLONE,
        TYPHON
    };

    class amoral : public unit
    {
    protected:
        amoral_type type;

    public:
        amoral();
        amoral(const amoral_type &);
        amoral(const amoral_type &, const unsigned &, const unsigned &);

        amoral_type get_type();
        unsigned get_damage_val();
        unsigned get_defense_val();

        void set_type(const amoral_type &);
        void set_damage_val(const unsigned &);
        void set_defense_val(const unsigned &);

        void hit(squad);
        void defence(squad);
    };
}

#endif