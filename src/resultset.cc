#include "errors.h"
#include "records.h"
#include "resultset.h"

using namespace v8;

namespace node_zoom {

Persistent<Function> ResultSet::constructor;

void ResultSet::Init() {
    NanScope();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("ResultSet"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "setOption", SetOption);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getOption", GetOption);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getRecords", GetRecords);

    NanAssignPersistent(constructor, tpl->GetFunction());
}

ResultSet::ResultSet(ZOOM_resultset resultset) : zset_(resultset) {}

ResultSet::~ResultSet() {
    ZOOM_resultset_destroy(zset_);
}

NAN_METHOD(ResultSet::New) {}

NAN_METHOD(ResultSet::GetOption) {
    ResultSet* resset = node::ObjectWrap::Unwrap<ResultSet>(args.This());

    if (args.Length() < 1) {
        NanThrowError(ArgsSizeError("Get", 1, args.Length()));
        return;
    }

    NanUtf8String key(args[0]);
    const char *value = ZOOM_resultset_option_get(resset->zset_, *key);

    if (value) {
        NanReturnValue(NanNew(value));
    }
}

NAN_METHOD(ResultSet::SetOption) {
    ResultSet* resset = node::ObjectWrap::Unwrap<ResultSet>(args.This());

    if (args.Length() < 2) {
        NanThrowError(ArgsSizeError("Set", 1, args.Length()));
        return;
    }

    NanUtf8String key(args[0]);
    NanUtf8String value(args[1]);
    ZOOM_resultset_option_set(resset->zset_, *key, *value);

    NanReturnValue(args.This());
}

NAN_METHOD(ResultSet::GetRecords) {
    NanScope();

    if (args.Length() < 2) {
        NanThrowError(ArgsSizeError("Records", 2, args.Length()));
        return;
    }

    ResultSet* resset = node::ObjectWrap::Unwrap<ResultSet>(args.This());
    size_t index = args[0]->Uint32Value();
    size_t counts = args[1]->Uint32Value();

    NanCallback *callback = new NanCallback(args[2].As<Function>());
    GetRecordsWorker *worker = new GetRecordsWorker(
        callback, resset->zset_, index, counts);

    NanAsyncQueueWorker(worker);
}

NAN_METHOD(ResultSet::Size) {
    NanScope();
    ResultSet * resset = node::ObjectWrap::Unwrap<ResultSet>(args.This());
    NanReturnValue(NanNew<Number>(ZOOM_resultset_size(resset->zset_)));
}

void GetRecordsWorker::Execute() {
    zrecords_ = new ZOOM_record[counts_];
    ZOOM_resultset_records(zresultset_, zrecords_, index_, counts_);
}

void GetRecordsWorker::HandleOKCallback() {
    NanScope();

    Records* records = new Records(zrecords_, counts_);
    Local<Object> wrapper = NanNew(Records::constructor)->NewInstance();
    NanSetInternalFieldPointer(wrapper, 0, records);

    Local<Value> argv[] = {
        NanNull(),
        wrapper
    };

    callback->Call(2, argv);
}

} // namespace node_zoom
