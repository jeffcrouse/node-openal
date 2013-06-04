
var openal = require('../openal')

var device = new openal.Device();
var context = new openal.Context( device );

openal.MakeContextCurrent( context );
openal.SetListenerOrientation(1, 0, 0);

var data = new openal.WavData(__dirname+"/../sounds/synth.wav");
var synth = new openal.Source( data );

var x=0, y=0, z=0;
var start = new Date();
setInterval(function(){
	x+=0.05;

	openal.SetListenerPosition(x, y, z);
	synth.Play();
	var seconds = (new Date() - start)/1000;
	console.log(seconds+' secs');
}, 1000);

