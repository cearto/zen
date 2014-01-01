
 // function onWindowResize() {
 //        camera.aspect = window.innerWidth / window.innerHeight;
 //        camera.updateProjectionMatrix();
 //        renderer.setSize( window.innerWidth, window.innerHeight );
//  //    }

// function updateView(R, theta, phi, zoomFactor){
// 	camera.fov = 300 *  zoomFactor;
// 	camera.updateProjectionMatrix();
// 	render();
// }
// function addSphere(scene){
	
// 	var radius = 0.25, segments = 40,rings = 40;
// 	var geom = new THREE.SphereGeometry(radius, segments, rings);
// 	var sphere = new THREE.Mesh(geom, cubeMaterial);
// 	sphere.geometry.dynamic = true;
	
// 	testHighlight(sphere);

// 	scene.add(sphere);
// 	test = [];
// 	for(var i = 0; i < sphere.geometry.vertices.length/2; i++) test.push(i);
// 	MeshBalloon.transform(sphere, test, null, 1.5);
// }



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

// function loadCylinder(scene){
// 	var radius = 5, segments = 40,rings = 40, height=10;
// 	var geom = new THREE.CylinderGeometry(radius, radius, height, rings, segments, false);
// 	var object = new THREE.Mesh(geom, cubeMaterial);
// 	cl(object);
// 	object.overdraw = true;
// 	object.geometry.dynamic = true;
// 	object.normalizeMesh();

// 	scene.userData.objects.push(new ExpFab(object, regions, pca));	
// 	GUIRegions();
// 	scene.add(object);

// 	return {'name' : 'Cylinder', 'mtime': "3:45pm", 'author' : 'Cesar Torres', 'date': 'November 19, 2013'}
// }
