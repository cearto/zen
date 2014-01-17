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

var mainExpFab;
// SCENE VARIABLES
var camera, controls, projector;
var stats, WIDTH, HEIGHT, ASPECT, NEAR, FAR, VIEW_ANGLE;
var Z = 300; var zoomLevel = 0.01; var theta = 0; var phi = 0;

var xaxis = new THREE.Vector3(1, 0, 0);
var yaxis = new THREE.Vector3(0, 1, 0);
var zaxis = new THREE.Vector3(0, 0, 1);

// MOUSE EVENTS
var down  = new THREE.Vector3(0, 0, 0);
var up  = new THREE.Vector3(0, 0, 0);
var current  = new THREE.Vector3(0, 0, 0);
var originalAxisLength = 1;
var activeAxis = null;
var activeOperation = '';

// SCENE VARIABLES
var scene = new THREE.Scene();
var renderer = new THREE.WebGLRenderer({ antialias: true,  preserveDrawingBuffer: true } );


var defaultMaterialSettings = { 
    wireframe: false, 
    //ambient: 0x030303, 
    color: 0xdddddd, // gray 
    specular: 0x0000, //white
    shininess: 30, 
    shading: THREE.FlatShading, 
    vertexColors: THREE.VertexColors, 
    overdraw : true,
    transparent: true,
    opacity: 0.5
};

var defaultColor = 0x777777;
var hoverColor = 0xAAAAAA;
var downColor = 0xAAAAAA;

colors = [{'selected': 0xffc94c, 'active':0xffd87f, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, //yellow
      {'selected':0x41ccb4, 'active': 0x69d7c4, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // light green
      {'selected':0x40c365, 'active': 0x67cf84, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // green
      {'selected': 0xDB4090, 'active':0xe36ba9, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // pink
      {'selected':0x00a8e1, 'active': 0x62d7ff, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // blue
      {'selected':0xd82727, 'active': 0xe05252, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // red
      {'selected':0xebb114, 'active': 0xefc143, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // orange
      {'selected':0x542c54, 'active': 0x542c54, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // purple
      {'selected': 0xffc94c, 'active':0xffd87f, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, //yellow
      {'selected':0x41ccb4, 'active': 0x69d7c4, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // light green
      {'selected':0x40c365, 'active': 0x67cf84, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // green
      {'selected': 0xDB4090, 'active':0xe36ba9, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // pink
      {'selected':0x00a8e1, 'active': 0x62d7ff, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // blue
      {'selected':0xd82727, 'active': 0xe05252, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // red
      {'selected':0xebb114, 'active': 0xefc143, 'default': defaultColor, 'hover': hoverColor, 'down': downColor}, // orange
      {'selected':0x542c54, 'active': 0x542c54,  'default': defaultColor, 'hover': hoverColor, 'down': downColor}// purple
];


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
function clone(obj) {
    if (null == obj || "object" != typeof obj) return obj;
    var copy = obj.constructor();
    for (var attr in obj) {
        if (obj.hasOwnProperty(attr)) copy[attr] = clone(obj[attr]);
    }
    return copy;
}

function include(arr,obj) {
    return (arr.indexOf(obj) != -1);
}
function decimalToHexString(number)
{
    if (number < 0)
    {
      number = 0xFFFFFFFF + number + 1;
    }

    return number.toString(16).toUpperCase();
}