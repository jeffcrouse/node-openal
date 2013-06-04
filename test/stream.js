
var openal = require('../openal')
	, spawn = require('child_process').spawn
	, util = require('util')


var device = new openal.Device();
var context = new openal.Context( device );

openal.MakeContextCurrent( context );
openal.SetListenerOrientation(1, 0, 0);


var data = new openal.WavData(__dirname+"/../sounds/synth.wav");
var synth = new openal.Source( data );

/*
var stream = new openal.Stream(2, 16, 44100);
var file = util.format("%s/../sounds/Palisades.mp3", __dirname);
var args = ["-af", "resample=44100:0:1,channels=2,format=s16le",  
			"-nogui", "-novideo", "-noconsolecontrols", "-really-quiet", 
			"-ao", "pcm:nowaveheader:file=/dev/stdout", file];


var mplayer = spawn('mplayer', args);
mplayer.stdout.on('data', function (data) {
	do {} while( !stream.Ready() );
	stream.Buffer( data );
});

mplayer.stderr.on('data', function (data) {
	console.log('stderr: ' + data);
});

mplayer.on('close', function (code) {
	console.log('mplayer process exited with code ' + code);
});
*/

var gain = 1;
var x=0, y=0, z=0;
var start = new Date();
setInterval(function(){
	x+=0.05;
	gain *= 0.8;

	//stream.SetGain(gain);	
	//stream.SetPosition(-x, y, z);
	//console.log( stream.GetPosition() );

	//openal.SetListenerPosition(x, y, z);
	synth.Play();
	var seconds = (new Date() - start)/1000;
	console.log(seconds, 'secs');
}, 1000);

