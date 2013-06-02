#include <node.h>
#include <node_buffer.h>
#include "NodeOpenALMPStream.h"

using namespace v8;
using namespace std;

struct stat statbuf;

// --------------------------------------------------------
void NodeOpenALMPStream::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Stream"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(String::NewSymbol("Ready"), FunctionTemplate::New(Ready)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("Buffer"), FunctionTemplate::New(Buffer)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("SetPosition"), FunctionTemplate::New(SetPosition)->GetFunction());

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Stream"), constructor);
}


// --------------------------------------------------------
Handle<Value> NodeOpenALMPStream::New(const Arguments& args) {
	HandleScope scope;
	NodeOpenALMPStream* stream = new NodeOpenALMPStream();
	stream->Wrap( args.This() );
	return args.This();
}


// --------------------------------------------------------
Handle<Value> NodeOpenALMPStream::Buffer(const Arguments& args) {
	HandleScope scope;
	NodeOpenALMPStream* obj = ObjectWrap::Unwrap<NodeOpenALMPStream>(args.This());

	Local<Value> buffer = args[0];
	size_t size = node::Buffer::Length( buffer->ToObject() );
	char* bufferdata = node::Buffer::Data( buffer->ToObject() );

	obj->buffer(size, bufferdata);
	return scope.Close(v8::Undefined());
}


// --------------------------------------------------------
Handle<Value> NodeOpenALMPStream::Ready(const Arguments& args) {
	HandleScope scope;
	NodeOpenALMPStream* obj = ObjectWrap::Unwrap<NodeOpenALMPStream>(args.This());


	return scope.Close(Boolean::New( obj->ready() ));
}

// --------------------------------------------------------
Handle<Value> NodeOpenALMPStream::SetPosition(const Arguments& args) {
	HandleScope scope;
	NodeOpenALMPStream* obj = ObjectWrap::Unwrap<NodeOpenALMPStream>(args.This());

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	obj->setPosition(x, y, z);

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
NodeOpenALMPStream::NodeOpenALMPStream() {
	/* Generate the buffers and sources */
	alGenBuffers(NUM_BUFFERS, buffers);
	alGenSources(1, &sourceid);

	ALenum error = alGetError();
	if(error != AL_NO_ERROR) {
		cout << "Error generating :( " << ErrorCheck(error) << endl;;
		return;
	}
	n = 0;
	frequency = 44000;
	format = AL_FORMAT_STEREO16;
	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
}

// -----------------------------------------------------
NodeOpenALMPStream::~NodeOpenALMPStream() {
	ALint val;
	/* Although mplayer is done giving us data, OpenAL may still be
	 * playing the remaining buffers. Wait until it stops. */
	do {
		alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
	} while(val == AL_PLAYING);

	alDeleteSources(1, &sourceid);
	alDeleteBuffers(NUM_BUFFERS, buffers);
}

// -----------------------------------------------------
void NodeOpenALMPStream::buffer(size_t size, char* data) {
	// Prefill all of the buffers
	if(n < NUM_BUFFERS-1) {
		alBufferData(buffers[n], format, data, size, frequency);
		ALenum error = alGetError();
		if(error != AL_NO_ERROR) {
			cout << "Error loading :( " << ErrorCheck(error) << endl;;
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

		/* For each processed buffer... */
		while(val--)
		{
			/* Pop the oldest queued buffer from the source, fill it
			 * with the new data, then requeue it */
			alSourceUnqueueBuffers(sourceid, 1, &buffer);
			alBufferData(buffer, format, data, size, frequency);
			alSourceQueueBuffers(sourceid, 1, &buffer);
			if(alGetError() != AL_NO_ERROR) {
			    cout << "Error buffering :(" << endl;
			    break;
			}
		}
		/* Make sure the source is still playing, and restart it if needed. */
		alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
		if(val != AL_PLAYING)
			alSourcePlay(sourceid);
	}
}

// -----------------------------------------------------
bool NodeOpenALMPStream::ready() {
	if(n < NUM_BUFFERS-1) return true;

	ALint val;
	alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);
	return (val >0);
}

// --------------------------------------------------------
void NodeOpenALMPStream::setPosition(double x, double y, double z) {
	alSource3f(sourceid, AL_POSITION, x, y, z);
}

