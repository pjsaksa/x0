// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2018 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero/Application.h>
#include <xzero/io/FileUtil.h>
#include <xzero/io/FileView.h>
#include <xzero/io/File.h>
#include <xzero/RuntimeError.h>
#include <xzero/Buffer.h>
#include <xzero/logging.h>
#include <xzero/sysconfig.h>
#include <xzero/defines.h>
#include <fmt/format.h>

#include <system_error>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <limits.h>
#include <stdlib.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#if defined(XZERO_OS_WINDOWS)
#include <io.h>
//static inline int lseek(int fd, long offset, int origin) { return _lseek(fd, offset, origin); }
static inline int read(int fd, void* buf, unsigned count) { return _read(fd, buf, count); }
#else
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#endif

namespace xzero {

static const char PathSeperator = '/';

char FileUtil::pathSeperator() noexcept {
  return PathSeperator;
}

std::string FileUtil::currentWorkingDirectory() {
  return fs::current_path().string();
}

std::string FileUtil::absolutePath(const std::string& relpath) {
  if (relpath.empty())
    return currentWorkingDirectory();

  if (relpath[0] == PathSeperator)
    return relpath; // absolute already

  return joinPaths(currentWorkingDirectory(), relpath);
}

Result<std::string> FileUtil::realpath(const std::string& relpath) {
  fs::path abspath = fs::absolute(fs::path(relpath));
  return Success(abspath.string());
}

bool FileUtil::exists(const std::string& path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0;
}

bool FileUtil::isDirectory(const std::string& path) {
  return fs::is_directory(path);
}

bool FileUtil::isRegular(const std::string& path) {
  return fs::is_regular_file(path);
}

uintmax_t FileUtil::size(const std::string& path) {
  return fs::file_size(path);
}

uintmax_t FileUtil::sizeRecursive(const std::string& path) {
  uintmax_t totalSize = 0;
  for (auto& dir : fs::recursive_directory_iterator(path))
    if (fs::is_regular_file(dir.path()))
      totalSize += fs::file_size(dir.path());

  return totalSize;
}

void FileUtil::ls(const std::string& path,
                  std::function<bool(const std::string&)> callback) {
  for (const auto& dir : fs::directory_iterator(path)) {
    const fs::path& p = dir.path();
    if (p == ".." || p == ".")
      continue;

    if (!callback(p.string()))
      break;
  }
}

std::string FileUtil::joinPaths(const std::string& base,
                                const std::string& append) {
  if (base.empty())
    return append;

  if (append.empty())
    return base;

  if (base.back() == PathSeperator) {
    if (append.front() == PathSeperator) {
      return base + append.substr(1);
    } else {
      return base + append;
    }
  } else {
    if (append.front() == PathSeperator) {
      return base + append;
    } else {
      return base + '/' + append;
    }
  }
}

FileHandle FileUtil::open(const std::string& path, FileOpenFlags oflags, int mode) {
#if defined(XZERO_OS_WINDOWS)
  DWORD access = 0;
  if (oflags & FileOpenFlags::Read)
    access |= GENERIC_READ;
  if (oflags & FileOpenFlags::Write)
    access |= GENERIC_WRITE;

  DWORD shareMode = FILE_SHARE_READ;

  DWORD disposition = 0;
  if (oflags & FileOpenFlags::CreateNew)
    disposition |= CREATE_NEW;
  else if (oflags & FileOpenFlags::Create)
    disposition |= OPEN_ALWAYS;
  if (oflags & FileOpenFlags::Truncate)
    disposition |= TRUNCATE_EXISTING;

  DWORD flagsAndAttribs = 0;
  flagsAndAttribs = FILE_ATTRIBUTE_NORMAL;

  if (FileOpenFlags::TempFile)
    flagsAndAttribs |= FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE;

  // TODO: any chances we can evaluate `mode` here?

  return FileHandle{ CreateFile(path.c_str(), access, shareMode, nullptr, disposition, flagsAndAttribs, nullptr) };
#else
  if (mode)
    return FileHandle(::open(path.c_str(), to_posix(oflags), mode));
  else
    return FileHandle(::open(path.c_str(), to_posix(oflags)));
#endif
}

void FileUtil::seek(FileHandle& fd, off_t offset) {
#if defined(XZERO_OS_WINDOWS)
  if constexpr(sizeof(offset) > 4) {
    if ((offset >> 32) != 0) {
      LONG low = offset & 0xFFFFFFFF;
      LONG high = (offset >> 32) & 0xFFFFFFFF;
      SetFilePointer(fd.native(), low, &high, FILE_BEGIN);
    } else {
      SetFilePointer(fd.native(), offset, nullptr, FILE_BEGIN);
    }
  } else {
    SetFilePointer(fd.native(), offset, nullptr, FILE_BEGIN);
  }
#else
  off_t rv = ::lseek(fd.native(), offset, SEEK_SET);
  if (rv == (off_t) -1)
    RAISE_ERRNO(errno);
#endif
}

size_t FileUtil::read(FileHandle& fd, Buffer* output) {
  return read(FileView{fd, 0, fd.size()}, output);
}

size_t FileUtil::read(File& file, Buffer* output) {
  FileHandle fd = file.createPosixChannel(FileOpenFlags::Read);
  return read(fd, output);
}

size_t FileUtil::read(const std::string& path, Buffer* output) {
  FileHandle fd{ open(path.c_str(), FileOpenFlags::Read) };
  if (fd.isClosed())
    RAISE_ERRNO(errno);

  return read(fd, output);
}

size_t FileUtil::read(const FileView& file, Buffer* output) {
  output->reserve(file.size() + 1);
  size_t nread = 0;
  size_t count = file.size();

#if !defined(HAVE_PREAD)
  FileUtil::seek(const_cast<FileView&>(file).handle(), file.offset());
#endif

  do {
    ssize_t rv;
#if defined(HAVE_PREAD)
    rv = ::pread(file.handle(), output->data(), file.size() - nread, file.offset() + nread);
#else
    rv = const_cast<FileView&>(file).handle().read(output->data(), count);
#endif
    if (rv < 0) {
      switch (errno) {
        case EINTR:
        case EAGAIN:
          break;
        default:
          RAISE_ERRNO(errno);
      }
    } else if (rv == 0) {
      // end of file reached
      break;
    } else {
      nread += rv;
    }
  } while (nread < file.size());

  output->data()[nread] = '\0';
  output->resize(nread);

  return nread;
}

Buffer FileUtil::read(FileHandle& fd) {
  Buffer output;
  read(FileView{fd, 0, fd.size()}, &output);
  return output;
}

Buffer FileUtil::read(File& file) {
  Buffer output;
  read(file, &output);
  return output;
}

Buffer FileUtil::read(const FileView& file) {
  Buffer output;
  read(file, &output);
  return output;
}

Buffer FileUtil::read(const std::string& path) {
  Buffer output;
  read(path, &output);
  return output;
}

void FileUtil::write(const std::string& path, const BufferRef& buffer) {
  FileHandle fd = open(path, FileOpenFlags::Write | FileOpenFlags::Create | FileOpenFlags::Truncate, 0660);
  if (fd.isClosed())
    RAISE_ERRNO(errno);

  ssize_t nwritten = 0;
  do {
    ssize_t rv = fd.write(buffer.data() + nwritten, buffer.size() - nwritten);
    if (rv < 0) {
      RAISE_ERRNO(errno);
    }
    nwritten += rv;
  } while (static_cast<size_t>(nwritten) < buffer.size());
}

void FileUtil::write(const std::string& path, const std::string& buffer) {
  write(path, BufferRef(buffer.data(), buffer.size()));
}

void FileUtil::write(FileHandle& fd, const char* cstr) {
  FileUtil::write(fd, BufferRef(cstr, strlen(cstr)));
}

void FileUtil::write(FileHandle& fd, const BufferRef& buffer) {
  size_t nwritten = 0;
  do {
    ssize_t rv = fd.write(buffer.data() + nwritten, buffer.size() - nwritten);
    if (rv < 0) {
      switch (errno) {
        case EINTR:
        case EAGAIN:
          break;
        default:
          RAISE_ERRNO(errno);
      }
    } else {
      nwritten += rv;
    }
  } while (nwritten < buffer.size());
}

void FileUtil::write(FileHandle& fd, const std::string& buffer) {
  FileUtil::write(fd, BufferRef(buffer.data(), buffer.size()));
}

void FileUtil::write(FileHandle& fd, const FileView& fileView) {
  write(fd, read(fileView));
}

std::string FileUtil::dirname(const std::string& path) {
  size_t n = path.rfind(PathSeperator);
  return n != std::string::npos
         ? path.substr(0, n)
         : std::string(".");
}

std::string FileUtil::basename(const std::string& path) {
  size_t n = path.rfind(PathSeperator);
  return n != std::string::npos
         ? path.substr(n)
         : path;
}

void FileUtil::mkdir_p(const std::string& path) {
  fs::create_directories(fs::path(path));
}

void FileUtil::rm(const std::string& path) {
  fs::remove(fs::path(path));
}

void FileUtil::chown(const std::string& path,
                     const std::string& user,
                     const std::string& group) {
#if defined(XZERO_OS_UNIX)
  errno = 0;
  struct passwd* pw = getpwnam(user.c_str());
  if (!pw) {
    if (errno != 0) {
      RAISE_ERRNO(errno);
    } else {
      // ("Unknown user name.");
      throw std::invalid_argument{"user"};
    }
  }
  int uid = pw->pw_uid;

  struct group* gr = getgrnam(group.c_str());
  if (!gr) {
    if (errno != 0) {
      RAISE_ERRNO(errno);
    } else {
      throw std::invalid_argument{"group"}; // "Unknown group name.");
    }
  }
  int gid = gr->gr_gid;

  if (::chown(path.c_str(), uid, gid) < 0)
    RAISE_ERRNO(errno);
#else
  // TODO: windows implementation decision
#endif
}

FileHandle FileUtil::createTempFile() {
  return createTempFileAt(tempDirectory());
}

#if defined(ENABLE_O_TMPFILE) && defined(O_TMPFILE)
inline FileHandle createTempFileAt_linux(const std::string& basedir, std::string* result) {
  // XXX not implemented on WSL
  int flags = O_TMPFILE | O_CLOEXEC | O_RDWR;
  int mode = S_IRUSR | S_IWUSR;
  int fd = ::open(basedir.c_str(), flags, mode);

  if (fd < 0)
    RAISE_ERRNO(errno);

  if (result)
    result->clear();

  return FileHandle{fd};
}
#endif

inline FileHandle createTempFileAt_default(const std::string& basedir, std::string* result) {
#if defined(XZERO_OS_WINDOWS)
  TCHAR buf[MAX_PATH];
  if (!GetTempFileName(basedir.c_str(), TEXT("TEMP"), 0 /* unique */, buf))
    return FileHandle{}; // TODO: RAISE_WINDOWS_ERROR();

  FileHandle fd = FileUtil::open(buf, FileOpenFlags::Write | FileOpenFlags::TempFile);
  if (fd.isClosed())
    RAISE_ERRNO(errno);

  if (result)
    *result = std::move(buf);

  return fd;
#elif defined(HAVE_MKOSTEMPS)
  std::string pattern = joinPaths(basedir, "XXXXXXXX.tmp");
  int flags = O_CLOEXEC;
  int fd = mkostemps(const_cast<char*>(pattern.c_str()), 4, flags);

  if (fd < 0)
    RAISE_ERRNO(errno);

  if (result)
    *result = std::move(pattern);
  else
    FileUtil::rm(pattern);

  return FileHandle{fd};
#else
  std::string pattern = FileUtil::joinPaths(basedir, "XXXXXXXX.tmp");
  int fd = mkstemps(const_cast<char*>(pattern.c_str()), 4);

  if (fd < 0)
    RAISE_ERRNO(errno);

  if (result)
    *result = std::move(pattern);
  else
    FileUtil::rm(pattern);

  return FileHandle{fd};
#endif
}

FileHandle FileUtil::createTempFileAt(const std::string& basedir, std::string* result) {
#if defined(ENABLE_O_TMPFILE) && defined(O_TMPFILE)
  static const bool isWSL = Application::isWSL();
  if (!isWSL)
    return createTempFileAt_linux(basedir, result);
  else
    return createTempFileAt_default(basedir, result);
#else
  return createTempFileAt_default(basedir, result);
#endif
}

std::string FileUtil::tempDirectory() {
  return fs::temp_directory_path().string();
}

}  // namespace xzero
