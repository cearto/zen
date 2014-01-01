var fileinfo;
var cubeMaterial;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  VISUALS  ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function addNormal(p, n, type){
	if(type)
		lineMaterial = new THREE.LineBasicMaterial({
        color: 0xff2222,
        linewidth: 4
    });
    else
    	lineMaterial = new THREE.LineBasicMaterial({
        color: 0x0000ff,
        linewidth: 4
    });
    var geometry = new THREE.Geometry();
    geometry.vertices.push(p.clone());
    geometry.vertices.push(n.clone());
    var line = new THREE.Line(geometry, lineMaterial);
    scene.add(line);
}
function addParticle(v, type){
	var	particles = new THREE.Geometry();
	if(type)
	    pMaterial = new THREE.ParticleBasicMaterial({
	      color: 0x00A8E1,
	      size: 0.5
	    });
	else
		pMaterial = new THREE.ParticleBasicMaterial({
	      color: 0x333333,
	      size: 0.5
	    });

	particles.dynamic = true;
	particleSystem = new THREE.ParticleSystem(particles, pMaterial);
	particle = v.clone();
	particleSystem.geometry.vertices.push(particle);
	scene.add(particleSystem);
	render();
}

var NONE = 0, DOWN = 1, UP = 2, DRAG = 3;
THREE.ArrowHelper.prototype.state = NONE;
THREE.ArrowHelper.prototype.colors = {"inactive" : 0xd8292f, "active": 0x991c20}; 
THREE.ArrowHelper.prototype.click = function(){
	if(this.state == DOWN){
		this.state = UP;
		this.setColor(this.colors["inactive"]);
	}
	else{
		this.state = DOWN;
		this.setColor(this.colors["active"]);
	}
	this.cone.geometry.colorsNeedUpdate = true;
	this.line.geometry.colorsNeedUpdate = true;
	render();
}

function buildAxesPCA(centroid, pca_s ) {
	length = 1;
    // var axes = new THREE.Object3D();
    var axes = [];
    
    var arrow = new THREE.ArrowHelper( pca_s[0].clone().normalize(), centroid.clone(), length, 0xd8292f );
    arrow.colors = {"inactive" : 0xd8292f, "active": 0x991c20};
    arrow.cone.name = "0";
    // axes.add(arrow);
    axes.push(arrow);
    
    arrow = new THREE.ArrowHelper( pca_s[1].clone().normalize(), centroid.clone(), length, 0x00956e );
    arrow.colors = {"inactive" : 0x00956e, "active": 0x00c894}
    // axes.add(arrow);
    arrow.cone.name = "1";
    axes.push(arrow);
    
    arrow = new THREE.ArrowHelper( pca_s[2].clone().normalize(), centroid.clone(), length, 0x435cc8 );
    arrow.colors = {"inactive" : 0x435cc8, "active": 0x7e8fd9}
    // axes.add(arrow);
    arrow.cone.name = "2";
    axes.push(arrow);


    // axes.add( buildAxis( centroid.clone(), pca_s[0].clone().multiplyScalar(length), 0xd8292f, false ) ); // +X
    // axes.add( buildAxis( centroid.clone(), pca_s[0].clone().multiplyScalar(-length), 0xd8292f, true) ); // -X
    // axes.add( buildAxis( centroid.clone(), pca_s[1].clone().multiplyScalar(length), 0x00956e, false ) ); // +Y
    // axes.add( buildAxis( centroid.clone(), pca_s[1].clone().multiplyScalar(-length), 0x00956e, true ) ); // -Y
    // axes.add( buildAxis( centroid.clone(), pca_s[2].clone().multiplyScalar(length), 0x435cc8, false ) ); // +Z
    // axes.add( buildAxis( centroid.clone(), pca_s[2].clone().multiplyScalar(-length), 0x435cc8, true ) ); // -Z
    // scene.add(axes);
    // render();
    return axes;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////  LOADING ROUTINE  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function initOpenGL(container){

	HEIGHT = $(window).height(),  WIDTH = $(window).width() - 260;
	VIEW_ANGLE = 35 * 0.1, ASPECT = WIDTH / HEIGHT, NEAR = 0.1, FAR = 2000;
 	extendTHREE();
 	scene.userData.objects = [];
 	// Setup
 	projector = new THREE.Projector();
	renderer.setSize(WIDTH, HEIGHT);

	container.append(renderer.domElement);
	setupLights(scene);
	setupCamera(scene);
	addControls();
	cubeMaterial = new THREE.MeshPhongMaterial( { wireframe: getURLParameter('w') == '1', ambient: 0x030303, color: 0xdddddd, specular: 0x009900, shininess: 30, shading: THREE.FlatShading, vertexColors: THREE.VertexColors, overdraw : true});
	// cubeMaterial = new THREE.MeshBasicMaterial( { wireframe: getURLParameter('w') == '1', color: 0xffffff, shading: THREE.SmoothShading, vertexColors: THREE.VertexColors,  overdraw : true} );
	// Scene
	loadFile(scene);
	
	// axes = buildAxes( 1000 );
	// scene.add(axes);
	
	$(renderer.domElement).css({position: 'relative', left: '50%', 'margin-left': -WIDTH/2, cursor: 'pointer'});
	render();
	$.get('/mesh/weather', function(data){
		weather = data;
		weather = normalize(weather);
		var w = new DataStream(weather, "Weather", "Uniform random var", "A uniform data stream.");
		w.add($('#streams'));
		w.load();
		datastreams.push(w);
	});

}

function saveFile(){
	var filename = $('.filename').html();
	$.ajax({
	  type: "POST",
	  url: '/mesh/save',
	  data: {'filename': filename, 'mesh' : THREE.saveGeometryToObj(mainExpFab.object.geometry)},
	  dataType: 'text'
	}).done(function(data){
		$('.filename').attr('href', data).html(filename + "_s");
	});
}

function loadFile(scene){
	
	// if(getURLParameter('n') == '0')
	// 	fileinfo = loadCylinder(scene);
	// else
		fileinfo = THREE.loadSTL(scene, '/stl/'+getURLParameter('n')+'.stl');
	
	GUIFile(fileinfo);
	// GUIRegions();
	GUIOperations();
	GUIStreams();
	loadDataStreams($('#streams'));
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  SCENE SETUP  ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


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

function render() { renderer.render( scene, camera );}


function buildAxes( length ) {
        var axes = new THREE.Object3D();

        axes.add( buildAxis( new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( length, 0, 0 ), 0xd8292f, false ) ); // +X
        axes.add( buildAxis( new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( -length, 0, 0 ), 0xd8292f, true) ); // -X
        axes.add( buildAxis( new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 0, length, 0 ), 0x00956e, false ) ); // +Y
        axes.add( buildAxis( new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 0, -length, 0 ), 0x00956e, true ) ); // -Y
        axes.add( buildAxis( new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 0, 0, length ), 0x435cc8, false ) ); // +Z
        axes.add( buildAxis( new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 0, 0, -length ), 0x435cc8, true ) ); // -Z
        return axes;
}
function buildAxis( src, dst, colorHex, dashed ) {
        var geom = new THREE.Geometry(), mat; 
        if(dashed) mat = new THREE.LineDashedMaterial({ linewidth: 1, color: colorHex, dashSize: 0.15, gapSize: 0});
        else mat = new THREE.LineBasicMaterial({ linewidth: 1, color: colorHex });

        geom.vertices.push( src.clone() );
        geom.vertices.push( dst.clone() );
        geom.computeLineDistances(); // This one is SUPER important, otherwise dashed lines will appear as simple plain lines

        var axis = new THREE.Line( geom, mat, THREE.LinePieces );
        return axis;
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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////  CALIBRATION AND LOADING FUNCTIONS   ////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
function extendTHREE(){
	THREE.Mesh.prototype.rotate = function(){
		var mesh = this;
		var axis = new THREE.Vector3( - 1, 0, 0 );
		var angle = Math.PI / 2;
		var matrix = new THREE.Matrix4().makeRotationAxis( axis, angle );

		var axis = new THREE.Vector3( 0, 1, 0 );
		var angle = Math.PI/2;
		var matrix2 = new THREE.Matrix4().makeRotationAxis( axis, angle );
		var middle = new THREE.Vector3(0, 0.5, 0);
		for(var i in mesh.geometry.vertices){
			mesh.geometry.vertices[i].applyMatrix4(matrix);
			mesh.geometry.vertices[i].applyMatrix4(matrix2);
			mesh.geometry.vertices[i].subVectors(mesh.geometry.vertices[i], middle);
		}
	}

	THREE.Mesh.prototype.normalizeMesh = function(){
		var mesh = this;
		var cpy2 = [];
		mesh.geometry.mergeVertices()
		for(var i in mesh.geometry.vertices)
			cpy2[i] = mesh.geometry.vertices[i].clone();
		var length = [];
		for(var i in cpy2)
			length[i] = cpy2[i].length();

		var max = Array.max2(length);
		
		for(var i in cpy2)
			cpy2[i].multiplyScalar(1.0/max);

		mesh.geometry.vertices = cpy2;
		// mesh.rotate();
	}

	THREE.saveGeometryToObj = function (geometry) {
		var s = '';
		for (i = 0; i < geometry.vertices.length; i++) {
		    s+= 'v '+(geometry.vertices[i].x) + ' ' +
		    geometry.vertices[i].y + ' '+
		    geometry.vertices[i].z + '\n';
		}

		for (i = 0; i < geometry.faces.length; i++) {

		    s+= 'f '+ (geometry.faces[i].a+1) + ' ' +
		    (geometry.faces[i].b+1) + ' '+
		    (geometry.faces[i].c+1);

		    if (geometry.faces[i].d !== undefined) {
		        s+= ' '+ (geometry.faces[i].d+1);
		    }
		    s+= '\n';
		}
		return s;
	}
	THREE.loadOFF = function(scene, filename){
		var loader = new THREE.OFFLoader();
		loader.load(filename, function ( object ) {
			var object = new THREE.Mesh( object.geometry, cubeMaterial );
			
			object.overdraw = true;
			object.geometry.dynamic = true;
			object.normalizeMesh();
			mainExpFab = new ExpFab(object, regions, pca);
			scene.userData.objects.push(mainExFab);	
			
		
	        scene.add( object );
	        render();
	        cl('OFF Loaded!');
		} );
		loader.load( filename );
		return {'name' : filename, 'mtime': "3:45pm", 'author' : 'Cesar Torres', 'date': 'November 19, 2013'}
	}	
	THREE.loadSTL = function(scene, filename){
		var loader = new THREE.STLLoader();
	    loader.addEventListener( 'load', function ( event ) {
	        var geometry = event.content;
	        var object = new THREE.Mesh( geometry, cubeMaterial );
	     
			object.overdraw = true;
			object.geometry.dynamic = true;
			object.normalizeMesh();
			mainExpFab = new ExpFab(object, regions, pca);
			scene.userData.objects.push(mainExpFab);	
			
			GUIRegions(mainExpFab);
	        scene.add( object );
	        render();
	        cl('STL Loaded!');
	    } );
	    loader.load( filename );
		return {'name' : filename, 'mtime': "3:45pm", 'author' : 'Cesar Torres', 'date': 'November 19, 2013'}
	}

}