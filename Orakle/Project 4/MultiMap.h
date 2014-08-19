#ifndef MULTIMAP_INCLUDED
#define MULTIMAP_INCLUDED

#include <iostream>
#include <string>


class MultiMap
{
private:
    //To prevent MultiMaps from being copied or assigned, declare these members
    //private and do not implement them.
    MultiMap(const MultiMap& other);
    MultiMap&operator=(const MultiMap& rhs);
    class association
    {
    public:
        association(std::string key, unsigned int value, association* parent = NULL)
        :m_key(key),m_value(value), m_parent(parent)
        {
            m_left = NULL;
            m_right = NULL;
        }
        std::string m_key;
        unsigned int m_value;
        association* m_left;
        association* m_right;
        association* m_parent;
    };
    association* m_root;
    void PostOrder(association* cur);

public:
    //You must implement this public nested MultiMap::Iterator class
    class Iterator
    {
    public:
        Iterator(); //You must have a default constructor
        Iterator(association* cur);
        Iterator(const Iterator& other);
        Iterator& operator=(const Iterator& rhs);
        bool valid() const;
        std::string getKey() const;
        unsigned int getValue() const;
        bool next();
        bool prev();
    private:
        association* m_iterator;
    };
    MultiMap();
    ~MultiMap();
    void clear();
    void insert(std::string key, unsigned int value);
    Iterator findEqual(std::string key) const;
    Iterator findEqualOrSuccessor(std::string key) const;
    Iterator findEqualOrPredecessor(std::string key) const;
};

#endif