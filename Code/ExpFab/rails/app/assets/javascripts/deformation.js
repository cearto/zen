var currentOperation;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////  MESH DEFORMATIONS   /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//geometry and transform information
function translate2d(g, t){
	var xvectors = [];
	var yvectors = [];
	for(var i in g.original){

		var vr = g.original[i].clone();
		vr.sub(g.x_hat);
		
		var vx = vr.clone().dot(t.other_axes[0]);
		var vy = vr.clone().dot(t.other_axes[1]);
		
		xvectors.push(vx);
		yvectors.push(vy);
	}
	var n = Array.max(xvectors) - Array.min(xvectors);
	var m = Array.max(yvectors) - Array.min(yvectors);
	var xmin = Array.min(xvectors);
	var ymin = Array.min(yvectors);
	
	console.log("MINS" , n, m);

	for(var i in g.m){
		var k = g.m[i]; // current index in structure
		var x = xvectors[k] - xmin;
		var y = yvectors[k] - ymin;
		var w = t.axis.clone();
		if(x < 0.01 && y < 0.01)
			console.log(t.scale(x, y, n, m));
		w.multiplyScalar( t.scale(x, y, n, m) );
		g.vp[k].addVectors(g.v[k], w);
	}


}
function balloon(g, t){
	var yvectors = [];

	// yvectors = i X (C - Vr) 
	for(var i in g.original){
		var vr = g.original[i].clone();
		vr.sub(g.x_hat);		
		vr.projectOnVector(t.axis);
		yvectors.push(vr);
	}

	//DEFORM - BALLOON
	var n = g.m.length;

	for(var i in g.m){
		var k = g.m[i];
		var w = g.original[k]
				.clone()
				.sub(yvectors[k]
						.clone()
						.add(g.x_hat)
					);

		w.multiplyScalar( t.scale(i, null,  n, null) );
		g.vp[k].addVectors(g.v[k], w);
	}

}
// V => V'

function rotate(g, t){
	var n  = g.m.length;
	for(var i in g.m){ // for every vertice in region
		var w = new THREE.Matrix4().makeRotationAxis( t.axis, t.scale(i, null, n, null) );	
		g.vp[g.m[i]].applyMatrix4(w);
	}
}

function scale(g, t){
	//DEFORM - SCALE
	var n = g.m.length; 
	for(var i in g.m){ 
		// for every vertex k in region
		var k = g.m[i]; 
		// move in the direction of the axis
		var w = t.axis.clone();
		w.multiplyScalar( t.scale(i, null, n, null) );
		// update vector buffer vp
		g.vp[k].addVectors(g.v[k], w);
	}
}

function normalize(arr){
	var max = Math.max.apply(null, arr);
	for(var i in arr) arr[i] /= max;
	return arr;
}

function selectedMask(v, object){
	var mask = [];
	if(selectedRegions == undefined)
		for(var i in v) mask.push(i);
	else
	$.each(selectedRegions, function(i, reg){ 
		var id  = parseInt($(reg).attr('data-r')); 
		for(var faceIndex in regions[id]){
			var face = object.geometry.faces[regions[id][faceIndex]];
			vertexIndex = face['a']; 
			mask.push(vertexIndex);
			vertexIndex = face['b']; 
			mask.push(vertexIndex);
			vertexIndex = face['c']; 
			mask.push(vertexIndex);
		}
	});
	return mask;
}

function selectedRegions2(expfab){
	var mask = [];
	if(selectedRegions == undefined)
		for(var i in expfab.regions) mask.push(i);
	else
	$.each(selectedRegions, function(i, reg){ 
		var id  = parseInt($(reg).attr('data-r')); 
		mask.push(id);
	});
	return mask;
}

function computeNormal(v1, v2){ return v1.clone().cross(v2).normalize();}



