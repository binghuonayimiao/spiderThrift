/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef spiderDownloadInfo_TYPES_H
#define spiderDownloadInfo_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>




class SpiderDownloadInfo;

typedef struct _SpiderDownloadInfo__isset {
  _SpiderDownloadInfo__isset() : opt(false) {}
  bool opt :1;
} _SpiderDownloadInfo__isset;

class SpiderDownloadInfo : public virtual ::apache::thrift::TBase {
 public:

  SpiderDownloadInfo(const SpiderDownloadInfo&);
  SpiderDownloadInfo& operator=(const SpiderDownloadInfo&);
  SpiderDownloadInfo() : name(), opt() {
  }

  virtual ~SpiderDownloadInfo() noexcept;
  std::string name;
  std::string opt;

  _SpiderDownloadInfo__isset __isset;

  void __set_name(const std::string& val);

  void __set_opt(const std::string& val);

  bool operator == (const SpiderDownloadInfo & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (__isset.opt != rhs.__isset.opt)
      return false;
    else if (__isset.opt && !(opt == rhs.opt))
      return false;
    return true;
  }
  bool operator != (const SpiderDownloadInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SpiderDownloadInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(SpiderDownloadInfo &a, SpiderDownloadInfo &b);

std::ostream& operator<<(std::ostream& out, const SpiderDownloadInfo& obj);



#endif
