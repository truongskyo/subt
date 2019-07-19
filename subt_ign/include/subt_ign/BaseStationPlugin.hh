/*
 * Copyright (C) 2019 Open Source Robotics Foundation
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

#ifndef SUBT_IGN_BASESTATIONPLUGIN_HH_
#define SUBT_IGN_BASESTATIONPLUGIN_HH_

#include <subt_communication_broker/subt_communication_client.h>

#include <map>
#include <memory>
#include <string>

#include <sdf/Element.hh>

#include <ignition/plugin/Register.hh>
#include <ignition/launch/Plugin.hh>
#include <ignition/transport/Node.hh>

#include "subt_ign/protobuf/artifact.pb.h"

namespace subt
{
  /// \brief A plugin to receive artifact reports from the teams.
  class BaseStationPlugin : public ignition::launch::Plugin
  {
    /// \brief Constructor
    public: BaseStationPlugin();

    /// \brief Destructor
    public: ~BaseStationPlugin();

    // Documentation inherited
    public: virtual bool Load(const tinyxml2::XMLElement *_elem) override final;

    /// \brief Callback for processing an artifact report.
    /// \param[in] _srcAddress Unused.
    /// \param[in] _dstAddress Unused.
    /// \param[in] _dstPort Unused.
    /// \param[in] _data Serialized artifact.
    private: void OnArtifact(const std::string &_srcAddress,
                             const std::string &_dstAddress,
                             const uint32_t _dstPort,
                             const std::string &_data);

    private: void RunLoop();

    /// \brief Thread to send Ack when required.
    private: std::thread ackThread;

    /// \brief Score acknowledgement data to send.
    private: std::map<std::string,
                      std::vector<subt::msgs::ArtifactScore>> scores;

    /// \brief Client address to respond with data.
    private: std::string resAddress;

    /// \brief Flag to terminate ack thread.
    private: std::atomic<bool> running;

    /// \brief Mutex to protect ack data.
    private: std::mutex mutex;

    /// \brief Condition variable to signal ack data ready.
    private: std::condition_variable cv;

    /// \brief An ignition transport node.
    private: ignition::transport::Node node;

    /// \brief SubT communication client.
    private: std::unique_ptr<subt::CommsClient> client;
  };
}

// Register the plugin
IGNITION_ADD_PLUGIN(subt::BaseStationPlugin, ignition::launch::Plugin)
#endif
