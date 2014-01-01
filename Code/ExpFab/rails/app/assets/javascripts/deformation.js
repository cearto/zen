
weather = [];

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
Mapping.square = function(param){ 
	var s = param * 3 - 1;
	return s * s;
};




function balloon(original, mask, vprime, centroid,  majoraxis, ds, amp){
	var yvectors = [];

	// yvectors = i X (C - Vr) 
	for(var i in original){
		var vr = original[i].clone();
		vr.sub(centroid);		
		vr.projectOnVector(majoraxis);
		yvectors.push(vr);
	}


	//DEFORM - BALLOON
	var n = mask.length;
	for(var i in mask){
		var weight = original[mask[i]].clone().sub(yvectors[mask[i]].clone().add(centroid));
		weight.multiplyScalar( Mapping.dsp(i, n, ds) * amp );
		vprime[mask[i]].addVectors(original[mask[i]], weight);
	}
}
function rotate(original, mask, vprime, centroid,  majoraxis, ds, amp){
	
	//DEFORM - SCALE
	var n = mask.length;
	for(var i in mask){ // for every vertice in region
		var weight = majoraxis.clone();
		weight.multiplyScalar( Mapping.dsp(i, n, ds) * amp );
		// cl(weight);
		vprime[mask[i]].addVectors(original[mask[i]], weight);
	}
}

function scale(original, mask, vprime, centroid,  majoraxis, ds, amp){
	
	//DEFORM - SCALE
	var n = mask.length;
	for(var i in mask){ // for every vertice in region
		var weight = majoraxis.clone();
		weight.multiplyScalar( Mapping.dsp(i, n, ds) * amp );
		// cl(weight);
		vprime[mask[i]].addVectors(original[mask[i]], weight);
	}
}
function normalize(arr){
	var max = Math.max.apply(null, arr);
	for(var i in arr)
	arr[i] /= max;
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
