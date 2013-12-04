
weather = [];

var yvectors;
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


function scale(original, mask, vprime, centroid,  majoraxis, ds, amp){
	yvectors = [];

	for(var i in original){
		var vr = original[i].clone();
		vr.sub(centroid);		
		vr.projectOnVector(majoraxis);
		yvectors.push(vr);
	}

	dsp = function(i,n){
		var param = parseFloat(i)/n;
		return linInterp(param, ds);
	}
	/* param is from 0 to 1 */
	linInterp = function(param, data){
		var n = data.length - 1;
		var index = param * n;
		var interp = index - parseInt(index);
		index = parseInt(index);
		return data[index] * (1-interp) +  data[index + 1] * (interp);
	}
	square = function(param){ 
		var s = param * 3 - 1;
		return s * s;
	};

	//DEFORM - BALLOON
	var n = mask.length;
	for(var i in mask){
		var weight = original[mask[i]].clone().sub(yvectors[mask[i]].clone().add(centroid));
		weight.multiplyScalar( dsp(i, n) * amp );
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
		for(var i in exfab.regions) mask.push(i);
	else
	$.each(selectedRegions, function(i, reg){ 
		var id  = parseInt($(reg).attr('data-r')); 
		cl(id);
		mask.push(id);
	});
	return mask;
}

function computeNormal(v1, v2){ return v1.clone().cross(v2).normalize();}




// addParticle(centroid, true);
// var close = []; for(var i in mask) close.push([mask[i], original[mask[i]].distanceTo(centroid)]); close = close.sort(function(a,b){return a[1]-b[1]}); id1 = close[0][0]; id2 = close[1][0]; var j = 2; while(id2 == id1){ id2 = close[j][0]; j++;}
// var v1 = original[id1].clone(); addNormal(centroid, v1, false); v1 = v1.sub(centroid);addParticle(original[id1].clone(), false);
// var v2 = original[id2].clone(); addNormal(centroid, v2, false); v2 = v2.sub(centroid);addParticle(original[id2].clone(), false);
// n = computeNormal(v1, v2);

// this.walk = function(centroid, n, delta, steps){
// 		addNormal(centroid, centroid.clone().add(n.clone().multiplyScalar(delta)), true);
// 		centroid = centroid.clone().add(n.clone().multiplyScalar(delta));
// 		addParticle(centroid, true);
// 		var close = []; for(var i in mask) close.push([mask[i], original[mask[i]].distanceTo(centroid)]); close = close.sort(function(a,b){return a[1]-b[1]}); id1 = close[0][0]; id2 = close[1][0]; var j = 2; while(id2 == id1){ id2 = close[j][0]; j++;}
// 		var v1 = original[id1].clone(); 
// 		//addNormal(centroid, v1, false); 
// 		v1 = v1.sub(centroid);
// 		//addParticle(original[id1].clone(), false);
// 		var v2 = original[id2].clone(); 
// 		// addNormal(centroid, v2, false); 
// 		v2 = v2.sub(centroid);
// 		// addParticle(original[id2].clone(), false);		
// 		n1 = computeNormal(v1, v2);
// 		if(n1.clone().add(n).length() < n.length()) n1.multiplyScalar(-1);
// 		if(steps > 0)
// 			this.walk(centroid, n1, delta, steps - 1);
// 	}
// 	this.walk(centroid, n, 0.02, 20);

// 	for(var i in mask)
// 		cl(original[mask[i]].x + " " + original[mask[i]].y + " " + original[mask[i]].z);
/* Mesh Deformation */
// var close = [];
// function walkAxis(original, mask, centroid){
// 	var close = []
// 	for(var i in mask) 
// 		close.push([mask[i], original[mask[i]].distanceTo(centroid)]);
// 	//find two closest points
// 	close = close.sort(function(a,b){return a[1]-b[1]});
// 	id1 = close[0][0];
// 	id2 = close[1][0];
// 	var j = 2;
// 	while(id2 == id1){
// 		id2 = close[j][0];
// 		j ++ ;
// 	}
// 	//addParticle(original[id1].clone(), false);
// 	//addParticle(original[id2].clone(), false);
// 	//compute normal of plane spanned by two vectors
// 	var v1 = original[id1].clone();
// 	var v2 = original[id2].clone();
// 	//cl(close);
// 	addNormal(centroid, v1, false);
// 	addNormal(centroid, v2, false);
// 	v1 = v1.sub(centroid);
// 	v2 = v2.sub(centroid);
// 	n = computeNormal(v1, v2);
// 	n = centroid.clone().add(n.multiplyScalar(0.1));
// 	addNormal(centroid, n, true);
// 	return n;
// }
