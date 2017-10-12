#include <uv.h>
#include <sstream>
#include "errors.h"
#include "query.h"
#include "resultset.h"
#include "connection.h"

using namespace v8;

namespace node_zoom {

Persistent<Function> Connection::constructor;

void Connection::Init(Handle<Object> exports) {
    NanScope();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Connection"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "connect", Connect);
    NODE_SET_PROTOTYPE_METHOD(tpl, "destory", Destory);
    NODE_SET_PROTOTYPE_METHOD(tpl, "search", Search);

    NanAssignPersistent(constructor, tpl->GetFunction());
    exports->Set(NanNew("Connection"), tpl->GetFunction());
}

Connection::Connection(Options *opts) {
    zconn_ = ZOOM_connection_create(opts->zoom_options());
}

Connection::~Connection() {
    ZOOM_connection_destroy(zconn_);
}

NAN_METHOD(Connection::New) {
    NanScope();

    if (args.IsConstructCall()) {
        if (args.Length() < 1) {
            NanThrowError(ArgsSizeError("Constructor", 1, args.Length()));
            return;
        }
        Options* opts = node::ObjectWrap::Unwrap<Options>(args[0]->ToObject());
        Connection* obj = new Connection(opts);
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = NanNew<Function>(constructor);
        NanReturnValue(cons->NewInstance(argc, argv));
    }
}

NAN_METHOD(Connection::Connect) {
    NanScope();

    if (args.Length() < 2) {
        NanThrowError(ArgsSizeError("Connect", 2, args.Length()));
        return;
    }

    if (!args[0]->IsString()) {
        NanThrowError(ArgTypeError("first", "string"));
        return;
    }

    if (!args[1]->IsNumber()) {
        NanThrowError(ArgTypeError("second", "number"));
        return;
    }

    if (!args[2]->IsFunction()) {
        NanThrowError(ArgTypeError("third", "function"));
        return;
    }

    Connection* connection = node::ObjectWrap::Unwrap<Connection>(args.This());

    NanUtf8String *host = new NanUtf8String(args[0]);
    int port = args[1]->Uint32Value();
    NanCallback *callback = new NanCallback(args[2].As<Function>());
    ConnectWorker *worker = new ConnectWorker(
        callback, connection->zconn_, host, port);

    NanAsyncQueueWorker(worker);
}

NAN_METHOD(Connection::Destory) {
    NanScope();

    Connection* connection = node::ObjectWrap::Unwrap<Connection>(args.This());
    ZOOM_connection_destroy(connection->zconn_);
}

NAN_METHOD(Connection::Search) {
    NanScope();

    if (args.Length() < 2) {
        NanThrowError(ArgsSizeError("Search", 1, args.Length()));
        return;
    }

    if (!args[0]->IsObject()) {
        NanThrowError(ArgTypeError("first", "object"));
        return;
    }

    if (!args[1]->IsFunction()) {
        NanThrowError(ArgTypeError("second", "function"));
        return;
    }

    Connection* connection = node::ObjectWrap::Unwrap<Connection>(args.This());
    Query* query = node::ObjectWrap::Unwrap<Query>(args[0]->ToObject());
    
    NanCallback *callback = new NanCallback(args[1].As<Function>());
    SearchWorker *worker = new SearchWorker(
        callback, connection->zconn_, query->zoom_query());

    NanAsyncQueueWorker(worker);
}

ConnectWorker::~ConnectWorker() {
    delete host_;
}

void ConnectWorker::Execute() {
    ZOOM_connection_connect(zconn_, **host_, port_);

    int error = 0;
    const char *errmsg, *addinfo;

    if ((error = ZOOM_connection_error(zconn_, &errmsg, &addinfo))) {
        std::ostringstream ss;

        ss << "error: "
            << errmsg
            << "(" << error << ") "
            << addinfo;

        SetErrorMessage(ss.str().c_str());
    }
}

void SearchWorker::Execute() {
    zresultset_ = ZOOM_connection_search(zconn_, zquery_);

    int error = 0;
    const char *errmsg, *addinfo;

    if ((error = ZOOM_connection_error(zconn_, &errmsg, &addinfo))) {
        std::ostringstream ss;

        ss << "error: "
            << errmsg
            << "(" << error << ") "
            << addinfo;

        SetErrorMessage(ss.str().c_str());
    }
}

void SearchWorker::HandleOKCallback() {
    NanScope();

    ResultSet* resultset = new ResultSet(zresultset_);
    Local<Object> wrapper = NanNew(ResultSet::constructor)->NewInstance();
    NanSetInternalFieldPointer(wrapper, 0, resultset);

    Local<Value> argv[] = {
        NanNull(),
        wrapper
    };

    callback->Call(2, argv);
}

} // namespace node_zoom
