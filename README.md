Orakle
======

A simple set of C++ classes that can be used to query large amounts of data.

With main.cpp file, test framework test.h, an HTTP class, and a Tokenizer class provided by the professor, I implemented Database class, MultiMap class, and an iterator class called MultiMap::Iterator.

class Database:
1. Able to provide a schema to a database.
2. Able to add one or more data records to a database, either retrieving from a website on the Internet, or adding by users locally.
3. Able to issue a query to the database and obtain a collection of records that match the query’s search criteria, ordered in a manner consistent with the query’s sorting criteria.

class MultiMap:
1. Implemented using a binary search tree.
2. Able to add a new item to a MultiMap
3. Able to search for items in a MultiMap, getting a MultiMap::Iterator indicating a matching item.

class MultiMap::Iterator:
1. Able to ccess the key/value association that is indicated by a MultiMap::Iterator.
2. Able to dvance an iterator forward or backward through its MultiMap.
3. Able to check if an iterator is valid.