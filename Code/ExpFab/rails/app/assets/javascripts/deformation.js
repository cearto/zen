
weather = [];

/* A mapping takes a range and maps it to datastream */
function Mapping(){};

Mapping.dsp = function(i,n, ds){
	var param = parseFloat(i)/n;
	return Mapping.linInterp(param, ds);
}
/* param is from 0 to 1 */
Mapping.linInterp = function(param, data){
	var n = data.length - 1;
	var index = param * n;
	var interp = index - parseInt(index);
	index = parseInt(index);
	return data[index] * (1-interp) +  data[index + 1] * (interp);
}

// TODO: Add noise functions



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////  MESH DEFORMATIONS   /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function balloon(t){
	var yvectors = [];

	// yvectors = i X (C - Vr) 
	for(var i in t.v){
		var vr = t.v[i].clone();
		vr.sub(t.x_hat);		
		vr.projectOnVector(t.axis);
		yvectors.push(vr);
	}

	//DEFORM - BALLOON
	var n = t.m.length;

	for(var i in t.m){
		var k = t.m[i];
		var w = t.v[k]
				.clone()
				.sub(yvectors[k]
						.clone()
						.add(t.x_hat)
					);

		w.multiplyScalar( t.scale(i, n) );
		t.vp[k].addVectors(t.v[k], w);
	}

}
// V => V'

function rotate(t){
	var n  = t.m.length;
	for(var i in t.m){ // for every vertice in region
		var w = new THREE.Matrix4().makeRotationAxis( t.axis, t.scale(i, n) );	
		t.vp[t.m[i]].applyMatrix4(w);
	}
}

function scale(t){
	//DEFORM - SCALE
	var n = t.m.length;
	for(var i in t.m){ // for every vertice in region
		var w = t.axis.clone();
		w.multiplyScalar( t.scale(i, n) );
		t.vp[t.m[i]].addVectors(t.v[t.m[i]], w);
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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  DNA.JS   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

var generateMode = false;

function DNA(){
	this.extract = function(){
		headers = []; for(var i in operations) headers.push(operations[i].name);
		opval = {};
		sliders = $('.op-slider');
		for(var i in sliders)
			opval[sliders[i].name]  = parseFloat(sliders[i].value);
			
		var fv = [];
		for(var i in headers) fv.push(opval[headers[i]]);
		return fv;
	}
	this.generate = function(){
		generateMode = true;
		$.each($('.region'), function(i,v){
			$('.region').removeClass('selected');
			$(v).click();
			$($('.op-slider')[0]).val(Math.random() * 2 - 1).change();
			$($('.op-slider')[1]).val(Math.random() * 2 - 1).change();
			$($('.op-slider')[2]).val(Math.random() * 2 - 1).change();
		});
		generateMode = false;
	}
}
