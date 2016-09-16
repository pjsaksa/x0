// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2016 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero/RaftServer.h>
#include <xzero/executor/LocalExecutor.h>
#include <xzero/testing.h>
#include <initializer_list>
#include <unordered_map>
#include <vector>

using namespace xzero;

class TestSystem : public RaftServer::StateMachine { // {{{
 public:
  TestSystem(RaftServer::Id id,
             RaftServer::Discovery* discovery,
             Executor* executor);

  void loadSnapshotBegin() override;
  void loadSnapshotChunk(const std::vector<uint8_t>& chunk) override;
  void loadSnapshotEnd() override;
  void applyCommand(const RaftServer::Command& serializedCmd) override;

  int get(int a) {
    if (tuples_.find(a) != tuples_.end())
      return tuples_[a];
    else
      return -1;
  }

 private:
  RaftServer::LocalTransport transport_;
  RaftServer::MemoryStore storage_;
  RaftServer raftServer_;
  std::unordered_map<int, int> tuples_;
};

TestSystem::TestSystem(RaftServer::Id id,
                       RaftServer::Discovery* discovery,
                       Executor* executor)
    : transport_(id),
      storage_(),
      raftServer_(executor, id, &storage_, discovery, &transport_, this),
      tuples_() {
}

void TestSystem::loadSnapshotBegin() {
  tuples_.clear();
}

void TestSystem::loadSnapshotChunk(const std::vector<uint8_t>& chunk) {
  // TODO: make me better
  for (size_t i = 0; i < chunk.size(); i += 2) {
    int a = (int) chunk[i];
    int b = (int) chunk[i + 1];
    tuples_[a] = b;
  }
}

void TestSystem::loadSnapshotEnd() {
  // no-op
}

void TestSystem::applyCommand(const RaftServer::Command& command) {
  int a = static_cast<int>(command[0]);
  int b = static_cast<int>(command[1]);
  tuples_[a] = b;
}
// }}}

TEST(RaftServer, five_node_test) {
  LocalExecutor executor;

  RaftServer::StaticDiscovery sd = {"s1", "s2", "s3", "s4", "s5"};

  TestSystem s1("s1", &sd, &executor);
  TestSystem s2("s2", &sd, &executor);
  TestSystem s3("s3", &sd, &executor);
  TestSystem s4("s4", &sd, &executor);
  TestSystem s5("s5", &sd, &executor);
}

TEST(RaftServer, join) {
}
