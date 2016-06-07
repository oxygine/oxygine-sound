function myb64(bytes)
{
    var binary = '';
    var len = bytes.byteLength;
    for (var i = 0; i < len; i++) {
        binary += String.fromCharCode( bytes[ i ] )
    }
    return window.btoa( binary );    
}


function looped()
{
	this.currentTime = 0;
	this.play();
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
			a.addEventListener('ended', looped, false);
		}

		var obj = {instance:a, looped:loop};

		return js2cpp.create(obj);
	},
	get: function(id){
		return js2cpp.get(id);
	},
	setVolume: function(id, volume){
		var s = sound.get(id);
		if (!s)
			return;
		s.instance.volume = volume;
	},
	pause: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		s.instance.pause();
	},
	resume: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		s.instance.play();
	},
	stop: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		if (s.looped)
			s.instance.removeEventListener('ended', looped);
		s.instance.pause(); 
	},
	update: function(id){
		var s = sound.get(id);
		if (!s)
			return;
		var ended = s.instance.ended;
		return ended;
	},	
	free: function(id){
		js2cpp.free(id);
	}
};