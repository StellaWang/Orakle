
//  Database.h

#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "MultiMap.h"
#include "http.h"
#include "Tokenizer.h"

class Database
{
public:
    enum IndexType {it_none, it_indexed};
    enum OrderingType {ot_ascending, ot_descending};
    
    struct FieldDescriptor
    {
        std::string name;
        IndexType index;
    };
    
    struct SearchCriterion
    {
        std::string fieldName;
        std::string minValue;
        std::string maxValue;
    };
    
    struct SortCriterion
    {
        std::string fieldName;
        OrderingType ordering;
    };
    
    static const int ERROR_RESULT = -1;
    Database();
    ~Database();
    bool specifySchema(const std::vector<FieldDescriptor>& schema);
    bool addRow(const std::vector<std::string>& rowOfData);
    bool loadFromURL(std::string url);
    bool loadFromFile(std::string filename);
   
    int getNumRows() const;
    bool getRow(int rowNum, std::vector<std::string>& row) const;
    int search(const std::vector<SearchCriterion>& searchCriteria,
               const std::vector<SortCriterion>& sortCriteria,
               std::vector<int>& results);
private:
    std::vector<std::vector<std::string>> m_rows;
    std::vector<MultiMap*> m_fieldIndex;
    std::vector<FieldDescriptor> m_schema;
    std::vector<SearchCriterion> m_searchCrit;
    std::vector<int> m_rowMatched;
    std::vector<int> m_rowIntersected;
    
    //To prevent Databases from being copied or assigned, declare these members
    //private and do not implement them.
    Database(const Database& other);
    Database& operator=(const Database& rhs);
};

#endif