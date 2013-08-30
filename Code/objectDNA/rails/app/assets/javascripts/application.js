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
//= require GUI.js
//= require sliders.js

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

function convertCanvasToImage(canvas) {
	image = new Image();
	image.src = canvas[0].toDataURL("image/png");	
	return image;
}

function mapValue(value, dMin, dMax, isContinuous, tMin, tMax){
	if(typeof(tMin)==='undefined') tMin = 0.0;
	if(typeof(tMax)==='undefined') tMax = 1.0;
	if(typeof(isContinuous)==='undefined')isContinuous = true;
	var val = ((value - dMin)/(dMax - dMin)) * (tMax - tMin) + tMin;
	return isContinuous ? val : parseInt(val);
}
