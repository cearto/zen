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
//= require_tree .
function toCanvasCoord(event){
  var c = $(renderer.domElement);
  return new THREE.Vector3(( (event.clientX - c.offset().left) / c.width() ) * 2 - 1, -( (event.clientY - c.offset().top)/ c.height() ) * 2 + 1, 0);
}

function cl(val){ console.log(val);}

THREE.Vector3.prototype.signTo = function(Vb){
  angle = this.angleTo(Vb);
  cross = this.clone().cross(Vb);
  if (zaxis.dot(cross) < 0) { 
    angle = -angle;
  }
  return angle;
}
Array.max = function( array ){
    return Math.max.apply( Math, array );
};

Array.max2 = function( array ){
	var max = Number.MIN_VALUE;
	for(var i in array)
		if(array[i] > max)
			max = array[i];
    return max;
};

function getURLParameter(name) {
    return decodeURI(
        (RegExp(name + '=' + '(.+?)(&|$)').exec(location.search)||[,null])[1]
    );
}

var invert = function (obj) {

  var h = {};

  for (var i in obj)
  	for(var j in obj[i])
  		if(obj[i][j] in h)
  			h[obj[i][j]].append(parseInt(i));
  		else
  			h[obj[i][j]] = [parseInt(i)];
  

  return h;
};

function decimalToHexString(number)
{
    if (number < 0)
    {
      number = 0xFFFFFFFF + number + 1;
    }

    return number.toString(16).toUpperCase();
}