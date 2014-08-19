
//  Database.cpp

#include "Database.h"
using namespace std;

Database::Database()
{
    
}

Database::~Database()
{
    for (int i = 0; i < m_fieldIndex.size(); i++)
    {
        delete m_fieldIndex[i];
        m_fieldIndex.erase(m_fieldIndex.begin() + i);
        i--;
    }
    
    m_rows.clear();
    m_fieldIndex.clear();
    m_schema.clear();
}

bool Database::specifySchema(const std::vector<FieldDescriptor>& schema)
{
    for (int i = 0; i < m_fieldIndex.size(); i++)
    {
        delete m_fieldIndex[i];
        m_fieldIndex.erase(m_fieldIndex.begin() + i);
        i--;
    }
    
    m_rows.clear();
    m_fieldIndex.clear();
    m_schema.clear();
    
    for (int i = 0; i < schema.size(); i++)
    {
        FieldDescriptor newSchema;
        newSchema.name = schema[i].name;
        newSchema.index = schema[i].index;
        m_schema.push_back(newSchema);
        
        MultiMap* Index = new MultiMap;
        m_fieldIndex.push_back(Index);
    }
    
    for (int i = 0; i < schema.size(); i++)   //check if there are no indexed field
    {
        if (schema[i].index == it_indexed)
            return true;
    }

    m_schema.clear();
    return false;
}

bool Database::addRow(const vector<string>& rowOfData)
{
    if (rowOfData.size() != m_schema.size())
        return false;

    m_rows.push_back(rowOfData);
    unsigned int position = m_rows.size()-1;
    for (int i = 0; i<rowOfData.size(); i++)
    {
        if (m_schema[i].index == it_indexed)
        {
            m_fieldIndex[i]->insert(rowOfData[i], position);  //insert the data into specific index field
        }
    }
    return true;
}

bool Database::loadFromURL(string url)
{
    string page;
    if(!HTTP().get(url, page))
        return false;
    
    istringstream iss(page);
    string line;
    vector<FieldDescriptor> schema;
    vector<vector<string>> alldata;
    
    int countline = 0;
    while (getline(iss, line))
    {
        Tokenizer t(line, ",");
        string w;
        if (countline == 0)
        {
            while (t.getNextToken(w))
            {
                FieldDescriptor newFD;
                int last = w.size();
                if (w[last-1] == '*')       //if indexed, give rid of the last '*' character
                {
                    newFD.name = w.substr(0, last-1);
                    newFD.index = it_indexed;
                }
                else
                {
                    newFD.name = w;
                    newFD.index = it_none;
                }
                schema.push_back(newFD);
            }
        }
        else
        {
            vector<string> record;
            while (t.getNextToken(w))
            {
                record.push_back(w);
            }
            alldata.push_back(record);
        }
        countline++;
    }
    
    if (!specifySchema(schema))     //if cannot install a schema, return false
        return false;
    
    for (int i = 0; i < alldata.size(); i++)
    {
        if (!addRow(alldata[i]))    //if cannot add a row, return false
            return false;
    }
    return true;
}

bool Database::loadFromFile(string filename)
{
    ifstream infile(filename);
    if (!infile)
        return false;

    vector<FieldDescriptor> schema;
    vector<vector<string>> alldata;
    
    string line;
    int countline = 0;
    while (getline(infile, line))
    {
        Tokenizer t(line, ",");
        string w;
        if (countline == 0)
        {
            while (t.getNextToken(w))
            {
                FieldDescriptor newFD;
                int last = w.size();
                if (w[last-1] == '*')
                {
                    newFD.name = w.substr(0, last-1);
                    newFD.index = it_indexed;
                }
                else
                {
                    newFD.name = w;
                    newFD.index = it_none;
                }
                schema.push_back(newFD);
            }
        }
        else
        {
            vector<string> record;
            while (t.getNextToken(w))
            {
                record.push_back(w);
            }
            alldata.push_back(record);
        }
        countline++;
    }
    
    if (!specifySchema(schema))
        return false;
    
    for (int i = 0; i < alldata.size(); i++)
    {
        if (!addRow(alldata[i]))
            return false;
    }
    return true;
}

int Database::getNumRows() const
{
    return m_rows.size();
}

bool Database::getRow(int rowNum, std::vector<std::string>& row) const
{
    int rowsize = m_rows.size();
    if (rowNum > rowsize-1)
        return false;
    vector<string> temp = m_rows[rowNum];
    for (int i = 0; i < temp.size(); i++)
    {
        row.push_back(temp[i]);
    }
    return true;
}

int Database::search(const std::vector<SearchCriterion>& searchCriteria,
                     const std::vector<SortCriterion>& sortCriteria,
                     std::vector<int>& results)
{
    m_rowIntersected.clear();
    m_searchCrit.clear();
    results.clear();
    for (size_t i = 0; i < searchCriteria.size(); i++)   //install the searchCriteria into the database
    {
        SearchCriterion s;
        s.fieldName = searchCriteria[i].fieldName;
        s.minValue = searchCriteria[i].minValue;
        s.maxValue = searchCriteria[i].maxValue;
        m_searchCrit.push_back(s);
    }
    if (m_searchCrit.empty()) {
        return ERROR_RESULT;
    }
    
    //testingtesting
    for (size_t i = 0; i < m_searchCrit.size(); i++)
    {
        cout << m_searchCrit[i].fieldName << endl;
        cout << m_searchCrit[i].minValue << endl;
        cout << m_searchCrit[i].maxValue << endl;
        cout << "+++++++++" << endl;
    }
    
    for (size_t i = 0; i < m_searchCrit.size(); i++)
    {
        m_rowMatched.clear();
        int j = 0;
        while (j< m_schema.size())
        {
            if (m_schema[j].name == m_searchCrit[i].fieldName)
                break;
            j++;
        }
        if (j == m_schema.size()) {
            return ERROR_RESULT;
        }
        if (m_searchCrit[i].minValue == "" && m_searchCrit[i].maxValue == "") {
            return ERROR_RESULT;
        }
        
        for (size_t k = 0; k < m_rows.size() ; k++)
        {
            vector<string> theRow = m_rows[k];
            if (theRow[j] >= m_searchCrit[i].minValue && theRow[j] <= m_searchCrit[i].maxValue)
            {
                m_rowMatched.push_back(k);
            }
        }
        
        if (i == 0){
            m_rowIntersected = m_rowMatched;
        }
        else
        {
        vector<int> temp = m_rowIntersected;
        m_rowIntersected.clear();
        while (!m_rowMatched.empty())           //this part below is to help get the intersected row #s
        {
            int row1 = m_rowMatched.back();
            m_rowMatched.pop_back();
            vector<int>::iterator it = temp.begin();
            while (it != temp.end())
            {
                if ((*it) == row1)
                {
                    m_rowIntersected.push_back(row1);
                    break;
                }
                it++;
            }
        }
        }
    }
    
    results = m_rowIntersected;
    
    return m_rowIntersected.size();
}






