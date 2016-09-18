// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2016 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT
#pragma once

#include <xzero/raft/rpc.h>
#include <memory>
#include <unordered_map>

namespace xzero {
namespace raft {

/**
 * Abstracts communication between Server instances.
 */
class Transport {
 public:
  virtual ~Transport() {}

  // leader
  virtual void send(Id target, const VoteRequest& message) = 0;
  virtual void send(Id target, const AppendEntriesRequest& message) = 0;
  virtual void send(Id target, const InstallSnapshotRequest& message) = 0;

  // follower / candidate
  virtual void send(Id target, const AppendEntriesResponse& message) = 0;
  virtual void send(Id target, const VoteResponse& message) = 0;
  virtual void send(Id target, const InstallSnapshotResponse& message) = 0;
};

class Listener;

class InetTransport : public Transport {
 public:
  explicit InetTransport(Id myId, Listener* receiver);
  ~InetTransport();

  void send(Id target, const VoteRequest& message) override;
  void send(Id target, const VoteResponse& message) override;
  void send(Id target, const AppendEntriesRequest& message) override;
  void send(Id target, const AppendEntriesResponse& message) override;
  void send(Id target, const InstallSnapshotRequest& message) override;
  void send(Id target, const InstallSnapshotResponse& message) override;

 private:
  Id myId_;
  Listener* receiver_;
  std::unique_ptr<Connector> connector_;
  std::unordered_map<Id, EndPoint*> endpoints_;
};

class LocalTransport : public Transport {
 public:
  explicit LocalTransport(Id localId, Listener* receiver);

  void send(Id target, const VoteRequest& message) override;
  void send(Id target, const VoteResponse& message) override;
  void send(Id target, const AppendEntriesRequest& message) override;
  void send(Id target, const AppendEntriesResponse& message) override;
  void send(Id target, const InstallSnapshotRequest& message) override;
  void send(Id target, const InstallSnapshotResponse& message) override;

 private:
  Id localId_;
  std::unordered_map<Id, Server*> peers_;
};

} // namespace raft
} // namespace xzero
