#pragma once

static constexpr size_t STATIC_MAP_MAX_VALUES = 10;

template<typename T, typename U>
struct constexpr_couple {
    T key;
    U value;
};

template<typename T, typename U>
class constexpr_map {
public:
    constexpr constexpr_map (std::array<constexpr_couple<T, U>, STATIC_MAP_MAX_VALUES> values) : m_values(values) {
    }

    constexpr U get(T key) const {
        for (auto it = m_values.cbegin(); it != m_values.cend(); ++it) {
            if (it->key == key) {
                return it->value;
            }
        }

        return U{};
    }

private:
    std::array<constexpr_couple<T, U>, STATIC_MAP_MAX_VALUES> m_values;
};

