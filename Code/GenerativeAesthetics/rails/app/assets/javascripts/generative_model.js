// Generative Model.js
// @author = Cesar Torres
// @date = 18 Aug 2013


function GM(preview, filename){

	this.filename = filename;
	this.preview = preview;
	this.parameters = null;

	this.sample = function(){
		var canvas = $('canvas');
		var img = convertCanvasToImage(canvas);
		$('#preview').append(img);	  
	}
	/* Creates a ML friendly feature vector from current params; 
	 * values are mapped to a floating-point representation
	 */
	this.featureVector = function(){
		var fv = []
		var headers = []
		var val;
		for(var i in this.parameters){
			headers.push(i);
			val = mapValue(this.parameters[i], 0, 10);
			fv.push(val)
		}
		return fv;
	}
	/* Converts a ML feature vector into a object defined by the model */


	/* Loads a jscad file into the web canvas defined in aura.js */
	this.load = function(filename){
		if(!filename) filename = this.filename;
		else this.filename = filename;
		var jqxhr = $.get(filename, function(data){
		
		gProcessor = new OpenJsCad.Processor($('#viewer')[0]);
		gProcessor.setJsCad(data, filename);
		
		});
	}
}


