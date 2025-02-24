#ifndef __DACE_SERDE__
#define __DACE_SERDE__

#include <cassert>
#include <istream>
#include <iostream>
#include <sstream>
#include <optional>

#include "get_albedos.h"

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


void deserialize(config_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# i_emiss_from_band_lw"});  // Should contain '# i_emiss_from_band_lw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->i_emiss_from_band_lw = m.read<std::remove_pointer<decltype(x ->i_emiss_from_band_lw)>::type>(s);

read_line(s, {"# sw_albedo_weights"});  // Should contain '# sw_albedo_weights'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->sw_albedo_weights = m.read<std::remove_pointer<decltype(x ->sw_albedo_weights)>::type>(s);

read_line(s, {"# i_band_from_reordered_g_lw"});  // Should contain '# i_band_from_reordered_g_lw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->i_band_from_reordered_g_lw = m.read<std::remove_pointer<decltype(x ->i_band_from_reordered_g_lw)>::type>(s);

read_line(s, {"# i_band_from_reordered_g_sw"});  // Should contain '# i_band_from_reordered_g_sw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->i_band_from_reordered_g_sw = m.read<std::remove_pointer<decltype(x ->i_band_from_reordered_g_sw)>::type>(s);

}


void deserialize(single_level_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# sw_albedo_direct"});  // Should contain '# sw_albedo_direct'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->sw_albedo_direct = m.read<std::remove_pointer<decltype(x ->sw_albedo_direct)>::type>(s);

read_line(s, {"# lw_emissivity"});  // Should contain '# lw_emissivity'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_emissivity = m.read<std::remove_pointer<decltype(x ->lw_emissivity)>::type>(s);

read_line(s, {"# sw_albedo"});  // Should contain '# sw_albedo'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->sw_albedo = m.read<std::remove_pointer<decltype(x ->sw_albedo)>::type>(s);

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


std::string serialize(const config_type* x) {
    std::stringstream s;
    add_line("# i_emiss_from_band_lw", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->i_emiss_from_band_lw];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->i_emiss_from_band_lw[i]), s);
    }
}

add_line("# sw_albedo_weights", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->sw_albedo_weights];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->sw_albedo_weights[i]), s);
    }
}

add_line("# i_band_from_reordered_g_lw", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->i_band_from_reordered_g_lw];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->i_band_from_reordered_g_lw[i]), s);
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


std::string serialize(const single_level_type* x) {
    std::stringstream s;
    add_line("# sw_albedo_direct", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->sw_albedo_direct];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->sw_albedo_direct[i]), s);
    }
}

add_line("# lw_emissivity", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_emissivity];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_emissivity[i]), s);
    }
}

add_line("# sw_albedo", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->sw_albedo];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->sw_albedo[i]), s);
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