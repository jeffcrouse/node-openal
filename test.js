
var openal = require('./build/Debug/openal')
	, spawn = require('child_process').spawn
	, util = require('util')


var device = new openal.Device();
var context = new openal.Context( device );

openal.MakeContextCurrent( context );
openal.SetListenerOrientation(1, 0, 0);


var data = new openal.WavData(__dirname+"/sounds/synth.wav");
var synth = new openal.Source( data );


var stream = new openal.Stream();

var file = util.format("%s/sounds/Palisades.mp3", __dirname);
var args = ["-af", "resample=44100:0:1,channels=2,format=s16le",  
			"-nogui", "-novideo", "-noconsolecontrols", "-really-quiet", 
			"-ao", "pcm:nowaveheader:file=/dev/stdout", file];
console.log( args.join(" ") );

var mplayer = spawn('mplayer', args);

mplayer.stdout.on('data', function (data) {
	//console.log("buffering data");
	stream.Buffer( data );
});

mplayer.stderr.on('data', function (data) {
	console.log('stderr: ' + data);
});

mplayer.on('close', function (code) {
	console.log('mplayer process exited with code ' + code);
});


var x=0, y=0, z=0;

setInterval(function(){
	x+=0.2;
	openal.SetListenerPosition(x, y, z);
	synth.Play();
}, 1000);

