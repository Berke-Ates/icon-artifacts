#ifndef __DACE_SERDE__
#define __DACE_SERDE__

#include <cassert>
#include <istream>
#include <iostream>
#include <sstream>
#include <optional>

#include "solver_mcica_lw.h"

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


void deserialize(cloud_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# fraction"});  // Should contain '# fraction'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->fraction = m.read<std::remove_pointer<decltype(x ->fraction)>::type>(s);

read_line(s, {"# fractional_std"});  // Should contain '# fractional_std'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->fractional_std = m.read<std::remove_pointer<decltype(x ->fractional_std)>::type>(s);

read_line(s, {"# overlap_param"});  // Should contain '# overlap_param'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->overlap_param = m.read<std::remove_pointer<decltype(x ->overlap_param)>::type>(s);

}


void deserialize(pdf_sampler_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# val"});  // Should contain '# val'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->val = m.read<std::remove_pointer<decltype(x ->val)>::type>(s);

}


void deserialize(config_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# i_band_from_reordered_g_lw"});  // Should contain '# i_band_from_reordered_g_lw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->i_band_from_reordered_g_lw = m.read<std::remove_pointer<decltype(x ->i_band_from_reordered_g_lw)>::type>(s);

read_line(s, {"# pdf_sampler"});  // Should contain '# pdf_sampler'

x ->pdf_sampler = new std::remove_pointer<decltype(x ->pdf_sampler)>::type;
deserialize(x->pdf_sampler, s);

}


void deserialize(flux_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# lw_up"});  // Should contain '# lw_up'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_up = m.read<std::remove_pointer<decltype(x ->lw_up)>::type>(s);

read_line(s, {"# lw_dn"});  // Should contain '# lw_dn'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_dn = m.read<std::remove_pointer<decltype(x ->lw_dn)>::type>(s);

read_line(s, {"# lw_up_clear"});  // Should contain '# lw_up_clear'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_up_clear = m.read<std::remove_pointer<decltype(x ->lw_up_clear)>::type>(s);

read_line(s, {"# lw_dn_clear"});  // Should contain '# lw_dn_clear'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_dn_clear = m.read<std::remove_pointer<decltype(x ->lw_dn_clear)>::type>(s);

read_line(s, {"# lw_dn_surf_g"});  // Should contain '# lw_dn_surf_g'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_dn_surf_g = m.read<std::remove_pointer<decltype(x ->lw_dn_surf_g)>::type>(s);

read_line(s, {"# lw_dn_surf_clear_g"});  // Should contain '# lw_dn_surf_clear_g'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->lw_dn_surf_clear_g = m.read<std::remove_pointer<decltype(x ->lw_dn_surf_clear_g)>::type>(s);

read_line(s, {"# cloud_cover_lw"});  // Should contain '# cloud_cover_lw'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->cloud_cover_lw = m.read<std::remove_pointer<decltype(x ->cloud_cover_lw)>::type>(s);

}


void deserialize(randomnumberstream* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# iused"});  // Should contain '# iused'

deserialize(&(x->iused), s);

read_line(s, {"# inittest"});  // Should contain '# inittest'

deserialize(&(x->inittest), s);

read_line(s, {"# ix"});  // Should contain '# ix'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->ix = m.read<std::remove_pointer<decltype(x ->ix)>::type>(s);

read_line(s, {"# zrm"});  // Should contain '# zrm'

deserialize(&(x->zrm), s);

}


void deserialize(single_level_type* x, std::istream& s) {
    bool yep;
    array_meta m;
    read_line(s, {"# iseed"});  // Should contain '# iseed'

m = read_array_meta(s);


// We only need to allocate a volume of contiguous memory, and let DaCe interpret (assuming it follows the same protocol 
// as us).
x ->iseed = m.read<std::remove_pointer<decltype(x ->iseed)>::type>(s);

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


std::string serialize(const cloud_type* x) {
    std::stringstream s;
    add_line("# fraction", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->fraction];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->fraction[i]), s);
    }
}

add_line("# fractional_std", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->fractional_std];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->fractional_std[i]), s);
    }
}

add_line("# overlap_param", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->overlap_param];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->overlap_param[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const pdf_sampler_type* x) {
    std::stringstream s;
    add_line("# val", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->val];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->val[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const config_type* x) {
    std::stringstream s;
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

add_line("# pdf_sampler", s);
add_line(serialize(x->pdf_sampler), s);
    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const flux_type* x) {
    std::stringstream s;
    add_line("# lw_up", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_up];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_up[i]), s);
    }
}

add_line("# lw_dn", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_dn];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_dn[i]), s);
    }
}

add_line("# lw_up_clear", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_up_clear];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_up_clear[i]), s);
    }
}

add_line("# lw_dn_clear", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_dn_clear];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_dn_clear[i]), s);
    }
}

add_line("# lw_dn_surf_g", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_dn_surf_g];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_dn_surf_g[i]), s);
    }
}

add_line("# lw_dn_surf_clear_g", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->lw_dn_surf_clear_g];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->lw_dn_surf_clear_g[i]), s);
    }
}

add_line("# cloud_cover_lw", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->cloud_cover_lw];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->cloud_cover_lw[i]), s);
    }
}

    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const randomnumberstream* x) {
    std::stringstream s;
    add_line("# iused", s);
add_line(serialize(x->iused), s);
add_line("# inittest", s);
add_line(serialize(x->inittest), s);
add_line("# ix", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->ix];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->ix[i]), s);
    }
}

add_line("# zrm", s);
add_line(serialize(x->zrm), s);
    std::string out = s.str();
    if (out.length() > 0) out.pop_back();
    return out;
}


std::string serialize(const single_level_type* x) {
    std::stringstream s;
    add_line("# iseed", s);

{
    const array_meta& m = (*ARRAY_META_DICT())[x->iseed];
    add_line("# rank", s);
    add_line(m.rank, s);
    add_line("# size", s);
    for (auto i : m.size) add_line(i, s);
    add_line("# lbound", s);
    for (auto i : m.lbound) add_line(i, s);
    add_line("# entries", s);
    for (int i=0; i<m.volume(); ++i) {
        add_line(serialize(x->iseed[i]), s);
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