node-openal
===========

This addon makes it possible to use the OpenAL library from within a node application.  

### Helpful Sources
I didn't know anything about OpenAL or creating c++ node "addons" before starting this, so I got a lot of help from the following sources:

* [node-midi](https://github.com/justinlatimer/node-midi) provided a great example of a nicely designed c++-based node module
* [nodejs.org addon manual](http://nodejs.org/api/addons.html)
* [This simple OpenAL example](http://pastebin.com/rYp59nYg)
* [OpenAL Programmers Guide](http://connect.creativelabs.com/openal/Documentation/OpenAL_Programmers_Guide.pdf)
* [My OpenAL framework is broken, how can I fix it? (OSX Lion)](http://stackoverflow.com/questions/10536001/my-openal-framework-is-broken-how-can-i-fix-it-osx-lion)

### Testing
As a newcomer to node addons, rather than setting up the whole Eclipse-based debugging environment, I found it helpful to compile my addon in debug mode and just run node through gdb:

	node-gyp clean && node-gyp configure && node-gyp build --debug
	gdb node
	(gdb) set args test/stream.js
	(gdb) run

Even though node isn't compiled with debug symbols, this got me close enough to the problem when I was getting weird segfaults.

### To do
* Use [libuv](https://github.com/joyent/libuv) for stream reading?
* [OpenAL-soft](http://kcat.strangesoft.net/openal.html) air absorption, occlusion, and environmental reverb
* same as [EFX](http://connect.creativelabs.com/developer/Wiki/Introduction%20to%20EFX.aspx)?
* [use MPlayer for compressed formats?](http://kcat.strangesoft.net/openal-tutorial.html) or [ogg](http://devmaster.net/posts/2895/openal-lesson-8-oggvorbis-streaming-using-the-source-queue)
* ultimately, I want to be able to stream the audio that is generated.  Is [this article](http://pedromtavares.wordpress.com/2012/12/28/streaming-audio-on-the-web-with-nodejs/) relavant?