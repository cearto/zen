function JSONLoader(){}
var numFiles = 0;
JSONLoader.load = function(filename){
	$.getJSON("/helloworld.json", function(data){
		console.log("HEllo");
		console.log(data);
		numFiles = data.length;
	});
}
JSONLoader.save = function(filename){
	
}
JSONLoader.saveFile = function(){
	JSONLoader.updateFileCount(function(data){
	var fn = 'GML ' + numFiles + '.json'
	console.log('SAVING ' + fn + "....");
	$.post('/voxelgml/save_gml', {'filename' : fn , 'gml' : gml.toJSON(true)})
	.done(function(data){
		GUI.alert('Saved!', "The file '"+ fn +"' has been saved to the servers!");
	});
}
)};

JSONLoader.updateFileCount = function(inner){
	files = $.get('/voxelgml/saved_files', function(data){ numFiles = data.length; inner(data); });
}
