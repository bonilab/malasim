/* 
 * File:   PersonIndexByLocationMovingLevelHandler.h
 * Author: Merlin
 *
 * Created on August 1, 2013, 9:27 PM
 */

#ifndef PERSONINDEXBYLOCATIONMOVINGLEVELHANDLER_H
#define    PERSONINDEXBYLOCATIONMOVINGLEVELHANDLER_H

#include "Utils/Index/Indexer.h"

class PersonIndexByLocationMovingLevelHandler : public utils::Indexer {
  //disable copy and assign
  PersonIndexByLocationMovingLevelHandler(const PersonIndexByLocationMovingLevelHandler &) = delete;
  void operator=(const PersonIndexByLocationMovingLevelHandler &) = delete;

 public:
  PersonIndexByLocationMovingLevelHandler();

  //    PersonIndexByLocationMovingLevelHandler(const PersonIndexByLocationMovingLevelHandler& orig);
  virtual ~PersonIndexByLocationMovingLevelHandler();

 private:

};

#endif    /* PERSONINDEXBYLOCATIONMOVINGLEVELHANDLER_H */

