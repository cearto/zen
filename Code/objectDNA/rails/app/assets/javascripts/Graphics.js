var mouse2D, mouse3D, raycaster, theta = 45,
isShiftDown = false, isCtrlDown = false,
target = new THREE.Vector3( 0, 200, 0 );
var meshTest;
function Graphics(){

  this.container, this.stats;
  this.camera, this.scene, this.renderer;
  this.projector, this.plane;
  this.normalMatrix = new THREE.Matrix3();
  this.ROLLOVERED;

  this.init = function(){
    this.container = document.createElement( 'div' );
    $('#viewport-panel').append( this.container ).css('position', 'relative');

    var info = document.createElement( 'div' );
    info.style.position = 'absolute';
    info.style.top = '0px';
    info.style.width = '100%';
    info.style.textAlign = 'center';

    info.innerHTML = '<strong>control + click</strong>: remove voxel, <strong>shift</strong>: rotate, <a href="javascript:save();return false;">save .png</a>';
    // this.container.appendChild( info );
    this.camera = new THREE.PerspectiveCamera( 30, $('#viewport-panel').width() /$('#viewport-panel').height() , 1, 10000 );
    this.camera.position.y = 800;
    this.scene = new THREE.Scene();

    // Grid
    var size = 500, step = 50;
    var geometry = new THREE.Geometry();

    for ( var i = - size; i <= size; i += step ) {

      geometry.vertices.push( new THREE.Vector3( - size, 0, i ) );
      geometry.vertices.push( new THREE.Vector3(   size, 0, i ) );
      geometry.vertices.push( new THREE.Vector3( i, 0, - size ) );
      geometry.vertices.push( new THREE.Vector3( i, 0,   size ) );

    }

    var material = new THREE.LineBasicMaterial( { color: 0x000000, opacity: 0.2 } );
    var line = new THREE.Line( geometry, material );
    line.type = THREE.LinePieces;
    this.scene.add( line );
    //

    this.projector = new THREE.Projector();
    this.plane = new THREE.Mesh( new THREE.PlaneGeometry( 1000, 1000 ), new THREE.MeshBasicMaterial() );
    this.plane.rotation.x = - Math.PI / 2;
    this.plane.visible = false;
    this.scene.add( this.plane );
    mouse2D = new THREE.Vector3( 0, 10000, 0.5 );

    // Lights

    var ambientLight = new THREE.AmbientLight( 0x606060 );
    this.scene.add( ambientLight );

    var directionalLight = new THREE.DirectionalLight( 0xffffff );
    directionalLight.position.x = Math.random() - 0.5;
    directionalLight.position.y = Math.random() - 0.5;
    directionalLight.position.z = Math.random() - 0.5;
    directionalLight.position.normalize();
    this.scene.add( directionalLight );

    var directionalLight = new THREE.DirectionalLight( 0x808080 );
    directionalLight.position.x = Math.random() - 0.5;
    directionalLight.position.y = Math.random() - 0.5;
    directionalLight.position.z = Math.random() - 0.5;
    directionalLight.position.normalize();
    this.scene.add( directionalLight );

    this.renderer = new THREE.CanvasRenderer();
    this.renderer.setSize( $('#viewport-panel').width(), $('#viewport-panel').height() );

    this.container.appendChild(this.renderer.domElement);

    addClickHandler(document, 'mousemove', this.onDocumentMouseMove, false, this);
    addClickHandler(document, 'mousedown', this.onDocumentMouseDown, false, this);
    addClickHandler(document, 'keydown', this.onDocumentKeyDown, false, this);
    addClickHandler(document, 'keyup', this.onDocumentKeyUp, false, this);
    addClickHandler(document, 'resize', this.onWindowResize, false, this);

  }

  // GRAPHICS FUNCTIONS
  this.createElement = function (intersect){
    //GRAPHICS
    console.log(intersect);
    /* Get normal of intersected object */
    this.normalMatrix.getNormalMatrix( intersect.object.matrixWorld );
    var normal = intersect.face.normal.clone();
    normal.applyMatrix3( this.normalMatrix ).normalize();
    
    //GML
    /* Get position, make a new Voxel */
    var position = new THREE.Vector3().addVectors( intersect.point, normal );
    var geometry = new THREE.CubeGeometry( 50, 50, 50 );
    var voxel = new Voxel(position, geometry, intersect, 'Cube');
    meshTest = voxel;
    return voxel;
  }

  this.addElement = function(vx){
    this.scene.add( vx.mesh ); //Graphics
  }
  
  this.loadScene = function(gml){
    console.log('Rendering ..');
    comp = gml.getComponents();
    for(var i in comp){
      console.log(comp[i]);
      gml.addComponent(comp[i]);
      g.scene.add( this.renderFromFile(comp[i]) ); //Graphics
      meshTest = comp[i];
    }

  }


this.renderFromFile = function(vx){
    for ( var i = 0; i < vx.geometry.faces.length; i ++ )
        setHex(vx.geometry.faces[ i ].color, 0x00A8e1 );
    /* Apply matte material */
    var material = new THREE.MeshLambertMaterial( { vertexColors: THREE.FaceColors } );
    mesh = new THREE.Mesh( vx.geometry, vx.material );

    /* Snap to grid */ 
    mesh.position.x = Math.floor( vx.position.x / 50 ) * 50 + 25;
    mesh.position.y = Math.floor( vx.position.y / 50 ) * 50 + 25;
    mesh.position.z = Math.floor( vx.position.z / 50 ) * 50 + 25;
    mesh.matrixAutoUpdate = false;
    mesh.updateMatrix();
    this.scene.add(mesh);
    return mesh;
}


  this.removeComponent = function (intersect){
    if ( intersect.object != this.plane )
      this.scene.remove( intersect.object );
    return intersect.object;
  }

  this.removeAll = function(){
    components =  gml.getComponents();
    for(var i in components)
        this.scene.remove(components[i].mesh); 
  }

  //DOCUMENT HANDLERS
  this.onWindowResize = function(event, self) {
    this.camera.aspect = $('#viewport-panel').width()/ $('#viewport-panel').height();
    this.camera.updateProjectionMatrix();
    this.renderer.setSize( $('#viewport-panel').width(), $('#viewport-panel').height() );
  }

  this.onDocumentMouseMove = function( event, self ) {
    var offset = $('canvas').offset();
    //event.preventDefault();

    // Coordinates are float-optimized with origin at the center of the canvas
    mouse2D.x = ((event.clientX - offset.left) / $('#viewport-panel').width()) * 2 - 1 ;//
    mouse2D.y = -((event.clientY - offset.top) / $('#viewport-panel').height()) * 2 + 1 ;
    var intersects = raycaster.intersectObjects( self.scene.children );
    //console.log( self.scene.children);

    if ( intersects.length > 0 && self.ROLLOVERED != null) {
      if ( self.ROLLOVERED ) self.ROLLOVERED.color.setHex( 0x00b8c1 );

      self.ROLLOVERED = intersects[ 0 ].face;
      self.ROLLOVERED.color.setHex( 0x00A8e1 )
    }
  }
  
  this.onDocumentMouseDown = function( event, self ) {
    // console.log('mousedown');
    var intersects = raycaster.intersectObjects( self.scene.children );

    /* Checks if ray hits an object in the this.scene */
    if ( intersects.length > 0 ) {
      var intersect = intersects[ 0 ];
      if ( isCtrlDown ) removeComponent(intersect);         
      else addComponent(self.createElement(intersect));
    }

  }

  this.onDocumentKeyDown = function( event, self ) {
    switch( event.keyCode ) {
      case 16: isShiftDown = true; break;
      case 17: isCtrlDown = true; break;
    }
  }

  this.onDocumentKeyUp = function( event, self ) {
    switch( event.keyCode ) {
      case 16: isShiftDown = false; break;
      case 17: isCtrlDown = false; break;
    }
  }


  // UTILITY FUNCTIONS
  this.save = function() {
    window.open( this.renderer.domElement.toDataURL('image/png'), 'mywindow' );
  }

  this.animate = function(){
    var self = this;
    requestAnimationFrame( function(){ self.animate() } );
    this.render();
  }

  this.render = function() {
    if ( isShiftDown ) {
      theta += mouse2D.x * 3;
    }
    this.camera.position.x = 1400 * Math.sin( theta * Math.PI / 360 );
    this.camera.position.z = 1400 * Math.cos( theta * Math.PI / 360 );
    this.camera.lookAt( target );

    raycaster = this.projector.pickingRay( mouse2D.clone(), this.camera );
    this.renderer.render( this.scene, this.camera );
  }
}

function Generator(model){
  this.model = model;
}