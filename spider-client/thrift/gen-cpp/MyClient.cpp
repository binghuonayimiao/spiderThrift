#include "Spider.h"
#include </usr/local/include/thrift/transport/TSocket.h>
#include </usr/local/include/thrift/transport/TBufferTransports.h>
#include </usr/local/include/thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

int main(int argc, char **argv) {
    ::std::shared_ptr<TSocket> socket(new TSocket("localhost", 9090)); 
    ::std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    ::std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
	SpiderDownloadInfo s;
    s.name = "zgs_test1";
    SpiderClient client(protocol);
    client.writeInfo(s);


    transport->close();
    return 0;
}
