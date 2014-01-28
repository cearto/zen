
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////  MESH DEFORMATIONS   /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

		w.multiplyScalar( t.scale(i, n) );
		g.vp[k].addVectors(g.v[k], w);
	}

}
// V => V'

function rotate(g, t){
	var n  = g.m.length;
	for(var i in g.m){ // for every vertice in region
		var w = new THREE.Matrix4().makeRotationAxis( t.axis, t.scale(i, n) );	
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
		w.multiplyScalar( t.scale(i, n) );
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



