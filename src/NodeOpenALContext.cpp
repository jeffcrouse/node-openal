#include <node.h>
#include "NodeOpenALContext.h"

using namespace v8;

//vector<NodeOpenALContext*> NodeOpenALContext::contexts;


NodeOpenALContext::NodeOpenALContext(NodeOpenALDevice* dev) {
	context = alcCreateContext(dev->device, NULL);
    if(context==NULL) {
		std::cout << "cannot open context" << std::endl;
		return;
    }
};


NodeOpenALContext::~NodeOpenALContext() {
	if(context) {
		cout << "destroying context" << endl;
		alcDestroyContext(context);
	}
};

void NodeOpenALContext::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Context"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Context"), constructor);
}

Handle<Value> NodeOpenALContext::New(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	if ( !args[0]->IsObject() ) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close( Undefined() );
	}

	NodeOpenALDevice* dev = node::ObjectWrap::Unwrap<NodeOpenALDevice>(args[0]->ToObject());
	
	NodeOpenALContext* ctx = new NodeOpenALContext( dev );
	//contexts.push_back( ctx );

	ctx->Wrap(args.This());

	return args.This();
}

