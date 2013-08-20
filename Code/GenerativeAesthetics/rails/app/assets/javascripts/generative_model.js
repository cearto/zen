// Generative Model.js
// @author = Cesar Torres
// @date = 18 Aug 2013


function GM(preview, filename){
	this.filename = filename;
	this.preview = preview;
	this.sample = function(){
	  var canvas = $('canvas');
	  var img = convertCanvasToImage(canvas);
	  //var gi = $('<img></img>').attr('src', img.src);
	  //console.log(gi);
	  //this.preview.append(gi);
	}
	this.load = function(filename){
		if(!filename) filename = this.filename;
		else this.filename = filename;
		$.get(filename, function(data){
			gProcessor = new OpenJsCad.Processor($('#viewer')[0]);
			gProcessor.setJsCad(data, filename);
			model.sample();
		})
	}
}

function convertCanvasToImage(canvas) {
	image = new Image();
	image.src = canvas[0].toDataURL("image/png");
	$('#preview').append(image);
	return image;
}

