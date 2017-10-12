#include "errors.h"
#include "record.h"

using namespace v8;

namespace node_zoom {

Persistent<Function> Record::constructor;

void Record::Init() {
    NanScope();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Record"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "get", Get);

    NanAssignPersistent(constructor, tpl->GetFunction());
}

Record::Record(ZOOM_record record) : zrecord_(record) {}

Record::~Record() {
    ZOOM_record_destroy(zrecord_);
}

NAN_METHOD(Record::New) {}

NAN_METHOD(Record::Get) {
    NanScope();

    Record* record = node::ObjectWrap::Unwrap<Record>(args.This());

    if (args.Length() < 1) {
        NanThrowError(ArgsSizeError("Get", 1, args.Length()));
        return;
    }

    NanUtf8String type(args[0]);
    const char *value = ZOOM_record_get(record->zrecord_, *type, NULL);

    if (value) {
        NanReturnValue(NanNew(value));
    }
}

} // namespace node_zoom
