#include <node.h>
#include <v8.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "NodeWavData.h"
#include "NodeOpenALDevice.h"
#include "NodeOpenALContext.h"
#include "NodeOpenALSource.h"


using namespace v8;
using namespace std;





// --------------------------------------------------
Handle<Value> SetListenerOrientation(const Arguments& args) {
	v8::HandleScope scope;
	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	float o[] = {x, y, z, 0, 1, 0};
	alListenerfv(AL_ORIENTATION, o);
	return scope.Close(v8::Undefined());
}

// --------------------------------------------------
Handle<Value> SetListenerPosition(const Arguments& args) {
	v8::HandleScope scope;
	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	float p[] = {x, y, z, 0, 1, 0};
	alListenerfv(AL_POSITION, p);
	return scope.Close(v8::Undefined());
}

// --------------------------------------------------
Handle<Value> MakeContextCurrent(const Arguments& args) {
	v8::HandleScope scope;

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	if ( !args[0]->IsObject() ) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close( Undefined() );
	}

	NodeOpenALContext* ctx = node::ObjectWrap::Unwrap<NodeOpenALContext>(args[0]->ToObject());
	alcMakeContextCurrent(ctx->context);

	return scope.Close(v8::Undefined());
}

// --------------------------------------------------
void Init(Handle<Object> exports) {
	NodeWavData::Init(exports);
	NodeOpenALContext::Init(exports);
	NodeOpenALDevice::Init(exports);
	NodeOpenALSource::Init(exports);

	exports->Set( String::NewSymbol("MakeContextCurrent"), FunctionTemplate::New(MakeContextCurrent)->GetFunction() );
	exports->Set( String::NewSymbol("SetListenerPosition"), FunctionTemplate::New(SetListenerPosition)->GetFunction() );
	exports->Set( String::NewSymbol("SetListenerOrientation"), FunctionTemplate::New(SetListenerOrientation)->GetFunction() );
}



/**
*	Initialize
*/
NODE_MODULE(openal, Init)