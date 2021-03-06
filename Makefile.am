# This file is part of the "x0" project, http://github.com/christianparpart/x0>
#   (c) 2009-2018 Christian Parpart <christian@parpart.family>
#
# Licensed under the MIT License (the "License"); you may not use this
# file except in compliance with the License. You may obtain a copy of
# the License at: http://opensource.org/licenses/MIT

EXTRA_DIST = autogen.sh
bin_PROGRAMS =
lib_LIBRARIES =
noinst_LIBRARIES =
check_PROGRAMS =
TESTS =

####### compile opts ##########################################################

AM_CXXFLAGS = \
	$(BASE_CXXFLAGS) \
	-DXZERO_CLUSTERDIR="\"$(XZERO_CLUSTERDIR)\"" \
	-I$(top_builddir)/src \
	-I$(top_srcdir)/src

AM_LDFLAGS = \
	$(BASE_LIBS) \
	$(BASE_LDFLAGS) \
	$(PLATFORM_LDFLAGS) \
	$(OPENSSL_LDADD) \
	$(OPENSSL_LIBS) \
	$(ZLIB_LIBS) \
	$(BZLIB_LIBS) \
	-lpthread -ldl

####### libXzeroTesting_main.a ################################################

lib_LIBRARIES += libXzeroTesting_main.a
libXzeroTesting_main_a_SOURCES = src/xzero/testing_main.cc

####### libXzero.a ############################################################

lib_LIBRARIES += libXzero.a

libXzero_a_SOURCES = \
	src/xzero/AnsiColor.cc \
	src/xzero/Application.cc \
	src/xzero/Buffer.cc \
	src/xzero/BufferUtil.cc \
	src/xzero/Counter.cc \
	src/xzero/DeadlineTimer.cc \
	src/xzero/Duration.cc \
	src/xzero/ExceptionHandler.cc \
	src/xzero/Flags.cc \
	src/xzero/HugeBuffer.cc \
	src/xzero/IEEE754.cc \
	src/xzero/JsonWriter.cc \
	src/xzero/MimeTypes.cc \
	src/xzero/MonotonicClock.cc \
	src/xzero/MonotonicTime.cc \
	src/xzero/PosixSignals.cc \
	src/xzero/Random.cc \
	src/xzero/RegExp.cc \
	src/xzero/RuntimeError.cc \
	src/xzero/StackTrace.cc \
	src/xzero/StringUtil.cc \
	src/xzero/TokenShaper.cc \
	src/xzero/Tokenizer.cc \
	src/xzero/UTF8.cc \
	src/xzero/UnixTime.cc \
	src/xzero/Uri.cc \
	src/xzero/WallClock.cc \
	src/xzero/base64.cc \
	src/xzero/base64url.cc \
	src/xzero/executor/Executor.cc \
	src/xzero/executor/LocalExecutor.cc \
	src/xzero/executor/PosixScheduler.cc \
	src/xzero/executor/SafeCall.cc \
	src/xzero/executor/ThreadPool.cc \
	src/xzero/executor/ThreadedExecutor.cc \
	src/xzero/hash/FNV.cc \
	src/xzero/http/BadMessage.cc \
	src/xzero/http/Cookies.cc \
	src/xzero/http/HeaderField.cc \
	src/xzero/http/HeaderFieldList.cc \
	src/xzero/http/HttpChannel.cc \
	src/xzero/http/HttpConnectionFactory.cc \
	src/xzero/http/HttpDateGenerator.cc \
	src/xzero/http/HttpFileHandler.cc \
	src/xzero/http/HttpListener.cc \
	src/xzero/http/HttpMethod.cc \
	src/xzero/http/HttpOutputCompressor.cc \
	src/xzero/http/HttpRangeDef.cc \
	src/xzero/http/HttpRequest.cc \
	src/xzero/http/HttpRequestInfo.cc \
	src/xzero/http/HttpResponse.cc \
	src/xzero/http/HttpService.cc \
	src/xzero/http/HttpStatus.cc \
	src/xzero/http/HttpTransport.cc \
	src/xzero/http/HttpVersion.cc \
	src/xzero/http/MediaRange.cc \
	src/xzero/http/client/Http1Connection.cc \
	src/xzero/http/client/HttpClient.cc \
	src/xzero/http/client/HttpTransport.cc \
	src/xzero/http/fastcgi/Connection.cc \
	src/xzero/http/fastcgi/ConnectionFactory.cc \
	src/xzero/http/fastcgi/Generator.cc \
	src/xzero/http/fastcgi/RequestParser.cc \
	src/xzero/http/fastcgi/ResponseParser.cc \
	src/xzero/http/fastcgi/bits.cc \
	src/xzero/http/hpack/DynamicTable.cc \
	src/xzero/http/hpack/Generator.cc \
	src/xzero/http/hpack/Huffman.cc \
	src/xzero/http/hpack/Parser.cc \
	src/xzero/http/hpack/StaticTable.cc \
	src/xzero/http/http1/Channel.cc \
	src/xzero/http/http1/Connection.cc \
	src/xzero/http/http1/ConnectionFactory.cc \
	src/xzero/http/http1/Generator.cc \
	src/xzero/http/http1/Parser.cc \
	src/xzero/http/http2/Connection.cc \
	src/xzero/http/http2/ConnectionFactory.cc \
	src/xzero/http/http2/ErrorCode.cc \
	src/xzero/http/http2/FrameGenerator.cc \
	src/xzero/http/http2/FrameParser.cc \
	src/xzero/http/http2/FrameType.cc \
	src/xzero/http/http2/SettingParameter.cc \
	src/xzero/http/http2/Settings.cc \
	src/xzero/http/http2/Stream.cc \
	src/xzero/http/mock/Transport.cc \
	src/xzero/inspect.cc \
	src/xzero/io/DataChain.cc \
	src/xzero/io/File.cc \
	src/xzero/io/FileDescriptor.cc \
	src/xzero/io/FileRepository.cc \
	src/xzero/io/FileUtil.cc \
	src/xzero/io/FileView.cc \
	src/xzero/io/Filter.cc \
	src/xzero/io/GzipFilter.cc \
	src/xzero/io/LocalFile.cc \
	src/xzero/io/LocalFileRepository.cc \
	src/xzero/io/MemoryFile.cc \
	src/xzero/io/MemoryFileRepository.cc \
	src/xzero/logging.cc \
	src/xzero/net/Cidr.cc \
	src/xzero/net/DnsClient.cc \
	src/xzero/net/EndPointWriter.cc \
	src/xzero/net/InetAddress.cc \
	src/xzero/net/SslConnector.cc \
	src/xzero/net/SslContext.cc \
	src/xzero/net/SslEndPoint.cc \
	src/xzero/net/TcpConnection.cc \
	src/xzero/net/TcpConnector.cc \
	src/xzero/net/TcpEndPoint.cc \
	src/xzero/net/UdpClient.cc \
	src/xzero/net/UdpConnector.cc \
	src/xzero/net/UdpEndPoint.cc \
	src/xzero/raft/Discovery.cc \
	src/xzero/raft/Error.cc \
	src/xzero/raft/Generator.cc \
	src/xzero/raft/InetTransport.cc \
	src/xzero/raft/LocalTransport.cc \
	src/xzero/raft/Parser.cc \
	src/xzero/raft/Server.cc \
	src/xzero/raft/ServerUtil.cc \
	src/xzero/raft/Storage.cc \
	src/xzero/raft/Transport.cc \
	src/xzero/raft/rpc.cc \
	src/xzero/testing.cc \
	src/xzero/text/IniFile.cc \
	src/xzero/thread/Queue.cc \
	src/xzero/thread/SignalHandler.cc \
	src/xzero/thread/Wakeup.cc \
	src/xzero/util/BinaryMessageReader.cc \
	src/xzero/util/BinaryMessageWriter.cc \
	src/xzero/util/BinaryReader.cc \
	src/xzero/util/BinaryWriter.cc \
	src/xzero/util/SimpleRateLimit.cc

libXzerodir = $(includedir)
nobase_libXzero_HEADERS = \
	src/xzero/AnsiColor.h \
	src/xzero/Api.h \
	src/xzero/Application.h \
	src/xzero/Buffer.h \
	src/xzero/BufferUtil.h \
	src/xzero/Callback.h \
	src/xzero/CompletionHandler.h \
	src/xzero/Counter.h \
	src/xzero/CustomDataMgr.h \
	src/xzero/DeadlineTimer.h \
	src/xzero/Duration.h \
	src/xzero/Duration_impl.h \
	src/xzero/ExceptionHandler.h \
	src/xzero/Flags.h \
	src/xzero/HugeBuffer.h \
	src/xzero/IEEE754.h \
	src/xzero/JsonWriter.h \
	src/xzero/LinkedCounter.h \
	src/xzero/MimeTypes.h \
	src/xzero/MonotonicClock.h \
	src/xzero/MonotonicTime.h \
	src/xzero/MonotonicTime_impl.h \
	src/xzero/PosixSignals.h \
	src/xzero/PrefixTree.h \
	src/xzero/Random.h \
	src/xzero/RegExp.h \
	src/xzero/Result-inl.h \
	src/xzero/Result.h \
	src/xzero/RuntimeError.h \
	src/xzero/StackTrace.h \
	src/xzero/StringUtil-impl.h \
	src/xzero/StringUtil.h \
	src/xzero/SuffixTree.h \
	src/xzero/TokenShaper-inl.h \
	src/xzero/TokenShaper.h \
	src/xzero/Tokenizer.h \
	src/xzero/UTF8.h \
	src/xzero/UnixSignalInfo.h \
	src/xzero/UnixTime.h \
	src/xzero/UnixTime_impl.h \
	src/xzero/Uri.h \
	src/xzero/WallClock.h \
	src/xzero/base64-inl.h \
	src/xzero/base64.h \
	src/xzero/base64url-inl.h \
	src/xzero/base64url.h \
	src/xzero/defines.h \
	src/xzero/executor/EventLoop.h \
	src/xzero/executor/Executor-inl.h \
	src/xzero/executor/Executor.h \
	src/xzero/executor/LocalExecutor.h \
	src/xzero/executor/NativeScheduler.h \
	src/xzero/executor/PosixScheduler.h \
	src/xzero/executor/SafeCall.h \
	src/xzero/executor/ThreadPool.h \
	src/xzero/executor/ThreadedExecutor.h \
	src/xzero/hash/FNV.h \
	src/xzero/http/BadMessage.h \
	src/xzero/http/Cookies.h \
	src/xzero/http/HeaderField.h \
	src/xzero/http/HeaderFieldList.h \
	src/xzero/http/HttpChannel.h \
	src/xzero/http/HttpConnectionFactory.h \
	src/xzero/http/HttpDateGenerator.h \
	src/xzero/http/HttpFileHandler.h \
	src/xzero/http/HttpHandler.h \
	src/xzero/http/HttpInfo.h \
	src/xzero/http/HttpListener.h \
	src/xzero/http/HttpMethod.h \
	src/xzero/http/HttpOutputCompressor.h \
	src/xzero/http/HttpRangeDef-inl.h \
	src/xzero/http/HttpRangeDef.h \
	src/xzero/http/HttpRequest.h \
	src/xzero/http/HttpRequestInfo.h \
	src/xzero/http/HttpResponse.h \
	src/xzero/http/HttpResponseInfo.h \
	src/xzero/http/HttpService.h \
	src/xzero/http/HttpStatus.h \
	src/xzero/http/HttpTransport.h \
	src/xzero/http/HttpVersion.h \
	src/xzero/http/MediaRange.h \
	src/xzero/http/client/Http1Connection.h \
	src/xzero/http/client/HttpClient.h \
	src/xzero/http/client/HttpTransport.h \
	src/xzero/http/cluster/Api.h \
	src/xzero/http/cluster/ApiHandler.cc \
	src/xzero/http/cluster/ApiHandler.h \
	src/xzero/http/cluster/Backend.cc \
	src/xzero/http/cluster/Backend.h \
	src/xzero/http/cluster/Cache.h \
	src/xzero/http/cluster/ClientAbortAction.h \
	src/xzero/http/cluster/Cluster.cc \
	src/xzero/http/cluster/Cluster.h \
	src/xzero/http/cluster/Context.cc \
	src/xzero/http/cluster/Context.h \
	src/xzero/http/cluster/HealthMonitor.cc \
	src/xzero/http/cluster/HealthMonitor.h \
	src/xzero/http/cluster/Scheduler.cc \
	src/xzero/http/cluster/Scheduler.h \
	src/xzero/http/cluster/SchedulerStatus.h \
	src/xzero/http/fastcgi/Connection.h \
	src/xzero/http/fastcgi/ConnectionFactory.h \
	src/xzero/http/fastcgi/Generator.h \
	src/xzero/http/fastcgi/RequestParser.h \
	src/xzero/http/fastcgi/ResponseParser.h \
	src/xzero/http/fastcgi/bits-inl.h \
	src/xzero/http/fastcgi/bits.h \
	src/xzero/http/hpack/DynamicTable.h \
	src/xzero/http/hpack/Generator.h \
	src/xzero/http/hpack/Huffman.h \
	src/xzero/http/hpack/Parser.h \
	src/xzero/http/hpack/StaticTable.h \
	src/xzero/http/hpack/TableEntry.h \
	src/xzero/http/http1/Channel.h \
	src/xzero/http/http1/Client.h \
	src/xzero/http/http1/Connection.h \
	src/xzero/http/http1/ConnectionFactory.h \
	src/xzero/http/http1/Generator.h \
	src/xzero/http/http1/Parser.h \
	src/xzero/http/http2/Connection.h \
	src/xzero/http/http2/ConnectionFactory.h \
	src/xzero/http/http2/DependencyTree.h \
	src/xzero/http/http2/ErrorCode.h \
	src/xzero/http/http2/Flow-inl.h \
	src/xzero/http/http2/Flow.h \
	src/xzero/http/http2/FrameGenerator.h \
	src/xzero/http/http2/FrameListener.h \
	src/xzero/http/http2/FrameParser.h \
	src/xzero/http/http2/FrameType.h \
	src/xzero/http/http2/SettingParameter.h \
	src/xzero/http/http2/Settings.h \
	src/xzero/http/http2/Stream.h \
	src/xzero/http/http2/StreamID.h \
	src/xzero/http/http2/StreamState.h \
	src/xzero/http/mock/Transport.h \
	src/xzero/inspect-impl.h \
	src/xzero/inspect.h \
	src/xzero/io/DataChain.h \
	src/xzero/io/DataChainListener.h \
	src/xzero/io/File.h \
	src/xzero/io/FileDescriptor.h \
	src/xzero/io/FileRepository.h \
	src/xzero/io/FileUtil.h \
	src/xzero/io/FileView.h \
	src/xzero/io/Filter.h \
	src/xzero/io/GzipFilter.h \
	src/xzero/io/LocalFile.h \
	src/xzero/io/LocalFileRepository.h \
	src/xzero/io/MemoryFile.h \
	src/xzero/io/MemoryFileRepository.h \
	src/xzero/logging.h \
	src/xzero/net/Cidr.h \
	src/xzero/net/DnsClient.h \
	src/xzero/net/EndPointWriter.h \
	src/xzero/net/IPAddress.h \
	src/xzero/net/InetAddress-inl.h \
	src/xzero/net/InetAddress.h \
	src/xzero/net/SslConnector.h \
	src/xzero/net/SslContext.h \
	src/xzero/net/SslEndPoint.h \
	src/xzero/net/TcpConnection.h \
	src/xzero/net/TcpConnector.h \
	src/xzero/net/TcpEndPoint.h \
	src/xzero/net/UdpClient.h \
	src/xzero/net/UdpConnector.h \
	src/xzero/net/UdpEndPoint.h \
	src/xzero/raft/Discovery.h \
	src/xzero/raft/Error.h \
	src/xzero/raft/Generator.h \
	src/xzero/raft/Handler.h \
	src/xzero/raft/InetTransport.h \
	src/xzero/raft/Listener.h \
	src/xzero/raft/MessageType.h \
	src/xzero/raft/Parser.h \
	src/xzero/raft/Server-inl.h \
	src/xzero/raft/Server.h \
	src/xzero/raft/ServerUtil.h \
	src/xzero/raft/StateMachine.h \
	src/xzero/raft/Storage.h \
	src/xzero/raft/Transport.h \
	src/xzero/raft/rpc.h \
	src/xzero/testing.h \
	src/xzero/text/IniFile.h \
	src/xzero/thread/Future-impl.h \
	src/xzero/thread/Future.h \
	src/xzero/thread/Queue.h \
	src/xzero/thread/SignalHandler.h \
	src/xzero/thread/Wakeup.h \
	src/xzero/time_constants.h \
	src/xzero/traits.h \
	src/xzero/util/BinaryMessageReader.h \
	src/xzero/util/BinaryMessageWriter.h \
	src/xzero/util/BinaryReader.h \
	src/xzero/util/BinaryWriter.h \
	src/xzero/util/SimpleRateLimit.h \
	src/xzero/util/UnboxedRange.h

# TODO: finalize
if PLATFORM_LINUX
libXzero_a_SOURCES += \
	src/xzero/executor/LinuxScheduler.cc

nobase_libXzero_HEADERS += \
	src/xzero/executor/LinuxScheduler.h
endif

if PLATFORM_DARWIN
libXzero_a_SOURCES += \
	src/xzero/KQueueSignals.cc

nobase_libXzero_HEADERS += \
	src/xzero/KQueueSignals.h
endif

install-data-hook:
	for file in $(DESTDIR)/$(libXzerodir)/src/*; do \
		mv -v $$file $(DESTDIR)/$(libXzerodir); \
	done
	rmdir $(DESTDIR)/$(libXzerodir)/src

####### libXzeroFlow.a ########################################################

# TODO option(ENABLE_FLOW_DIRECT_THREADED_VM "Flow VM using direct threaded mode [default: off]" OFF)

lib_LIBRARIES += libXzeroFlow.a

libXzeroFlow_a_SOURCES = \
	src/flow/AST.cc \
	src/flow/ASTPrinter.cc \
	src/flow/FlowCallVisitor.cc \
	src/flow/FlowLexer.cc \
	src/flow/FlowParser.cc \
	src/flow/FlowToken.cc \
	src/flow/LiteralType.cc \
	src/flow/IRGenerator.cc \
	src/flow/MatchClass.cc \
	src/flow/NativeCallback.cc \
	src/flow/Signature.cc \
	src/flow/SourceLocation.cc \
	src/flow/TargetCodeGenerator.cc \
	src/flow/ir/BasicBlock.cc \
	src/flow/ir/Constant.cc \
	src/flow/ir/ConstantArray.cc \
	src/flow/ir/ConstantValue.cc \
	src/flow/ir/IRBuilder.cc \
	src/flow/ir/IRHandler.cc \
	src/flow/ir/IRProgram.cc \
	src/flow/ir/Instr.cc \
	src/flow/ir/InstructionVisitor.cc \
	src/flow/ir/Instructions.cc \
	src/flow/ir/PassManager.cc \
	src/flow/ir/Value.cc \
	src/flow/transform/EmptyBlockElimination.cc \
	src/flow/transform/InstructionElimination.cc \
	src/flow/transform/MergeBlockPass.cc \
	src/flow/transform/UnusedBlockPass.cc \
	src/flow/vm/ConstantPool.cc \
	src/flow/vm/Handler.cc \
	src/flow/vm/Instruction.cc \
	src/flow/vm/Match.cc \
	src/flow/vm/Program.cc \
	src/flow/vm/Runner.cc \
	src/flow/vm/Runtime.cc

libXzeroFlowdir = $(includedir)
nobase_libXzeroFlow_HEADERS = \
	src/flow/AST.h \
	src/flow/ASTPrinter.h \
	src/flow/ASTVisitor.h \
	src/flow/FlowCallVisitor.h \
	src/flow/FlowLexer.h \
	src/flow/FlowParser.h \
	src/flow/FlowToken.h \
	src/flow/LiteralType.h \
	src/flow/IRGenerator.h \
	src/flow/MatchClass.h \
	src/flow/NativeCallback.h \
	src/flow/Params.h \
	src/flow/Signature.h \
	src/flow/SourceLocation.h \
	src/flow/TargetCodeGenerator.h \
	src/flow/ir/BasicBlock.h \
	src/flow/ir/Constant.h \
	src/flow/ir/ConstantArray.h \
	src/flow/ir/ConstantValue.h \
	src/flow/ir/HandlerPass.h \
	src/flow/ir/IRBuilder.h \
	src/flow/ir/IRBuiltinFunction.h \
	src/flow/ir/IRBuiltinHandler.h \
	src/flow/ir/IRHandler.h \
	src/flow/ir/IRProgram.h \
	src/flow/ir/Instr.h \
	src/flow/ir/InstructionVisitor.h \
	src/flow/ir/Instructions.h \
	src/flow/ir/PassManager.h \
	src/flow/ir/Value.h \
	src/flow/transform/EmptyBlockElimination.h \
	src/flow/transform/InstructionElimination.h \
	src/flow/transform/MergeBlockPass.h \
	src/flow/transform/UnusedBlockPass.h \
	src/flow/vm/ConstantPool.h \
	src/flow/vm/Handler.h \
	src/flow/vm/Instruction.h \
	src/flow/vm/Match.h \
	src/flow/vm/Program.h \
	src/flow/vm/Runner.h \
	src/flow/vm/Runtime.h

####### TESTS #################################################################

TESTS += xzero_test
check_PROGRAMS += xzero_test

xzero_test_SOURCES = \
	src/flow/FlowLexer-test.cc \
	src/flow/FlowParser-test.cc \
	src/flow/vm/Runner-test.cc \
	src/xzero/Base64-test.cc \
	src/xzero/Buffer-test.cc \
	src/xzero/Callback-test.cc \
	src/xzero/Channel-test.cc \
	src/xzero/DeadlineTimer-test.cc \
	src/xzero/Flags-test.cc \
	src/xzero/HugeBuffer-test.cc \
	src/xzero/MimeTypes-test.cc \
	src/xzero/PrefixTree-test.cc \
	src/xzero/Result-test.cc \
	src/xzero/StringUtil-test.cc \
	src/xzero/SuffixTree-test.cc \
	src/xzero/TokenShaper-test.cc \
	src/xzero/Tokenizer-test.cc \
	src/xzero/Uri-test.cc \
	src/xzero/executor/PosixScheduler-test.cc \
	src/xzero/executor/ThreadPool-test.cc \
	src/xzero/hash/FNV-test.cc \
	src/xzero/http/HeaderField-test.cc \
	src/xzero/http/HeaderFieldList-test.cc \
	src/xzero/http/HttpChannel-test.cc \
	src/xzero/http/HttpFileHandler-test.cc \
	src/xzero/http/HttpRangeDef-test.cc \
	src/xzero/http/HttpRequestInfo-test.cc \
	src/xzero/http/MediaRange-test.cc \
	src/xzero/http/client/HttpClient-test.cc \
	src/xzero/http/fastcgi/Generator-test.cc \
	src/xzero/http/fastcgi/RequestParser-test.cc \
	src/xzero/http/fastcgi/ResponseParser-test.cc \
	src/xzero/http/hpack/DynamicTable-test.cc \
	src/xzero/http/hpack/Generator-test.cc \
	src/xzero/http/hpack/Parser-test.cc \
	src/xzero/http/hpack/StaticTable-test.cc \
	src/xzero/http/http1/Connection-test.cc \
	src/xzero/http/http1/Generator-test.cc \
	src/xzero/http/http1/Parser-test.cc \
	src/xzero/http/http2/Connection-test.cc \
	src/xzero/http/http2/Flow-test.cc \
	src/xzero/http/http2/FrameGenerator-test.cc \
	src/xzero/http/http2/FrameParser-test.cc \
	src/xzero/http/http2/Stream-test.cc \
	src/xzero/io/DataChain-test.cc \
	src/xzero/io/MemoryFile-test.cc \
	src/xzero/net/Cidr-test.cc \
	src/xzero/net/TcpConnector-test.cc \
	src/xzero/raft/Generator-test.cc \
	src/xzero/raft/InetTransport-test.cc \
	src/xzero/raft/Parser-test.cc \
	src/xzero/raft/Server-test.cc \
	src/xzero/raft/ServerUtil-test.cc \
	src/xzero/thread/Future-test.cc \
	src/xzero/thread/Queue-test.cc \
	src/xzero/util/BinaryReader-test.cc \
	src/xzero/util/BinaryWriter-test.cc

if PLATFORM_LINUX
xzero_test_SOURCES += \
	src/xzero/executor/LinuxScheduler-test.cc
endif

xzero_test_LDADD = \
	libXzeroTesting_main.a \
	libXzeroFlow.a \
	libXzero.a \
	$(AM_LDFLAGS) \
	$(PAM_LDFLAGS)

####### x0d ###################################################################

bin_PROGRAMS += x0d

x0d_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	-I$(top_srcdir)/src \
	-DX0D_CONFIGFILE="\"$(X0D_CONFIGFILE)\"" \
	-DX0D_LOGDIR="\"$(X0D_LOGDIR)\"" \
	-DX0D_PIDFILE="\"$(X0D_PIDFILE)\""

x0d_LDADD = \
	libXzeroFlow.a \
	libXzero.a \
	$(AM_LDFLAGS) \
	$(PAM_LDFLAGS)

# x0d_LDFLAGS += -static -static-libgcc -static-libstdc++

x0d_SOURCES = \
	$(x0d_types) \
	src/x0d/Context.cc \
	src/x0d/Daemon.cc \
	src/x0d/Module.cc \
	src/x0d/main.cc \
	src/x0d/modules/access.cc \
	src/x0d/modules/accesslog.cc \
	src/x0d/modules/auth.cc \
	src/x0d/modules/compress.cc \
	src/x0d/modules/core.cc \
	src/x0d/modules/dirlisting.cc \
	src/x0d/modules/empty_gif.cc \
	src/x0d/modules/proxy.cc \
	src/x0d/modules/userdir.cc \
	src/x0d/modules/webdav.cc

x0ddir = $(includedir)/x0d
x0d_HEADERS = \
	src/x0d/Context.h \
	src/x0d/Daemon.h \
	src/x0d/Module.h

nodist_x0d_SOURCES = \
	@builddir@/3rdparty/mime.mt.cc

x0d_types = 3rdparty/mime.types
BUILT_SOURCES = $(x0d_types:.types=.mt.cc)
dist_noinst_DATA = $(x0d_types)

%.mt.cc: %.types
	$(abs_top_srcdir)/mimetypes2cc.sh $^ $@ mimetypes2cc

####### man ###################################################################

man_MANS = x0d.8 x0d.conf.5 flow.7

man: x0d.8 x0d.conf.5 flow.7

x0d.8: docs/x0d.8.pod
	pod2man --name=x0d --section 8 --release --utf8 --stderr --utf8 \
		--center "x0d - high performance HTTP web server" \
		$< >$@

x0d.conf.5: docs/x0d.conf.5.pod
	pod2man --name=x0d --section 5 --release --utf8 --stderr --utf8 \
		--center "x0d - high performance HTTP web server configuration" \
		$< >$@

flow.7: docs/flow.7.pod
	pod2man --name=x0d --section 7 --release --utf8 --stderr --utf8 \
		--center "Flow Language Specification" \
		$< >$@

####### dist ##################################################################

dist_sysconf_DATA = x0d.conf

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = XzeroBase.pc XzeroFlow.pc XzeroTest.pc

####### examples ##############################################################

bin_PROGRAMS += hello_http
hello_http_SOURCES = examples/hello_http.cc
hello_http_CXXFLAGS = -I$(top_srcdir)/src $(AM_CXXFLAGS)
hello_http_LDADD = libXzero.a $(AM_LDFLAGS)

####### xurl ##################################################################

if ENABLE_XURL
bin_PROGRAMS += xurl
xurl_CXXFLAGS = \
	-I$(top_srcdir)/src \
	$(AM_CXXFLAGS)

xurl_LDADD = \
	libXzero.a \
	$(AM_LDFLAGS)

xurl_SOURCES = \
	src/xurl/main.cc
endif

# vim:ts=8
# EOF
