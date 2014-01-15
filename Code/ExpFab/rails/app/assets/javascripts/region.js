var selectedRegions;

function Region(expfab, regid, faces, pca){
	this.expfab = expfab;
	this.regid = regid;
	this.color = colors[regid];
	this.faces = faces;
	this.pca = [];

	for(var i in pca) this.pca.push(new THREE.Vector3(pca[i][0], pca[i][1], pca[i][2]));

	this.vertices = null;
	this.centroid = null;
	this.geometry = null;
	this.updateVertices();
	this.updateCentroid();

	this.highlight(false);

	this.pca_lines = buildAxesPCA(this.centroid, this.pca); 
	this.params = {
		"x_hat": this.centroid,
		"v": this.expfab.current, 
		"vp": this.expfab.object.geometry.vertices, 
		"m": this.vertices, 
	};
}

Region.prototype.generateImage = function(){
	var uriData    = this.expfab.mgui.canvas[0].toDataURL("image/png");
	var box = new THREE.Box2();
	
	var projScreenMat = new THREE.Matrix4();
	projScreenMat.multiplyMatrices( camera.projectionMatrix, camera.matrixWorldInverse );

	var worldVert = [];
	for(var i in this.geometry){
		var pt = THREE.toScreenXY ( this.geometry[i], projScreenMat, this.expfab.mgui.canvas)
		worldVert.push(pt);
		//cl(pt);
	}
	box.setFromPoints(worldVert);
	// box.min.setX(box.min.x * WIDTH);
	// box.min.setY(box.min.y * HEIGHT)
	// box.max.setX(box.max.x * WIDTH);
	// box.max.setY(box.max.y * HEIGHT);
	this.expfab.mgui.gallery.add(uriData, box);
}
Region.prototype.updateVertices = function(){
	this.vertices = [];
	this.geometry = [];
	for(var faceIndex in this.faces){
		var face = this.expfab.object.geometry.faces[this.faces[faceIndex]];
		vertexIndex = face['a']; this.vertices.push(vertexIndex);
		this.geometry.push(this.expfab.object.geometry.vertices[vertexIndex]);
		vertexIndex = face['b']; this.vertices.push(vertexIndex);
		this.geometry.push(this.expfab.object.geometry.vertices[vertexIndex]);
		vertexIndex = face['c']; this.vertices.push(vertexIndex);
		this.geometry.push(this.expfab.object.geometry.vertices[vertexIndex]);
	}
}
Region.prototype.updateCentroid = function(){
	var mask = this.vertices;
	this.centroid = new THREE.Vector3(0, 0, 0);
	for(var j in this.vertices) this.centroid.add(this.expfab.original[this.vertices[j]]);
		this.centroid.divideScalar(mask.length);
}
Region.prototype.highlight = function (active, type){
	var c = this.color['selected'];
	this.expfab.object.material.opacity = 1;
	if(!active){	
		c = this.color['default'];
		this.expfab.object.material.opacity = 0.8;
	}
	
	if(type == 'hover') c = !active ? this.color['default'] : this.color['hover'];
	for(var faceIndex in this.faces){
		var face = this.expfab.object.geometry.faces[this.faces[faceIndex]];
		face.vertexColors[0] = c;
		face.vertexColors[1] = c;
		face.vertexColors[2] = c;
	}
	this.expfab.object.geometry.colorsNeedUpdate = true; render();
}

Region.prototype.addGUI = function(container){
	var node = GUINode(container, "R" + this.regid, this.color['default'].getStyle(), null, false);
	var that = this;
	node.attr('data-n', this.expfab.id).attr('data-r', this.regid).addClass('region').click(function(){
			var id = parseInt($(this).attr('data-n'));
			var regid = parseInt($(this).attr('data-r'));
			var meshcolor = colors[regid];
			var obj = that.expfab.object;
			var reg = that.expfab.regions;
			var others = selectedRegions2(mainExpFab).length;

			if(! ($(this).hasClass('selected'))){
				that.highlight(true, 'active');
				$(this).addClass('selected');
				for(var i in that.pca_lines){
					if(others == 0 || others == mainExpFab.regions.length)
					scene.add(that.pca_lines[i]);
				}

			}

			else{
				that.highlight(false, 'active');
				$(this).removeClass('selected');
				for(var i in that.pca_lines){
					scene.remove(that.pca_lines[i]);
				}

			}

			selectedRegions = $('.selected');
			
			render();	
		}).hover(function(){
			if($(this).hasClass('selected')) return;
			var id = parseInt($(this).attr('data-n'));
			var regid = parseInt($(this).attr('data-r'));
			var meshcolor = colors[regid];
			var obj = that.expfab.object;
			var reg = that.expfab.regions;
			
				that.highlight(true, 'hover');
				

		}, function(){
			if($(this).hasClass('selected')) return;
			var id = parseInt($(this).attr('data-n'));
			var regid = parseInt($(this).attr('data-r'));
			var meshcolor = colors[regid];
			var obj = that.expfab.object;
			var reg = that.expfab.regions;
			
			that.highlight(false, 'hover');
			

		});

	container.append(node).css({
		width: '94%',
		padding: '3%',
		background: 'rgba(225, 225, 225, 0.8)'
	});
	render();
}

Region.prototype.transform = function(ds, amp){
	this.params.scale = function(i, n){ return Mapping.dsp(i, n, ds) * amp };

	if(activeOperation == 'Balloon')
		balloon(this.params);
	else if(activeOperation == "Scale")
		scale(this.params);
	else if(activeOperation == "Rotate")
		rotate(this.params);

	this.expfab.object.geometry.verticesNeedUpdate = true;
	render();
}	
