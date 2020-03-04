#include <nan.h>
#include "query.h"
#include "record.h"
#include "records.h"
#include "options.h"
#include "resultset.h"
#include "connection.h"

using namespace v8;

void InitAll(Local<Object> exports) {
    node_zoom::Query::Init(exports);
    node_zoom::Options::Init(exports);
    node_zoom::Connection::Init(exports);

    node_zoom::Record::Init();
    node_zoom::Records::Init();
    node_zoom::ResultSet::Init();
}

NODE_MODULE(zoom, InitAll);
