#pragma once

#include "cuckoo_base.h"


template<class K, class D, class HF = std::hash<K>,
         class Config = CuckooConfig<> >
class TSimpleCuckoo : public CuckooTraits<TSimpleCuckoo<K,D,HF,Config> >::Base_t
{
private:
    using This_t         = TSimpleCuckoo<K,D,HF,Config>;
    using Base_t         = typename CuckooTraits<This_t>::Base_t;
    using Bucket_t       = typename CuckooTraits<This_t>::Bucket_t;
    using HashSplitter_t = typename CuckooTraits<This_t>::HashSplitter_t;

    friend Base_t;

public:
    using Key            = typename CuckooTraits<This_t>::Key;
    using Data           = typename CuckooTraits<This_t>::Data;

    static constexpr size_t bs = CuckooTraits<This_t>::Config_t::bs;
    static constexpr size_t tl = 1;

    TSimpleCuckoo(size_t cap = 0      , double size_constraint = 1.1,
                 size_t dis_steps = 0, size_t seed = 0)
        : Base_t(std::max(size_t((cap*size_constraint)/bs)*bs, bs), size_constraint,
                 dis_steps, seed),
          n_buckets(std::max(size_t((cap*size_constraint)/bs), 1ul)),
          table(new Bucket_t[n_buckets])
    { }

    TSimpleCuckoo(const TSimpleCuckoo&) = delete;
    TSimpleCuckoo& operator=(const TSimpleCuckoo&) = delete;

    TSimpleCuckoo(TSimpleCuckoo&&) = default;
    TSimpleCuckoo& operator=(TSimpleCuckoo&&) = default;

    std::pair<size_t, Bucket_t*> getTable(size_t i)
    {
        return (! i) ? std::make_pair(n_buckets, table.get())
                     : std::make_pair(0,nullptr);
    }

private:
    size_t n_buckets;
    std::unique_ptr<Bucket_t[]> table;

    inline Bucket_t* getBucket1(HashSplitter_t h) const
    { return &(table[h.loc1 % n_buckets]); }

    inline Bucket_t* getBucket2(HashSplitter_t h) const
    { return &(table[h.loc2 % n_buckets]); }
};


template<class K, class D, class HF,
         class Config>
class CuckooTraits<TSimpleCuckoo<K,D,HF,Config> >
{
public:
    using Specialized_t  = TSimpleCuckoo<K,D,HF,Config>;
    using Base_t         = CuckooBase<Specialized_t>;
    using Key            = K;
    using Data           = D;
    using HashFct_t      = HF;
    using Config_t       = Config;

    static constexpr size_t tl = 1;
    static constexpr size_t bs = Config::bs;

    using Bucket_t       = Bucket<K,D,bs>;

    union HashSplitter_t
    {
        uint64_t hash;
        struct
        {
            uint64_t loc1 : 32;
            uint64_t loc2 : 32;
        };
    };
};

template<class K, class D, class HF = std::hash<K>,
         template<class> class DS = dstrat_triv,
         size_t BS = 8>
using SimpleCuckoo = TSimpleCuckoo<K,D,HF,CuckooConfig<BS,1,DS,no_hist_count> >;

template<class K, class D, class HF = std::hash<K>,
         template<class> class DS = dstrat_triv,
         size_t BS = 8>
using SimpleCuckooHist = TSimpleCuckoo<K,D,HF,CuckooConfig<BS,1,DS,hist_count> >;

template<class K, class D, class HF = std::hash<K>,
         template<class> class DS = dstrat_triv,
         size_t /* does nothing */ = 0, size_t BS = 8>
using SimpleCuckooWrap = TSimpleCuckoo<K,D,HF,CuckooConfig<BS,1,DS,no_hist_count> >;

template<class K, class D, class HF = std::hash<K>,
         template<class> class DS = dstrat_triv,
         size_t /* does nothing */ = 0, size_t BS = 8>
using SimpleCuckooWrapHist = TSimpleCuckoo<K,D,HF,CuckooConfig<BS,1,DS,hist_count> >;