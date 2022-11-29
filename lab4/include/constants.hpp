#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <vector>
#define UNITS_PER_SCHOOL (size_t)3

namespace constant
{
    /// @brief school types
    enum class school_type
    {
        ROBOTICS,
        ENERGISER,
        PARALLEL,
        BIOTECH,
        NANOSLIME
    };

    /// @brief units types
    enum class unit
    {
        ROBOMECH,
        CENTRY,
        COLOSSUS,
        GHOST,
        INFESTOR,
        DISRUPTOR,
        ELF,
        GNOME,
        DENDRIOD,
        MARINE,
        CYCLONE,
        REAPER,
        POLTERGEIST,
        TYPHON,
        MIMIC,
        LORD
    };
};

#endif