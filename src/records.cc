#include "errors.h"
#include "record.h"
#include "records.h"

using namespace v8;

namespace node_zoom {

Persistent<Function> Records::constructor;

void Records::Init() {
    NanScope();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Records"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "next", Next);
    NODE_SET_PROTOTYPE_METHOD(tpl, "hasNext", HasNext);

    NanAssignPersistent(constructor, tpl->GetFunction());
}

Records::~Records() {
    delete[] zrecords_;
}

NAN_METHOD(Records::New) {}

NAN_METHOD(Records::Next) {
    NanScope();
    Records* resset = node::ObjectWrap::Unwrap<Records>(args.This());

    if (resset->index_ >= resset->counts_) {
        NanThrowRangeError("Out of range");
    } else {
        ZOOM_record zrecord = resset->zrecords_[resset->index_++];

        if (zrecord == NULL) {
            NanReturnNull();
        } else {
            Record* record = new Record(ZOOM_record_clone(zrecord));
            Local<Object> wrapper = NanNew(Record::constructor)->NewInstance();
            NanSetInternalFieldPointer(wrapper, 0, record);
            NanReturnValue(wrapper);
        }
    }
}

NAN_METHOD(Records::HasNext) {
    NanScope();
    Records* resset = node::ObjectWrap::Unwrap<Records>(args.This());
    NanReturnValue(NanNew<Boolean>(resset->index_ < resset->counts_));
}

} // namespace node_zoom
