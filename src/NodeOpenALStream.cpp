#include <node.h>
#include <node_buffer.h>
#include "NodeOpenALStream.h"

using namespace v8;
using namespace std;

// --------------------------------------------------------
void NodeOpenALStream::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Stream"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(String::NewSymbol("Ready"), FunctionTemplate::New(Ready)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("Buffer"), FunctionTemplate::New(Buffer)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("SetPosition"), FunctionTemplate::New(SetPosition)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("GetPosition"), FunctionTemplate::New(GetPosition)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("SetGain"), FunctionTemplate::New(SetGain)->GetFunction());

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Stream"), constructor);
}


// --------------------------------------------------------
Handle<Value> NodeOpenALStream::New(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 3) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	if ( !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close( Undefined() );
	}

	double channels = args[0]->NumberValue();
	double bps = args[1]->NumberValue();
	double frequency = args[2]->NumberValue();

	NodeOpenALStream* stream = new NodeOpenALStream(channels, bps, frequency);
	stream->Wrap( args.This() );
	return args.This();
}


// --------------------------------------------------------
Handle<Value> NodeOpenALStream::Buffer(const Arguments& args) {
	HandleScope scope;
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	Local<Value> buffer = args[0];
	size_t size = node::Buffer::Length( buffer->ToObject() );
	char* bufferdata = node::Buffer::Data( buffer->ToObject() );

	obj->buffer(size, bufferdata);
	return scope.Close(v8::Undefined());
}


// --------------------------------------------------------
Handle<Value> NodeOpenALStream::Ready(const Arguments& args) {
	HandleScope scope;
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());
	return scope.Close(Boolean::New( obj->ready() ));
}

// --------------------------------------------------------
Handle<Value> NodeOpenALStream::SetPosition(const Arguments& args) {
	HandleScope scope;
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());

	if (args.Length() < 3) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	if ( !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close( Undefined() );
	}

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	obj->setPosition(x, y, z);

	return scope.Close(v8::Undefined());
}

// --------------------------------------------------------
Handle<Value> NodeOpenALStream::GetPosition(const Arguments& args) {
	HandleScope scope;
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());
	
	ALfloat x;
	ALfloat y;
	ALfloat z;
	alGetSource3f(obj->sourceid, AL_POSITION, &x, &y, &z);

	Local<Object> position = Object::New();
	position->Set(String::NewSymbol("x"),  Number::New(x));
	position->Set(String::NewSymbol("y"),  Number::New(y));
	position->Set(String::NewSymbol("z"),  Number::New(z));

	return scope.Close(position);
}


// --------------------------------------------------------
Handle<Value> NodeOpenALStream::SetGain(const Arguments& args) {
	HandleScope scope;
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close( Undefined() );
	}

	if ( !args[0]->IsNumber() ) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close( Undefined() );
	}


	double x = args[0]->NumberValue();
	obj->setGain(x);

	return scope.Close(v8::Undefined());
}




string ErrorCheck(ALenum error)
{
    if(error == AL_INVALID_NAME)
    {
        return "Invalid name";
    }
    else if(error == AL_INVALID_ENUM)
    {
        return " Invalid enum ";
    }
    else if(error == AL_INVALID_VALUE)
    {
        return " Invalid value ";
    }
    else if(error == AL_INVALID_OPERATION)
    {
        return " Invalid operation ";
    }
    else if(error == AL_OUT_OF_MEMORY)
    {
        return " Out of memory like! ";
    }
    return " Don't know ";
}



// -----------------------------------------------------
NodeOpenALStream::NodeOpenALStream(int channels, int bps, int _frequency) {
    if(channels==1) {
		if(bps==8) {
			format=AL_FORMAT_MONO8;
		} else {
        	format=AL_FORMAT_MONO16;               
		}
    } else {
		if(bps==8) {
			format=AL_FORMAT_STEREO8;
		} else {
			format=AL_FORMAT_STEREO16;             
		}      
    }

	/* Generate the buffers and sources */
	alGenBuffers(NUM_BUFFERS, buffers);
	alGenSources(1, &sourceid);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR) {
		cout << "Error generating :( " << ErrorCheck(error) << endl;;
		return;
	}

	n = 0;
	frequency = _frequency;
	alSource3f(sourceid, AL_POSITION, 0, 0, 0);


	ALfloat source0Pos[]={ 2.0, 0.0, 2.0};	// Front left of the listener
	ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

	alSourcef(sourceid, AL_PITCH, 1.0f);
	alSourcef(sourceid, AL_GAIN, 1.0f);
	alSourcefv(sourceid, AL_POSITION, source0Pos);
	alSourcefv(sourceid, AL_VELOCITY, source0Vel);
	alSourcei(sourceid, AL_LOOPING, AL_FALSE);
	alSourcef(sourceid, AL_ROLLOFF_FACTOR, 0.5f);

}

// -----------------------------------------------------
NodeOpenALStream::~NodeOpenALStream() {
	ALint val;
	/* Although mplayer is done giving us data, OpenAL may still be
	 * playing the remaining buffers. Wait until it stops. */
	do {
		alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
	} while(val == AL_PLAYING);

	cout << "deleting source and buffers" << endl;
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(NUM_BUFFERS, buffers);
}

// -----------------------------------------------------
void NodeOpenALStream::buffer(size_t size, char* data) {
	
	//cout << "received " << size << " bytes" << endl;
	ALenum error;

	// Prefill all of the buffers
	if(n < NUM_BUFFERS-1) {
		alBufferData(buffers[n], format, data, size, frequency);
		error = alGetError();
		if(error != AL_NO_ERROR) {
			cout << "Error loading :( " << ErrorCheck(error) << endl;
			return;
		}
		n++;

		if(n==NUM_BUFFERS-1) {
			alSourceQueueBuffers(sourceid, NUM_BUFFERS, buffers);
			alSourcePlay(sourceid);
		}
	} else {

		ALuint buffer;
		ALint val;

		/* Check if OpenAL is done with any of the queued buffers */
		alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);
		if(val <= 0)
			return;


		/* Pop the oldest queued buffer from the source, fill it
		 * with the new data, then requeue it */
		alSourceUnqueueBuffers(sourceid, 1, &buffer);
		alBufferData(buffer, format, data, size, frequency);
		alSourceQueueBuffers(sourceid, 1, &buffer);
		error = alGetError();
		if(error != AL_NO_ERROR) {
		    cout << "Error buffering :( " << ErrorCheck(error) << endl;
		    return;
		}

		/* Make sure the source is still playing, and restart it if needed. */
		alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
		if(val != AL_PLAYING)
			alSourcePlay(sourceid);
	}
}

// -----------------------------------------------------
bool NodeOpenALStream::ready() {
	if(n < NUM_BUFFERS-1) return true;
	ALint val;
	alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);
	return (val > 0);
}

// -----------------------------------------------------
void NodeOpenALStream::setGain(float g) {
	alSourcef(sourceid, AL_GAIN, g);
}

// --------------------------------------------------------
void NodeOpenALStream::setPosition(double x, double y, double z) {
	cout << "SETTING: x=" << x << " y=" << y << " z=" << z << endl;
	alSource3f(sourceid, AL_POSITION, x, y, z);
}

