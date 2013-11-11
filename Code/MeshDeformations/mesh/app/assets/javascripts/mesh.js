var lambertMaterial = new THREE.MeshLambertMaterial({color: 0xCC0000});
var basicMaterial = new THREE.MeshBasicMaterial({color: 0xCCCCCC});
var groundMaterial = new THREE.MeshPhongMaterial( { ambient: 0xCCCCCC, color: 0xCCCCC, specular: 0x101010 } );

var cubeMaterial = new THREE.MeshBasicMaterial( 
    { color: 0xffffff, shading: THREE.FlatShading, 
    vertexColors: THREE.VertexColors } );

var materials = [cubeMaterial, lambertMaterial, basicMaterial, groundMaterial];


function initOpenGL(container){
	HEIGHT = $(window).height(),  WIDTH = $(window).width() - 150;
	VIEW_ANGLE = 35 * 0.1, ASPECT = WIDTH / HEIGHT, NEAR = 0.1, FAR = 10000;
 	
 	// Setup
	renderer.setSize(WIDTH, HEIGHT);
	container.append(renderer.domElement);
	setupLights(scene);
	setupCamera(scene);
	addControls();
	
	// Scene
	// addSphere(scene);
	loadSTL('/stl/slotted_disk.stl');

	$(renderer.domElement).css({position: 'relative', left: '50%', 'margin-left': -WIDTH/2});
	render();
}

function addControls(){
	controls = new THREE.TrackballControls( camera, renderer.domElement );
	controls.rotateSpeed = 1.0;
	controls.zoomSpeed = 1.2;
	controls.panSpeed = 0.2;
	 
	controls.noZoom = false;
	controls.noPan = false;
	 
	controls.staticMoving = false;
	controls.dynamicDampingFactor = 0.3;
	 
	controls.minDistance = 1.1;
	controls.maxDistance = 100;
	 
	controls.keys = [ 16, 17, 18 ]; // [ rotateKey, zoomKey, panKey ]
	controls.addEventListener( 'change', render );
}
function highlightRegion(regionIndices, geom, color){
	var faceIndices = ['a', 'b', 'c', 'd'];  
	for(var i in regionIndices){
		var face = geom.faces[ regionIndices[i] ];   
		var numberOfSides = ( face instanceof THREE.Face3 ) ? 3 : 4;
		for( var j = 0; j < numberOfSides; j++ ){
		    var vertexIndex = face[ faceIndices[ j ] ]; 
		    face.vertexColors[ j ] = color;
		}
	}
}
function addSphere(scene){
	var gray = Math.random();
	var color = new THREE.Color( 0xffc94c );

	var radius = 0.25, segments = 20,rings = 20;
	var geom = new THREE.SphereGeometry(radius, segments, rings);
	var sphere = new THREE.Mesh(geom, cubeMaterial);
	sphere.geometry.dynamic = true;
	region = sphere.geometry.faces.length / 2;
	topRegion = [], bottomRegion = [];
	for(var i = 0; i < region; i++) topRegion.push(i);
	for(var i = region; i < region*2; i++) bottomRegion.push(i);

	highlightRegion(topRegion, sphere.geometry, color);
	gray = Math.random();
	color = new THREE.Color( 0x41ccb4);
	highlightRegion(bottomRegion, sphere.geometry, color);
	scene.add(sphere);
}

function setupLights(scene){
	var pointLight = new THREE.PointLight(0xFFFFFF);
	pointLight.position.x = 10;
	pointLight.position.y = 200;
	pointLight.position.z = 130;
	scene.add(pointLight);
}
function addPlane(scene){
	var plane = new THREE.Mesh( new THREE.PlaneGeometry( 4000, 800 ), basicMaterial );
	plane.rotation.x = -Math.PI/2;
	plane.position.y = -0.5;
	plane.static = true;
	scene.add( plane );
	plane.receiveShadow = true;
}

function setupCamera(){
	camera = new THREE.PerspectiveCamera(VIEW_ANGLE, ASPECT, NEAR, FAR);
	camera.up = new THREE.Vector3(0,1,0);
	camera.position.set( 20, 0.5, 3 );
	camera.lookAt(scene.position);
	scene.add(camera);
}

function animate() {
	requestAnimationFrame( animate );
	controls.update();
}

function render() {
	renderer.render( scene, camera );
}

function loadSTL(filename){
    var loader = new THREE.STLLoader();
    loader.addEventListener( 'load', function ( event ) {
        var geometry = event.content;
        var mesh = new THREE.Mesh( geometry, lambertMaterial );
        scene.add( mesh );
        render();
    } );
    loader.load( filename );
}	
 // function onWindowResize() {
 //        camera.aspect = window.innerWidth / window.innerHeight;
 //        camera.updateProjectionMatrix();
 //        renderer.setSize( window.innerWidth, window.innerHeight );
 //    }