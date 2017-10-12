#include "errors.h"
#include "query.h"

using namespace v8;

namespace node_zoom {

void check_query_ret(int ret) {
    if (ret == -1) {
        NanThrowError("Query Error");
    }
}

Persistent<Function> Query::constructor;

void Query::Init(Handle<Object> exports) {
    NanScope();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Query"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "prefix", Prefix);
    NODE_SET_PROTOTYPE_METHOD(tpl, "cql", CQL);
    NODE_SET_PROTOTYPE_METHOD(tpl, "sortBy", SortBy);

    NanAssignPersistent(constructor, tpl->GetFunction());
    exports->Set(NanNew("Query"), tpl->GetFunction());
}

Query::Query() {
    zquery_ = ZOOM_query_create();
}

Query::~Query() {
    ZOOM_query_destroy(zquery_);
}

NAN_METHOD(Query::New) {
    NanScope();

    if (args.IsConstructCall()) {
        Query* obj = new Query();
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    } else {
        const int argc = 0;
        Local<Value> argv[argc] = {};
        Local<Function> cons = NanNew<Function>(constructor);
        NanReturnValue(cons->NewInstance(argc, argv));
    }
}

NAN_METHOD(Query::Prefix) {
    NanScope();
    Query* query = node::ObjectWrap::Unwrap<Query>(args.This());
    NanUtf8String query_str(args[0]);
    int ret = ZOOM_query_prefix(query->zquery_, *query_str);
    check_query_ret(ret);
    NanReturnValue(args.This());
}

NAN_METHOD(Query::CQL) {
    NanScope();
    Query* query = node::ObjectWrap::Unwrap<Query>(args.This());
    NanUtf8String query_str(args[0]);
    int ret = ZOOM_query_cql(query->zquery_, *query_str);
    check_query_ret(ret);
    NanReturnValue(args.This());
}

NAN_METHOD(Query::SortBy) {
    NanScope();

    Query* query = node::ObjectWrap::Unwrap<Query>(args[0]->ToObject());
    int ret;
    NanUtf8String strategy(args[0]);
    NanUtf8String criteria(args[1]);

    switch (args.Length()) {
        case 1:
            ret = ZOOM_query_sortby(query->zquery_, *strategy);
            check_query_ret(ret);
            break;
        case 2:
            ret = ZOOM_query_sortby2(query->zquery_, *strategy, *criteria);
            check_query_ret(ret);
            break;
        default:
            NanThrowError(ArgsSizeError("SortBy", 2, args.Length()));
            return;
    }

    NanReturnValue(args.This());
}

ZOOM_query Query::zoom_query() {
    return zquery_;
}

} // namespace node_zoom
