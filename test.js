var openal = require('./build/Release/openal');

var device = new openal.Device();
var context = new openal.Context( device );

openal.MakeContextCurrent( context );

openal.SetListenerOrientation(1, 0, 0);


var data = new openal.WavData(__dirname+"/sounds/synth.wav");


var synth = new openal.Source( data );

var x=0, y=0, z=0;

setInterval(function(){
	x+=0.2;
	openal.SetListenerPosition(x, y, z);
	synth.Play();
}, 1000);

