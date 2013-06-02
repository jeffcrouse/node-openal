#pragma once
#define NUM_BUFFERS 10


#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

using namespace v8;
using namespace std;



// http://kcat.strangesoft.net/openal-tutorial.html
class NodeOpenALMPStream : public node::ObjectWrap {
    public:
        static void Init(v8::Handle<v8::Object> exports);
        void buffer(size_t size, char* bufferdata);
        void setPosition(double x, double y, double z);
        void play();


		/* These are what we'll use for OpenAL playback */
		ALuint sourceid, buffers[NUM_BUFFERS];
		ALuint frequency;
		ALenum format;

		int n;
    private:
        static v8::Handle<v8::Value> New(const v8::Arguments& args);
        static v8::Handle<v8::Value> Buffer(const v8::Arguments& args);
        static v8::Handle<v8::Value> Play(const v8::Arguments& args);
        static v8::Handle<v8::Value> SetPosition(const v8::Arguments& args);

		NodeOpenALMPStream();
    	~NodeOpenALMPStream();
};