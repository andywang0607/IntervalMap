#ifndef INTERVALMAP_H
#define INTERVALMAP_H

#include <iostream>
#include <iterator>
#include <map>
#include <limits>
#include <mutex>
#include <memory>
#include <sstream>

/**
 *  IntervalMap is a thread safe map enable to map a interval to a object
 */
template<typename K, typename V>
class IntervalMap {
    std::map<K, V> m_map;
    mutable std::mutex mut;

public:
    /// Delete default constructor: insert a V instance for those interval not defined
    IntervalMap() = delete;

    /**
     * @brief Copy construtor
     * @param val a reference of V for not defined interval
     */
    IntervalMap(V const& val) {
        m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
    }

    /**
     * @brief Move constructor
     * @param val rvalue reference for not defined interval
     */
    IntervalMap(V const&& val) {
        m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
    }

    /**
     * @brief insert Insert a interval to a value
     * @param keyBegin begin point of interval
     * @param keyEnd end point of interval
     * @param val object or value for insert
     * @return
     */
    bool insert(K const& keyBegin, K const& keyEnd, V const& val) {
        if (!(keyBegin < keyEnd))
            return false;

        using mapIter = typename decltype(m_map)::iterator;
        std::lock_guard<std::mutex> lk(mut);

        /// End point of the input range
        mapIter iterEnd = m_map.find(keyEnd);

        if (iterEnd != m_map.end()){
            auto l = m_map.lower_bound(keyEnd);
            iterEnd->second = l->second;
        } else { // This interval is not defined
            iterEnd = m_map.insert(m_map.end(), std::make_pair(keyEnd, m_map.begin()->second));
        }

        /// Begin point of the input range
        auto iterBegin = m_map.insert_or_assign(keyBegin, val).first;

        /// Cleanup the new range
        m_map.erase(std::next(iterBegin), iterEnd);

        /// Clear redundant
        // For example:
        // There are same object(value) in both [5, 10) and [7, 10) (Overlap)
        // Clear [7, 10) in this step
        auto iterRight = iterEnd;
        auto iterLeft = (iterBegin != m_map.begin() ? std::prev(iterBegin) : iterBegin);
        while (iterRight != iterLeft) {
            auto iterNext = std::prev(iterRight);
            if (!(iterRight->second == iterNext->second)) {
                iterRight = iterNext;
                continue;
            }
            // Delete redundant element
            m_map.erase(iterRight);
            iterRight = iterNext;
        }
        return true;
    }

    /**
     * @brief getEndPoint
     * @param val Value for search
     * @return End point of interval which contain val
     */
    K getEndPoint(K val) const {
        std::lock_guard<std::mutex> lk(mut);
        return (m_map.upper_bound(val))->first;
    }

    /**
     * @brief getBeginPoint
     * @param val Value for search
     * @return Begin point of interval which contain val
     */
    K getBeginPoint(K val) const {
        std::lock_guard<std::mutex> lk(mut);
        return (--m_map.upper_bound(val))->first;
    }

    /**
     * @brief operator []
     * @param key Key to be searched for
     * @return Element with specified key is found
     */
    V const& operator[](K const& key) const {
        std::lock_guard<std::mutex> lk(mut);
        return (--m_map.upper_bound(key))->second;
    }

    /**
     * @brief find Searches the container for an element with a key equivalent to k and return it
     * @param key Key to be searched for
     * @note I designed it for Fundamental types, Suggest to apply findObject if type of K is big class
     * @return Element with specified key is found
     */
    V find(K const& key) const {
        std::lock_guard<std::mutex> lk(mut);
        return (--m_map.upper_bound(key)->second);
    }

    /**
     * @brief findObject find Searches the container for an element with a key equivalent to k and return its pointer
     * @param key Key to be searched for
     * @return Pointer of element with specified key is found
     */
    std::shared_ptr<V> findObject(K const& key) const {
        std::lock_guard<std::mutex> lk(mut);
        return std::make_shared<V>((--m_map.upper_bound(key))->second);
    }

    /**
     * @brief operator << Enable to print this class with simple cout
     */
    friend std::ostream &operator << (std::ostream &os, const IntervalMap &map){
        for (auto&&[key, val] : map.m_map)
            os<< "[" << key << ":" << val << "]";
        os << "\n";
        return os;
    }
};

#endif // INTERVALMAP_H
