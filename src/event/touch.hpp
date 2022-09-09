/*
 * Copyright 2015 Aldebaran
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef TOUCH_EVENT_REGISTER_HPP
#define TOUCH_EVENT_REGISTER_HPP

#include <string>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <qi/session.hpp>

#include <rclcpp/rclcpp.hpp>
#include <naoqi_bridge_msgs/msg/bumper.hpp>
#include <naoqi_bridge_msgs/msg/hand_touch.hpp>
#include <naoqi_bridge_msgs/msg/head_touch.hpp>

#include <naoqi_driver/tools.hpp>
#include <naoqi_driver/recorder/globalrecorder.hpp>

// Converter
#include "../src/converters/touch.hpp"
// Publisher
#include "../src/publishers/basic.hpp"
// Recorder
#include "../recorder/basic_event.hpp"

namespace naoqi
{

/**
* @brief GlobalRecorder concept interface
* @note this defines an private concept struct,
* which each instance has to implement
* @note a type erasure pattern in implemented here to avoid strict inheritance,
* thus each possible publisher instance has to implement the virtual functions mentioned in the concept
*/
template<class T>
class TouchEventRegister: public boost::enable_shared_from_this<TouchEventRegister<T> >
{

public:

  /**
  * @brief Constructor for recorder interface
  */
  TouchEventRegister();
  TouchEventRegister(const std::string& name, const std::vector<std::string> keys, const float& frequency, const qi::SessionPtr& session );
  ~TouchEventRegister();

  void resetPublisher( rclcpp::Node* node );
  void resetRecorder( boost::shared_ptr<naoqi::recorder::GlobalRecorder> gr );

  void startProcess();
  void stopProcess();

  void writeDump(const rclcpp::Time& time);
  void setBufferDuration(float duration);

  void isRecording(bool state);
  void isPublishing(bool state);
  void isDumping(bool state);

  void touchCallback(std::string &key, qi::AnyValue &value, qi::AnyValue &message);
  void touchCallbackMessage(std::string &key, bool &state, naoqi_bridge_msgs::msg::Bumper &msg);
  void touchCallbackMessage(std::string &key, bool &state, naoqi_bridge_msgs::msg::HandTouch &msg);
  void touchCallbackMessage(std::string &key, bool &state, naoqi_bridge_msgs::msg::HeadTouch &msg);
  

private:
  void registerCallback();
  void unregisterCallback();
  void onEvent();

private:
  boost::shared_ptr<converter::TouchEventConverter<T> > converter_;
  boost::shared_ptr<publisher::BasicPublisher<T> > publisher_;
  //boost::shared_ptr<recorder::BasicEventRecorder<T> > recorder_;

  qi::SessionPtr session_;
  qi::AnyObject p_memory_;
  unsigned int serviceId;
  std::string name_;

  boost::mutex mutex_;

  bool isStarted_;
  bool isPublishing_;
  bool isRecording_;
  bool isDumping_;

protected:
  std::vector<std::string> keys_;
}; // class


class BumperEventRegister: public TouchEventRegister<naoqi_bridge_msgs::msg::Bumper>
{
public:
  BumperEventRegister( const std::string& name, const std::vector<std::string> keys, const float& frequency, const qi::SessionPtr& session ) : TouchEventRegister<naoqi_bridge_msgs::msg::Bumper>(name, keys, frequency, session) {}
};

class HeadTouchEventRegister: public TouchEventRegister<naoqi_bridge_msgs::msg::HeadTouch>
{
public:
  HeadTouchEventRegister( const std::string& name, const std::vector<std::string> keys, const float& frequency, const qi::SessionPtr& session ) : TouchEventRegister<naoqi_bridge_msgs::msg::HeadTouch>(name, keys, frequency, session) {}
};

class HandTouchEventRegister: public TouchEventRegister<naoqi_bridge_msgs::msg::HandTouch>
{
public:
  HandTouchEventRegister( const std::string& name, const std::vector<std::string> keys, const float& frequency, const qi::SessionPtr& session ) : TouchEventRegister<naoqi_bridge_msgs::msg::HandTouch>(name, keys, frequency, session) {}
};

//QI_REGISTER_OBJECT(BumperEventRegister, touchCallback)
//QI_REGISTER_OBJECT(HeadTouchEventRegister, touchCallback)

static bool _qiregisterTouchEventRegisterBumper() {
  ::qi::ObjectTypeBuilder<TouchEventRegister<naoqi_bridge_msgs::msg::Bumper> > b;
  QI_VAARGS_APPLY(__QI_REGISTER_ELEMENT, TouchEventRegister<naoqi_bridge_msgs::msg::Bumper>, touchCallback)
    b.registerType();
  return true;
  }
static bool BOOST_PP_CAT(__qi_registration, __LINE__) = _qiregisterTouchEventRegisterBumper();

static bool _qiregisterTouchEventRegisterHandTouch() {
  ::qi::ObjectTypeBuilder<TouchEventRegister<naoqi_bridge_msgs::msg::HandTouch> > b;
  QI_VAARGS_APPLY(__QI_REGISTER_ELEMENT, TouchEventRegister<naoqi_bridge_msgs::msg::HandTouch>, touchCallback)
    b.registerType();
  return true;
  }
static bool BOOST_PP_CAT(__qi_registration, __LINE__) = _qiregisterTouchEventRegisterHandTouch();

static bool _qiregisterTouchEventRegisterHeadTouch() {
  ::qi::ObjectTypeBuilder<TouchEventRegister<naoqi_bridge_msgs::msg::HeadTouch> > b;
  QI_VAARGS_APPLY(__QI_REGISTER_ELEMENT, TouchEventRegister<naoqi_bridge_msgs::msg::HeadTouch>, touchCallback)
    b.registerType();
  return true;
  }
static bool BOOST_PP_CAT(__qi_registration, __LINE__) = _qiregisterTouchEventRegisterHeadTouch();

} //naoqi

#endif
