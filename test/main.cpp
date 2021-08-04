#include <iostream>
#include <string>
#include <limits.h>

#include "include/IntervalMap.h"

using namespace std;

struct TimeVariantData
{
    TimeVariantData():
        m_timeStamp(-1)
      , m_value(-1)
    {}

    TimeVariantData(long long timeStamp, int val):
        m_timeStamp(timeStamp)
      , m_value(val)
    {}

    ~TimeVariantData()
    {}

    friend std::ostream &operator << (std::ostream &os, const TimeVariantData &data){
        os<< "time stamp: " << data.m_timeStamp << " / "
          << "value: " << data.m_value;
        return os;
    }

    bool operator==(const TimeVariantData& data)
    {
        return (m_timeStamp == data.m_timeStamp &&
                m_value == data.m_value);

    }

    long long m_timeStamp;
    int m_value;
};

int main(int argc, char *argv[])
{
    // If call findObject with undefined interval, this data will be return
    TimeVariantData initData;
    IntervalMap<long long, TimeVariantData> interValMap{initData};

    interValMap.insert(0, LLONG_MAX, TimeVariantData(0, 1));
    interValMap.insert(5, LLONG_MAX,TimeVariantData(5, 2));
    interValMap.insert(10, LLONG_MAX,TimeVariantData(10, 3));

    cout<< interValMap << endl;

    auto data = interValMap.findObject(0);
    cout << *data <<endl;   // time stamp: 0 / value: 1

    data = interValMap.findObject(3);
    cout << *data <<endl;   // time stamp: 0 / value: 1

    data = interValMap.findObject(5);
    cout << *data <<endl;   // time stamp: 5 / value: 2

    data = interValMap.findObject(11);
    cout << *data <<endl;   // time stamp: 10 / value: 3

    data = interValMap.findObject(-2);
    cout << *data <<endl;   // time stamp: -1 / value: -1

    long long endPoint = interValMap.getEndPoint(2);
    cout << endPoint <<endl;   // 5

    long long beginPoint = interValMap.getBeginPoint(2);
    cout << beginPoint <<endl;   // 0

    return 0;

}
