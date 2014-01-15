
var activeDataStream;
var datastreams = {};

function DataStream(data, name, subject, desc){
	this.data = data;
	this.desc = desc;
	this.subject = subject;
	this.name = name;
	var self = this;
	this.saveState = function(){
		var data = scene.userData.objects[0];
		for(var i in data.object.geometry.vertices)
			data.current[i] = data.object.geometry.vertices[i].clone();
 	}
 	this.restoreState = function(){
 		var data = scene.userData.objects[0];
		for(var i in data.original){
			data.object.geometry.vertices[i] = data.original[i].clone();
			data.current[i] = data.original[i].clone();
		}
		data.object.geometry.verticesNeedUpdate = true;
		render();
 	}

	this.applyToOperation = function(val){
		var data = scene.userData.objects[0];
		if(currentOperation != null){
			var sr = selectedRegions2(data);
			for(var i in sr) data.regions[sr[i]].transform(this.data, val);
		}
		//if(currentOperation != null)
		//	currentOperation.transform(data, null, this.data, val);
	}
	this.container = $('<span></span>').css( { width: '40%', 'display' : 'inline', 'float' : 'left'});


	this.constructGUI = function(subject, desc, isactive, index){
			var item = $('<div name="ds-'+ index +'" class="mesh-item"></div>')
						.click(function(){
							$('.mesh-item').removeClass('mesh-item-unread');
							$(this).addClass('mesh-item-unread');
							activeDataStream = self;
							//activeDataStream.applyToOperation(0.1);
						});
			// var avatar = '<div class="pure-u">\\
			// 		<img class="mesh-avatar" src="'+ avatar +'" height="64" width="64">\\
			// 		</div>';

			var info = $('<div> \
					<p class="mesh-desc">'+ this.name +'</p> \
				</div>').css( { width: '60%', 'display' : 'inline', 'float': 'left'});

			item.append(this.container).append(info).append(clearfix.clone());
			if(isactive) item.removeClass('mesh-item-unread');
			return item;
		}
		datastreams[this.name] = this;
}

DataStream.prototype.add = function(container){
	$(container).append(this.constructGUI(true, 1));
}

DataStream.prototype.load = function() { 
	this.container.sparkline(this.data, {
    	type: 'line'}
    );
};



