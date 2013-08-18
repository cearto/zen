// Generative Model.js
// @author = Cesar Torres
// @date = 18 Aug 2013


function GM(preview){
	this.preview = preview;
	this.sample = function(){
	  var canvas = $('canvas');
	  var img = convertCanvasToImage(canvas);
	  var gi = $('<img></img>').attr('src', img.src);
	  console.log(gi);
	  this.preview.append(gi);
	}
}

function convertCanvasToImage(canvas) {
	var image = new Image();
	image.src = canvas[0].toDataURL("image/png");
	return image;
}
