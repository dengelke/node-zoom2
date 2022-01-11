#include "errors.h"
#include "query.h"

using namespace v8;

namespace node_zoom {

void check_query_ret(int ret) {
    if (ret == -1) {
        Nan::ThrowError("Query Error");
    }
}

Nan::Persistent<Function> Query::constructor;

void Query::Init(Local<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Query").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "prefix", Prefix);
    Nan::SetPrototypeMethod(tpl, "cql", CQL);
    Nan::SetPrototypeMethod(tpl, "sortBy", SortBy);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(exports, Nan::New("Query").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Query::Query() {
    zquery_ = ZOOM_query_create();
}

Query::~Query() {
    ZOOM_query_destroy(zquery_);
}

NAN_METHOD(Query::New) {
    Nan::HandleScope scope;

    if (info.IsConstructCall()) {
        Query* obj = new Query();
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 0;
        Local<Value> argv[1] = {};
        Local<Function> cons = Nan::New<Function>(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
}

NAN_METHOD(Query::Prefix) {
    Nan::HandleScope scope;
    Query* query = Nan::ObjectWrap::Unwrap<Query>(info.This());
    Nan::Utf8String query_str(info[0]);
    int ret = ZOOM_query_prefix(query->zquery_, *query_str);
    check_query_ret(ret);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Query::CQL) {
    Nan::HandleScope scope;
    Query* query = Nan::ObjectWrap::Unwrap<Query>(info.This());
    Nan::Utf8String query_str(info[0]);
    int ret = ZOOM_query_cql(query->zquery_, *query_str);
    check_query_ret(ret);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Query::SortBy) {
    Nan::HandleScope scope;

    Query* query = Nan::ObjectWrap::Unwrap<Query>(Nan::To<Object>(info[0]).ToLocalChecked());
    int ret;
    Nan::Utf8String strategy(info[0]);
    Nan::Utf8String criteria(info[1]);

    switch (info.Length()) {
        case 1:
            ret = ZOOM_query_sortby(query->zquery_, *strategy);
            check_query_ret(ret);
            break;
        case 2:
            ret = ZOOM_query_sortby2(query->zquery_, *strategy, *criteria);
            check_query_ret(ret);
            break;
        default:
            Nan::ThrowError(ArgsSizeError("SortBy", 2, info.Length()));
            return;
    }

    info.GetReturnValue().Set(info.This());
}

ZOOM_query Query::zoom_query() {
    return zquery_;
}

} // namespace node_zoom
