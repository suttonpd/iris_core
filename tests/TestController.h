 /**
 * \file TestController.h
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012-2013 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * \section LICENSE
 *
 * This file is part of the Iris Project.
 *
 * Iris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Iris is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 * \section DESCRIPTION
 *
 * A basic controller used for testing.
 */

#ifndef TESTCONTROLLER_H_
#define TESTCONTROLLER_H_

#include "irisapi/Controller.h"

namespace iris
{

/** test implementation of a controller */
class TestController : public Controller
{
private:
  std::vector<int> test_;

public:
  TestController() 
    : Controller("testcontroller",  //name
    "This is just for a little bit of testing",  // description
    "Paul Sutton", //author
    "1.0") // version
  {}
  virtual ~TestController() {}
  virtual void initialize(){}
  virtual void process()
  {
    boost::shared_ptr< Event<int> > e;
    getEvent(e);
    test_ = e->data;
  }
  virtual void destroy(){}
  virtual void subscribeToEvents()
  {
    subscribeToEvent("testevent", "testcomponent");
  }
  std::vector<int> getData()
  {
    return test_;
  }
};

} /* namespace iris */

#endif /* TESTCONTROLLER_H_ */
