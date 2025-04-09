/* 
 * File:   PersonIndexByLocationStateAgeClassHandler.h
 * Author: nguyentran
 *
 * Created on May 2, 2013, 10:57 AM
 */

#ifndef PERSONINDEXBYLOCATIONSTATEAGECLASSHANDLER_H
#define    PERSONINDEXBYLOCATIONSTATEAGECLASSHANDLER_H

#include "Indexer.h"

class PersonIndexByLocationStateAgeClassHandler : public utils::Indexer {
  //disable copy and assign
  PersonIndexByLocationStateAgeClassHandler(const PersonIndexByLocationStateAgeClassHandler &) = delete;
  void operator=(const PersonIndexByLocationStateAgeClassHandler &) = delete;

 public:
  PersonIndexByLocationStateAgeClassHandler();

//    PersonIndexByLocationStateAgeClassHandler(const PersonIndexByLocationStateAgeClassHandler& orig);
  virtual ~PersonIndexByLocationStateAgeClassHandler();

 private:

};

#endif    /* PERSONINDEXBYLOCATIONSTATEAGECLASSHANDLER_H */

