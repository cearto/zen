// Generative Model.js
// @author = Cesar Torres
// @date = 18 Aug 2013


function GM(preview, filename){

	this.filename = filename;
	this.preview = preview;
	this.parameters = null;
	this.paramDefinitions;

	this.getParameters = function(){
		var h = {}
		for(var i in this.paramDefinitions)
			h[this.paramDefinitions[i]['name']] = {'range' : this.paramDefinitions[i]['domain'], 'cont': this.paramDefinitions[i]['cont'], 'val' : this.parameters[this.paramDefinitions[i]['name']] }
		return h;
	}

	this.sample = function(){
		var canvas = $('canvas');
		var img = convertCanvasToImage(canvas);
		$('#preview').append(img);	  
		GUI.makeRatingMenu($('#preview'), img, this.featureVector());
	}

	/* Creates a ML friendly feature vector from current params; 
	 * values are mapped to a floating-point representation
	 */
	this.featureVector = function(){
		var fv = []
		var headers = []
		var val;
		var p = this.getParameters();
		for(var i in p){
			headers.push(i);
			//console.log(p[i].val)
			val = mapValue(p[i].val, p[i].range[0], p[i].range[1]);
			fv.push(val)
		}
		return fv;
	}

	this.random = function(){
		var fv = []
		for(var i in this.parameters)
			fv.push(Math.random());

		return fv;
	}
	this.sampleFromSVM = function(){
		$.get('/gm/generate', function(data){

		});

	}

	/* Converts a ML feature vector into a object defined by the model */
	this.generate = function(featureVector){
		var i  = 0;
		var p = this.getParameters();
		for(j in p){
			this.parameters[j] = mapValue(featureVector[i], 0, 1, p[j].cont, p[j].range[0], p[j].range[1])
			i++;
		}
		return this.parameters;
	}

	/* Loads a jscad file into the web canvas defined in aura.js */
	this.load = function(filename){
		if(!filename) filename = this.filename;
		else this.filename = filename;
		var jqxhr = $.get(filename, function(data){
		
		gProcessor = new OpenJsCad.Processor($('#viewer')[0]);
		gProcessor.setJsCad(data, filename);
		
		});
	}

	this.sendToDB = function(){
		samples = $('.sample');
		if(samples.length == 0) return;
		data = []
		$.each(samples, function(i, f){ data.push(GUI.extractData(f))});
		$.ajax({
		  type: "POST",
		  url: '/gm/post_sample',
		  data: {'data' : data},
		  success: function(data){console.log(data)},
		  dataType: 'json'
		});
	
		return data;
	}
	this.updateDB = function(){
		samples = $('.sample');
		if(samples.length == 0) return;
		data = []
		$.each(samples, function(i, f){ data.push(GUI.extractData(f))});

	
	$.ajax({
		  type: "PUT",
		  url: '/gm/update_sample',
		  data: {'data' : data},
		  success: function(data){console.log(data)},
		  dataType: 'json'
	});
}
}


