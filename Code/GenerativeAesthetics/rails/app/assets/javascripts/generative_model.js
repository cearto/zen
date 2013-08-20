// Generative Model.js
// @author = Cesar Torres
// @date = 18 Aug 2013


function GM(preview, filename){
	this.filename = filename;
	this.preview = preview;
	this.sample = function(){
	   var canvas = $('canvas');
	   var img = convertCanvasToImage(canvas);

	  //html2canvas(document.body, {
      //onrendered: function(canvas) {
        $('#preview').append(img);
      //}
    //});
	  
	}
	this.load = function(filename){
		if(!filename) filename = this.filename;
		else this.filename = filename;
		var jqxhr = $.get(filename, function(data){
			gProcessor = new OpenJsCad.Processor($('#viewer')[0]);
			gProcessor.setJsCad(data, filename);
		});//.done(model.sample);
	}
}

function convertCanvasToImage(canvas) {
	image = new Image();
	image.src = canvas[0].toDataURL("image/jpg");
	
	return image;
}

