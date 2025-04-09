
#ifndef PERSONINDEXALLHANDLER_H
#define    PERSONINDEXALLHANDLER_H

#include "Indexer.h"

class PersonIndexAllHandler : public utils::Indexer {
public:
  //disallow copy and assign
  PersonIndexAllHandler(const PersonIndexAllHandler &) = delete;
    void operator=(const PersonIndexAllHandler &) = delete;

 public:
  PersonIndexAllHandler();

  virtual ~PersonIndexAllHandler();

 private:

};

#endif    /* PERSONINDEXALLHANDLER_H */

