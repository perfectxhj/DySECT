#pragma once

#include <memory>
#include <iostream>
#include <tuple>

template <class Parent>
class dstrat_triv
{
public:
    using Key            = typename Parent::Key;
    using Data           = typename Parent::Data;
    using Hashed_t       = typename Parent::Hashed_t;

    dstrat_triv(Parent&, size_t, size_t) {}
    dstrat_triv(Parent&, dstrat_triv&&) {}

    inline int insert(std::pair<Key, Data>, Hashed_t)
    {   return -1; }
};


class hist_count
{
public:
    hist_count(size_t s) : steps(s), hist(new size_t[s])
    { for (size_t i = 0; i < s; ++i) { hist[i] = 0; } }

    void add(size_t i) { auto ind = (i<steps) ? i:steps-1; ++hist[ind];}

    const size_t steps;
    std::unique_ptr<size_t[]> hist;
};


class no_hist_count
{
public:
    no_hist_count(size_t) { }
    void add(size_t) { }
    static constexpr size_t  steps = 0;
    static constexpr size_t* hist  = nullptr;
};

template<size_t BS = 8, size_t NH = 2, size_t TL = 256,
         template <class> class DisStrat = dstrat_triv,
         class HistCount = hist_count>
struct Config
{
    static constexpr size_t bs = BS;
    static constexpr size_t tl = TL;
    static constexpr size_t nh = NH;

    template <class T>
    using DisStrat_temp = DisStrat<T>;

    using HistCount_t = HistCount;
};