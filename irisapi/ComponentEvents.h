/**
 * \file ComponentEvents.h
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
 * Contains classes for the event interface of an Iris Component.
 */

#ifndef IRISAPI_COMPONENTEVENTS_H_
#define IRISAPI_COMPONENTEVENTS_H_

#include "irisapi/Event.h"
#include "irisapi/Exceptions.h"
#include "irisapi/ComponentCallbackInterface.h"

#include <boost/algorithm/string.hpp>

namespace iris
{

/** Represents an event of a component child class. Will be created by
 *  the registerEvent function in the ComponentEvents class.
 */
struct EventDescription
{
  std::string description;
  std::string name;
  int typeId;       ///< Type of data passed with event.

  EventDescription(std::string n="", std::string d="", int t=0) :
    description(d), name(n), typeId(t)
  {}
};

/** An interface to the events of a component
 *
 * The ComponentEvents class permits components to register events of different types. 
 * These events can then be triggered by the component together with relevent data.
 */
class ComponentEvents : boost::noncopyable
{
public:
  ComponentEvents():engine_(NULL){}
  void setEngine(ComponentCallbackInterface* e){engine_ = e;}
  size_t getNumEvents(){return events_.size();}
  std::map<std::string, EventDescription> getEvents(){return events_;}

protected:
  ComponentEvents& assignEvents(const ComponentEvents& other)
  {
    events_ = other.events_;
    engine_ = other.engine_;

    return *this;
  }

  void registerEvent(std::string name, std::string description, int typeId=0)
  {
    if(typeId < 0)
      throw InvalidDataTypeException("Invalid data type specified when registering event " + name);

    boost::to_lower(name);
    EventDescription e(name, description, typeId);
    events_[name] = e;
  }

  template<typename T>
  inline void activateEventInternal(std::string compName, std::string name, T &data);

  template<typename T>
  inline void activateEventInternal(std::string compName, std::string name, std::vector<T> &data);

private:
  std::map<std::string, EventDescription> events_;
  ComponentCallbackInterface* engine_;
};

template<typename T>
inline void ComponentEvents::activateEventInternal(std::string compName, std::string name, T &data)
{
  //Check that we have an interface to the engine
  if(engine_ == NULL)
    return;

  //Check that the event exists and that the datatypes match
  std::map<std::string, EventDescription>::const_iterator it = events_.find(name);
  if (it == events_.end())
    throw EventNotFoundException("Event " + name + " not found");
  
  boost::shared_ptr<EventBase> b(new Event<T>());
  boost::dynamic_pointer_cast< Event<T> >(b)->data.push_back(data);
  b->eventName = name;
  b->componentName = compName;
  b->typeId = TypeInfo<T>::identifier;
  engine_->activateEvent(b);
  return;
}

template<typename T>
inline void ComponentEvents::activateEventInternal(std::string compName, std::string name, std::vector<T> &data)
{
  //Check that we have an interface to the engine
  if(engine_ == NULL)
  {
    return;
  }

  //Check that the event exists and that the datatypes match
  std::map<std::string, EventDescription>::const_iterator it = events_.find(name);
  if (it == events_.end())
    throw EventNotFoundException("Event " + name + " not found");

  boost::shared_ptr<EventBase> b(new Event<T>());
  boost::shared_ptr< Event<T> > e = boost::dynamic_pointer_cast< Event<T> >(b);
  e->data.resize(data.size());
  std::copy(data.begin(), data.end(), e->data.begin());
  b->eventName = name;
  b->componentName = compName;
  b->typeId = TypeInfo<T>::identifier;
  engine_->activateEvent(b);
  return;
}

} // namespace iris

#endif // IRISAPI_COMPONENTEVENTS_H_
