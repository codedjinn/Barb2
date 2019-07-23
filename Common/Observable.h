#pragma once

#include <algorithm>
#include <vector>

namespace Common
{

using namespace std;

///<summary>
 /// Defines a class that does some basic housekeeping on listeners.
 ///</summary>
template <typename T>
class Observable
{

public:

    Observable() {}
    virtual ~Observable() {}

    void AddObserver(T* instance)
    {
        bool add = false;
        if (_observers.size() == 0)
        {
            add = true;
        }
        else
        {
            bool found = false;
            for (auto cur : _observers)
            {
                if (cur == instance)
                {
                    found = true;
                    break;
                }
            }
            add = !found;
        }

        if (add)
        {
            _observers.push_back(instance);
        }
    }

    void RemoveObserver(T* instance)
    {
        for (int i = 0; i < _observers.size(); i++)
        {
            auto item = _observers.at(i);
            if (item == instance)
            {
                _observers.erase(_observers.begin() + i);
                break;
            }
        }
    }

    void ClearObservers()
    {
        _observers.clear();
    }

protected:

    vector<T*> _observers;

};

    //// Bjarne Stroustrup:
    //// Interesting templated struct that tests if class T1 is inherited from T2
    //template <class T1, class T2> struct Can_Copy
    //{
    //	static Constraints(T1 a, T2 b)
    //	{
    //		T2 c = a;
    //		b = a;
    //	}
    //
    //	Can_Copy()
    //	{
    //		void(*p)(T1, T2) = Constraints;
    //	}
    //};
    //

}
