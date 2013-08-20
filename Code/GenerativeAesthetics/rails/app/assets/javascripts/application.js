// This is a manifest file that'll be compiled into application.js, which will include all the files
// listed below.
//
// Any JavaScript/Coffee file within this directory, lib/assets/javascripts, vendor/assets/javascripts,
// or vendor/assets/javascripts of plugins, if any, can be referenced here using a relative path.
//
// It's not advisable to add code directly here, but if you do, it'll appear at the bottom of the
// the compiled file.
//
// WARNING: THE FIRST BLANK LINE MARKS THE END OF WHAT'S TO BE PROCESSED, ANY BLANK LINE SHOULD
// GO AFTER THE REQUIRES BELOW.
//
//= require jquery
//= require jquery_ujs
//= require lightgl
//= require csg
//= require openjscad
//= require generative_model
//= require aura
//= require htmltocanvas



// require_tree .
// require bootstrap
// require three.min.js
// require Three
// require stats.min
// require Graphics.js
// require GML.js
//= require GUI.js
//= require JSONLoader.js

Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

function addClickHandler(elem, type, func, ret, self) {
  elem.addEventListener(type, function(e) {
      func(e, self);
  }, ret);
}

function circularJSON(structure){
	var cache = [];
	var cjson = JSON.stringify(structure, function(key, value) {
	    if (typeof value === 'object' && value !== null) {
	        if (cache.indexOf(value) !== -1) {
	            // Circular reference found, discard key
	            //console.log(value);
	            return;
	        }
	        // Store value in our collection
	        cache.push(value);
	    }
	    return value;
	});
	
	cache = null; // Enable garbage collection
	return cjson;
}

/* Testing Script */

function runTest(){
	var saveVoxel = meshTest;
	
	/* CUSTOM STORAGE */
	stored = gml.toJSON(false);
	// unstored = gml.loadJSON(JSON.parse(stored));

	//console.log("STORED GML");
	
	

	// stored = new Object();
	// stored.geometry = saveVoxel.geometry;
	// stored.position = saveVoxel.position;
	
	// a = JSON.stringify(stored);


	//a = circularJSON(saveVoxel);
	// var unstored = JSON.parse(a);

	// console.log("COMPARE");
	
	//unstored.geometry = new THREE.CubeGeometry(unstored.geometry.width, unstored.geometry.height, unstored.geometry.depth );
	// unstored.geometry = $.extend(new THREE.CubeGeometry(), unstored.geometry);

	// console.log( stored.geometry);
	// console.log( new THREE.CubeGeometry(unstored.geometry.width, unstored.geometry.height, unstored.geometry.depth ) );
	// console.log($.extend(new THREE.CubeGeometry(), unstored.geometry));
	// console.log(unstored);
	
	// unstored.geometry = stored.geometry;
	//unstored = stored;
	// return;

    // for ( var i = 0; i < unstored.geometry.faces.length; i ++ )
    //     setHex(unstored.geometry.faces[ i ].color, 0x00A8e1 );
    // /* Apply matte material */
    // var material = new THREE.MeshLambertMaterial( { vertexColors: THREE.FaceColors } );
    // unstored.mesh = new THREE.Mesh( unstored.geometry, material );

    //  Snap to grid  
    // unstored.mesh.position.x = Math.floor( unstored.position.x / 50 ) * 50 + 25;
    // unstored.mesh.position.y = Math.floor( unstored.position.y / 50 ) * 50 + 25;
    // unstored.mesh.position.z = Math.floor( unstored.position.z / 50 ) * 50 + 25;
    // unstored.mesh.matrixAutoUpdate = false;
    // unstored.mesh.updateMatrix();
    
	/* Removal */
	// console.log('removedAll!');
	g.removeAll();
	GUI.removeComponents();
	gml.removeComponents();

	// g.scene.add(unstored.mesh);
	

	// console.log("LOADING GML");
	gml.loadJSON(JSON.parse(stored));
	console.log('Loaded ' + gml.getNumComponents() +" voxels.")
	/* Addition */
	console.log("Rendering GML");
	var comp = gml.getComponents();
	//var comp = [saveVoxel]
	for(var i in comp){
		gml.addComponent(comp[i]);
		GUI.addComponent(comp[i]);
		g.addElement(comp[i]);
	}
	


	return ""//FINISHED TEST"
}
