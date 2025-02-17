#ifndef __DACE_SERDE__
#define __DACE_SERDE__

#include <cassert>
#include <istream>
#include <iostream>
#include <sstream>
#include <optional>

#include "gas_optics.h"

namespace serde {
    std::string scroll_space(std::istream& s) {
        std::string out;
        while (!s.eof() && (!s.peek() || isspace(s.peek()))) {
            out += s.get();
            assert(s.good());
        }
        return out;
    }

    std::string read_line(std::istream& s, const std::optional<std::string>& should_contain = {}) {
        if (s.eof()) return "<eof>";
        scroll_space(s);
        char bin[101];
        s.getline(bin, 100);
        assert(s.good());
        if (should_contain) {
            bool ok = (std::string(bin).find(*should_contain) != std::string::npos);
            if (!ok) {
                std::cerr << "Expected: '" << *should_contain << "'; got: '" << bin << "'" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return {bin};
    }

    struct array_meta;
    std::map<void*, array_meta>* ARRAY_META_DICT();

    struct array_meta {
        int rank = 0;
        std::vector<int> size, lbound;

        int volume() const {  return std::reduce(size.begin(), size.end(), 1, std::multiplies<int>()) ; }

        template<typename T> T* read(std::istream& s) const;
    };
    std::map<void*, array_meta>* ARRAY_META_DICT() {
        static auto* M = new std::map<void*, array_meta>();
        return M;
    }

    template<typename T>
    void read_scalar(T& x, std::istream& s) {
        if (s.eof()) return;
        scroll_space(s);
        s >> x;
    }

    void read_scalar(float& x, std::istream& s) {
        if (s.eof()) return;
        scroll_space(s);
        long double y;
        s >> y;
        x = y;
    }

    void read_scalar(double& x, std::istream& s) {
        if (s.eof()) return;
        scroll_space(s);
        long double y;
        s >> y;
        x = y;
    }

    void read_scalar(bool& x, std::istream& s) {
        char c;
        read_scalar(c, s);
        assert (c == '1' or c == '0');
        x = (c == '1');
    }

    array_meta read_array_meta(std::istream& s) {
        array_meta m;
        read_line(s, {"# rank"});  // Should contain '# rank'
        read_scalar(m.rank, s);
        m.size.resize(m.rank);
        m.lbound.resize(m.rank);
        read_line(s, {"# size"});  // Should contain '# size'
        for (int i=0; i<m.rank; ++i) {
            read_scalar(m.size[i], s);
        }
        read_line(s, {"# lbound"});  // Should contain '# lbound'
        for (int i=0; i<m.rank; ++i) {
            read_scalar(m.lbound[i], s);
        }
        return m;
    }

    template<typename T>
    std::pair<array_meta, T*> read_array(std::istream& s) {
        auto m = serde::read_array_meta(s);
        auto* y = m.read<T>(s);
        return {m, y};
    }

    
void deserialize(float* x, std::istream& s) {
    read_scalar(*x, s);
}
void deserialize(double* x, std::istream& s) {
    read_scalar(*x, s);
}
void deserialize(long double* x, std::istream& s) {
    read_scalar(*x, s);
}
void deserialize(int* x, std::istream& s) {
    read_scalar(*x, s);
}
void deserialize(long* x, std::istream& s) {
    read_scalar(*x, s);
}
void deserialize(long long* x, std::istream& s) {
    read_scalar(*x, s);
}
void deserialize(bool* x, std::istream& s) {
    read_scalar(*x, s);
}


void deserialize(gas_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# mixing_ratio"});  // Should contain '# mixing_ratio'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->mixing_ratio = m.read<std::remove_pointer<decltype(x ->mixing_ratio)>::type>(s);

}


void deserialize(config_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# i_band_from_g_lw"});  // Should contain '# i_band_from_g_lw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->i_band_from_g_lw = m.read<std::remove_pointer<decltype(x ->i_band_from_g_lw)>::type>(s);

read_line(s, {"# i_band_from_reordered_g_sw"});  // Should contain '# i_band_from_reordered_g_sw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->i_band_from_reordered_g_sw = m.read<std::remove_pointer<decltype(x ->i_band_from_reordered_g_sw)>::type>(s);

}


void deserialize(thermodynamics_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# temperature_fl"});  // Should contain '# temperature_fl'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->temperature_fl = m.read<std::remove_pointer<decltype(x ->temperature_fl)>::type>(s);

read_line(s, {"# pressure_hl"});  // Should contain '# pressure_hl'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->pressure_hl = m.read<std::remove_pointer<decltype(x ->pressure_hl)>::type>(s);

read_line(s, {"# temperature_hl"});  // Should contain '# temperature_hl'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->temperature_hl = m.read<std::remove_pointer<decltype(x ->temperature_hl)>::type>(s);

read_line(s, {"# pressure_fl"});  // Should contain '# pressure_fl'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->pressure_fl = m.read<std::remove_pointer<decltype(x ->pressure_fl)>::type>(s);

}


void deserialize(single_level_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# skin_temperature"});  // Should contain '# skin_temperature'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->skin_temperature = m.read<std::remove_pointer<decltype(x ->skin_temperature)>::type>(s);

read_line(s, {"# spectral_solar_scaling"});  // Should contain '# spectral_solar_scaling'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->spectral_solar_scaling = m.read<std::remove_pointer<decltype(x ->spectral_solar_scaling)>::type>(s);

read_line(s, {"# cos_sza"});  // Should contain '# cos_sza'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->cos_sza = m.read<std::remove_pointer<decltype(x ->cos_sza)>::type>(s);

}

    
template<typename T>
void add_line(const T& x, std::ostream& s, bool trailing_newline=true) {
    s << x;
    if (trailing_newline) s << std::endl;
}
void add_line(long long x, std::ostream& s, bool trailing_newline=true) {
    s << x;
    if (trailing_newline) s << std::endl;
}
void add_line(long double x, std::ostream& s, bool trailing_newline=true) {
    s << x;
    if (trailing_newline) s << std::endl;
}
void add_line(bool x, std::ostream& s, bool trailing_newline=true) {
    add_line(int(x), s, trailing_newline);
}
template<typename T>
std::string serialize(const T* x) {
    std::stringstream s;
    add_line(*x, s, false);
    return s.str();
}
std::string serialize(int x) {
    return std::to_string(x);
}
std::string serialize(long x) {
    return std::to_string(x);
}
std::string serialize(long long x) {
    return std::to_string(x);
}
std::string serialize(float x) {
    return std::to_string(x);
}
std::string serialize(double x) {
    return std::to_string(x);
}
std::string serialize(long double x) {
    return std::to_string(x);
}
std::string serialize(bool x) {
    return serialize(int(x));
}


std::string serialize(const gas_type* x) {
    std::stringstream s;
    add_line("# mixing_ratio", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->mixing_ratio];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->mixing_ratio[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const config_type* x) {
    std::stringstream s;
    add_line("# i_band_from_g_lw", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->i_band_from_g_lw];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->i_band_from_g_lw[i]), s);
    }
}

add_line("# i_band_from_reordered_g_sw", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->i_band_from_reordered_g_sw];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->i_band_from_reordered_g_sw[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const thermodynamics_type* x) {
    std::stringstream s;
    add_line("# temperature_fl", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->temperature_fl];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->temperature_fl[i]), s);
    }
}

add_line("# pressure_hl", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->pressure_hl];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->pressure_hl[i]), s);
    }
}

add_line("# temperature_hl", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->temperature_hl];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->temperature_hl[i]), s);
    }
}

add_line("# pressure_fl", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->pressure_fl];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->pressure_fl[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const single_level_type* x) {
    std::stringstream s;
    add_line("# skin_temperature", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->skin_temperature];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->skin_temperature[i]), s);
    }
}

add_line("# spectral_solar_scaling", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->spectral_solar_scaling];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->spectral_solar_scaling[i]), s);
    }
}

add_line("# cos_sza", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->cos_sza];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->cos_sza[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


    template<typename T>
    T* array_meta::read(std::istream& s) const {
        read_line(s, {"# entries"});
        auto* buf = new T[volume()];
        for (int i=0; i<volume(); ++i) {
            deserialize(&buf[i], s);
        }
        (*ARRAY_META_DICT())[buf] = *this;
        return buf;
    }
}  // namesepace serde

#endif // __DACE_SERDE__