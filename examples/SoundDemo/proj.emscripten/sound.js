function myb64(bytes)
{
    var binary = '';
    var len = bytes.byteLength;
    for (var i = 0; i < len; i++) {
        binary += String.fromCharCode( bytes[ i ] )
    }
    return window.btoa( binary );    
}

var sound = {
	_handles:{},

	play: function(path, volume, loop){
		try{
			var data = FS.readFile(path, {encoding:'binary'});	
		}
		catch(er)
		{
			return 0;
		}
		
		var b64data = myb64(data);
		var src = "data:audio/ogg;base64," + b64data;

		var a = new Audio();
		a.src = src;
		a.play();
		a.volume = volume;

		if (loop)
		{
			a.addEventListener('ended', function() {
			    this.currentTime = 0;
			    this.play();
			}, false);
		}

		return js2cpp.create(a);
	},
	get: function(id){
		return js2cpp.get(id);
	},
	setVolume: function(id, volume){
		var s = sound.get(id);
		if (!s)
			return;
		s.volume = volume;
	},
	pause: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		s.pause();
	},
	resume: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		s.play();
	},
	stop: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		s.pause(); 
	},
	update: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		return s.ended;
	},	
	free: function(id){
		js2cpp.free(id);
	}
};