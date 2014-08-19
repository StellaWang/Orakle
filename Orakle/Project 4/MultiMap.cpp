
//  MultiMap.cpp


#include "MultiMap.h"

MultiMap::Iterator::Iterator()
:m_iterator(NULL)
{
}

MultiMap::Iterator::Iterator(association* cur)
:m_iterator(cur)
{
}
                             
bool MultiMap::Iterator::valid() const
{
    if (m_iterator == NULL)
        return false;
    else
        return true;
}

MultiMap::Iterator::Iterator(const Iterator& other)
{
    m_iterator = other.m_iterator;
}

MultiMap::Iterator& MultiMap::Iterator::operator=(const Iterator& rhs)
{
    if (this != &rhs) {
    m_iterator = rhs.m_iterator;
    }
    return *this;
}

std::string MultiMap::Iterator::getKey() const
{
    if (valid())
        return m_iterator->m_key;
    else
        exit(1);
}

unsigned int MultiMap::Iterator::getValue() const
{
    if (valid())
        return m_iterator->m_value;
    else
        exit(1);
}

bool MultiMap::Iterator::next()
{
    if (!valid())
        return false;
        
    std::string key = m_iterator->m_key;
    if (m_iterator->m_right != NULL)
    {
        m_iterator = m_iterator->m_right;
        while (m_iterator->m_left != NULL)
        {
            m_iterator = m_iterator->m_left;
        }
        return true;
    }
    else                                    //else if (m_iterator->m_right == NULL)
    {
        m_iterator = m_iterator->m_parent;
        while (m_iterator != NULL)
        {
            if (key > m_iterator->m_key)
                m_iterator = m_iterator->m_parent;
            else                            //else if (key <= m_iterator->m_key)
                return true;
        }
        return false;
    }
}

bool MultiMap::Iterator::prev()
{
    if (!valid())
        return false;

    std::string key = m_iterator->m_key;
    if (m_iterator->m_left != NULL)
    {
        m_iterator = m_iterator->m_left;
        while (m_iterator->m_right != NULL)
        {
            m_iterator = m_iterator->m_right;
        }
        return true;
    }
    else                                   //else if (m_iterator->m_left == NULL)
    {
        m_iterator = m_iterator->m_parent;
        while (m_iterator != NULL)
        {
            if (key <= m_iterator->m_key)
                m_iterator = m_iterator->m_parent;
            else                            //else if (key > m_iterator->key)
                return true;
        }
        return false;
    }
}

MultiMap::MultiMap()
{
    m_root = NULL;
}

MultiMap::~MultiMap()
{
    clear();
}

void MultiMap::clear()
{
    PostOrder(m_root);   //call post order traversal to help delete all nodes
    m_root = NULL;
}

void MultiMap::insert(std::string key, unsigned int value)
{
    if (m_root == NULL) {
        m_root = new association(key, value);
        return;
    }
    
    association* cur = m_root;       //ascending by key if traversed in-order
    for (;;)
    {
        if (key <= cur->m_key) {
            if (cur->m_left != NULL)
                cur = cur->m_left;
            else
            {
                cur->m_left = new association(key, value, cur);
                return;
            }
        }
        else if (key > cur->m_key)
        {
            if (cur->m_right != NULL)
                cur = cur->m_right;
            else
            {
                cur->m_right = new association(key, value, cur);
                return;
            }
        }
    }
}

MultiMap::Iterator MultiMap::findEqual(std::string key) const
{
    Iterator it(m_root);
    
    while (it.valid())
    {
        if (it.getKey() == key)
        {
            Iterator next(it);
            it.prev();
            if (!it.valid() || it.getKey() < key)
                return next;
        }
        else if (it.getKey() > key)
            it.prev();
        else //it.getKey() < key
            it.next();
    }
    //else if iterator is invalid
    return it;
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(std::string key) const
{
    Iterator it(m_root);

    while (it.valid())
    {
        if (it.getKey() >= key)
        {
            Iterator next(it);
            it.prev();
            if (!it.valid() || it.getKey() < key)
                return next;
        }
        else //it.getKey()<key
            it.next();
    }

    //else if iterator is invalid
    return it;
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string key) const
{
    Iterator it(m_root);
    
    while (it.valid())
    {
        if (it.getKey() <= key)
        {
            Iterator prev(it);
            it.next();
            if (!it.valid() || it.getKey() > key)
                return prev;
        }
        else //it.getKey()<key
            it.prev();
    }
    //else if iterator is invalid
    return it;
}

void MultiMap::PostOrder(association* cur)
{
    if (cur == NULL)
        return;
    
    PostOrder(cur->m_left);
    PostOrder(cur->m_right);
    delete cur;
};