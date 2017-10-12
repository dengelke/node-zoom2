#include "errors.h"
#include "options.h"

using namespace v8;

namespace node_zoom {

Persistent<Function> Options::constructor;

void Options::Init(Handle<Object> exports) {
    NanScope();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Options"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "get", Get);
    NODE_SET_PROTOTYPE_METHOD(tpl, "set", Set);

    NanAssignPersistent(constructor, tpl->GetFunction());
    exports->Set(NanNew("Options"), tpl->GetFunction());
}

Options::Options() {
    zopts_ = ZOOM_options_create();
}

Options::Options(Options *opts) {
    zopts_ = ZOOM_options_create_with_parent(opts->zopts_);
}

Options::~Options() {
    ZOOM_options_destroy(zopts_);
}

ZOOM_options Options::zoom_options() {
    return zopts_;
}

NAN_METHOD(Options::New) {
    NanScope();

    if (args.IsConstructCall()) {
        Options* obj;

        if (args[0]->IsUndefined()) {
            obj = new Options();
        } else {
            Options* opts = node::ObjectWrap::Unwrap<Options>(args[0]->ToObject());
            obj = new Options(opts);
        }

        obj->Wrap(args.This());
        NanReturnValue(args.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = NanNew<Function>(constructor);
        NanReturnValue(cons->NewInstance(argc, argv));
    }
}

NAN_METHOD(Options::Get) {
    NanScope();

    if (args.Length() < 1) {
        NanThrowError(ArgsSizeError("Set", 1, args.Length()));
        return;
    }

    NanUtf8String key(args[0]);
    Options* opts = node::ObjectWrap::Unwrap<Options>(args.This());

    const char *value = ZOOM_options_get(opts->zopts_, *key);

    if (value) {
        NanReturnValue(NanNew(value));
    }
}

NAN_METHOD(Options::Set) {
    NanScope();

    if (args.Length() < 2) {
        NanThrowError(ArgsSizeError("Set", 2, args.Length()));
        return;
    }

    if (!args[0]->IsString()) {
        NanThrowError(ArgTypeError("first", "string"));
        return;
    }

    Options* opts = node::ObjectWrap::Unwrap<Options>(args.This());

    NanUtf8String key(args[0]);
    NanUtf8String value(args[1]);

    ZOOM_options_set(opts->zopts_, *key, *value);

    NanReturnValue(args.This());
}

} // namespace node_zoom
