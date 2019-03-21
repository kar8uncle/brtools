#ifndef BRTOOLS_UTIL_ID_DISPENSER_H
#define BRTOOLS_UTIL_ID_DISPENSER_H
#pragma once

#include <forward_list>

namespace brtools
{
namespace util
{
    template<typename _IDType>
    class id_dispenser
    {
    public:
        id_dispenser() : _m_next_id(0) {}

        _IDType dispense()
        {
            if (_m_reuseable_ids.empty())
            {
                return _m_next_id++;
            }
            else
            {
                const auto id = _m_reuseable_ids.front();
                _m_reuseable_ids.pop_front();
                return id;
            }
        }

        void recycle(const _IDType id)
        {
            if (id == _m_next_id - 1)
            {
                _m_next_id = id;
            }
            else
            {
                _m_reuseable_ids.push_front(id);
            }
        }

    private:
        _IDType                    _m_next_id;
        std::forward_list<_IDType> _m_reuseable_ids;
    };
}
}

#endif
