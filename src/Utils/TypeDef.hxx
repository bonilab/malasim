#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <array>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <vector>

class Person;

class PersonIndex;

class Event;

class Reporter;

class Drug;

class IStrategy;

class Therapy;

typedef unsigned long ul;

typedef std::vector<double> DoubleVector;
typedef std::vector<DoubleVector> DoubleVector2;
typedef std::vector<DoubleVector2> DoubleVector3;
typedef std::vector<int> IntVector;
typedef std::vector<IntVector> IntVector2;
typedef std::vector<IntVector2> IntVector3;
typedef std::vector<IntVector *> IntVectorPtrVector;
typedef std::vector<IntVector> *IntVector2Ptr;
typedef std::vector<unsigned int> UIntVector;

typedef std::vector<ul> LongVector;
typedef std::vector<LongVector> LongVector2;
typedef std::vector<LongVector2> LongVector3;

typedef std::vector<std::string> StringVector;
typedef std::vector<StringVector> StringVector2;

typedef std::map<int, int> IntIntMap;

typedef std::vector<Person *> PersonPtrVector;
typedef PersonPtrVector::iterator PersonPtrVectorIterator;

typedef std::vector<PersonPtrVector> PersonPtrVector2;
typedef std::vector<PersonPtrVector2> PersonPtrVector3;
typedef std::vector<PersonPtrVector3> PersonPtrVector4;

typedef std::vector<Event *> EventPtrVector;
typedef std::vector<EventPtrVector> EventPtrVector2;

typedef std::vector<Reporter *> ReporterPtrVector;

typedef std::list<PersonIndex *> PersonIndexPtrList;

typedef std::map<int, std::unique_ptr<Drug>> DrugUniquePtrMap;

typedef std::vector<std::unique_ptr<Therapy>> TherapyPtrVector;
typedef std::vector<std::unique_ptr<IStrategy>> StrategyPtrVector;

typedef std::vector<std::unique_ptr<Therapy>> TherapyDatabase;


#endif /* TYPEDEF_H */
