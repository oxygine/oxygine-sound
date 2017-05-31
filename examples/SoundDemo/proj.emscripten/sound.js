function myb64(bytes)
{
    var binary = '';
    var len = bytes.byteLength;
    for (var i = 0; i < len; i++) {
        binary += String.fromCharCode( bytes[ i ] )
    }
    return window.btoa( binary );    
}


function loopEnded()
{
	this.currentTime = 0;
	this.play();
}

var sound = {
	j2c: null,
	init: function() {
		this.j2c = js2cppX.alloc();
	},
	create: function(){

	},
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
			a.addEventListener('ended', loopEnded, false);
		}

		var obj = {instance:a, looped:loop};

		return this.j2c.create(obj);
	},
	get: function(id){
		return this.j2c.get(id);
	},
	setLoop: function(id, loop){
		var s = sound.get(id);
		if (!s)
			return;
		if (s.looped == loop)
			return;
		s.looped = loop;
		if (loop)
			s.instance.addEventListener('ended', loopEnded, false);
		else
			s.instance.removeEventListener('ended', loopEnded);

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
			s.instance.removeEventListener('ended', loopEnded);
		s.instance.pause(); 
	},
	update: function(id){
		var s = sound.get(id);
		if (!s)
			return true;
		if (s.looped)
			return false;
		var ended = s.instance.ended;
		return ended;
	},	
	free: function(id){
		this.j2c.free(id);
	},
	stats: function(){
		return this.j2c.get_size();
	},
};