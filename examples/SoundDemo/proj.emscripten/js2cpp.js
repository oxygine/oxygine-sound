var js2cpp = {
	_items:{},
	_id:0,
	create:function (data, id) {
		if (typeof(id) === "undefined")
		{
			this._id += 1;
			id = this._id;
		}

		this._items[id] = data;
		return id;
	},
	get:function(id){
		return this._items[id];
	},
	free:function(id){
		delete this._items[id];
	}
};