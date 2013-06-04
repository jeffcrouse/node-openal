#include <node.h>
#include "NodeOpenALSource.h"

using namespace v8;

// --------------------------------------------------------
NodeOpenALSource::NodeOpenALSource(NodeWavData* data) {
    
    if(data->channel==1) {
		if(data->bps==8) {
			format=AL_FORMAT_MONO8;
		} else {
        	format=AL_FORMAT_MONO16;               
		}
    } else {
		if(data->bps==8) {
			format=AL_FORMAT_STEREO8;
		} else {
			format=AL_FORMAT_STEREO16;             
		}      
    }

    alGenBuffers(1, &bufferid);
    alBufferData(bufferid, format, data->data, data->size, data->samplerate);
    alGenSources(1, &sourceid);
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
};

// --------------------------------------------------------
NodeOpenALSource::~NodeOpenALSource() {
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);
};

// --------------------------------------------------------
void NodeOpenALSource::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Source"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("Play"), FunctionTemplate::New(Play)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("SetPosition"), FunctionTemplate::New(SetPosition)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("SetLoop"), FunctionTemplate::New(SetLoop)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Source"), constructor);
}


// --------------------------------------------------------
Handle<Value> NodeOpenALSource::New(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	if ( !args[0]->IsObject() ) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close( Undefined() );
	}

	NodeWavData* data = node::ObjectWrap::Unwrap<NodeWavData>(args[0]->ToObject());

	NodeOpenALSource* source = new NodeOpenALSource(data);
	source->Wrap(args.This());
	return args.This();
}


// --------------------------------------------------------
void NodeOpenALSource::play() {
	alSourcePlay(sourceid);
}

// --------------------------------------------------------
void NodeOpenALSource::setPosition(double x, double y, double z) {
	alSource3f(sourceid, AL_POSITION, x, y, z);
}

// --------------------------------------------------------
void NodeOpenALSource::setLoop(bool loop) {
	if(loop)
		alSourcei(sourceid, AL_LOOPING, AL_TRUE);
	else
		alSourcei(sourceid, AL_LOOPING, AL_FALSE);
}

// --------------------------------------------------------
Handle<Value> NodeOpenALSource::Play(const Arguments& args) {
	HandleScope scope;
	NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(args.This());
	obj->play();
	return scope.Close(v8::Undefined());
}

// --------------------------------------------------------
Handle<Value> NodeOpenALSource::SetPosition(const Arguments& args) {
	HandleScope scope;
	NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(args.This());

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	obj->setPosition(x, y, z);

	return scope.Close(v8::Undefined());
}

// --------------------------------------------------------
Handle<Value> NodeOpenALSource::SetLoop(const Arguments& args) {
	HandleScope scope;
	NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(args.This());
	bool loop = args[0]->BooleanValue();
	obj->setLoop( loop );
	return scope.Close(v8::Undefined());
}


