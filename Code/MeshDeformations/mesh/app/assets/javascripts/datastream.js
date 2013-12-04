var currentOperation;
var activeDataStream;
var selectedRegions;

function Operation(container, name, icon, functionality){
	this.name = name;
	functionality = function(){

		var data = scene.userData.objects[0];
		activeDataStream = datastreams[0];
		activeDataStream.applyToOperation(0.1);
		currentOperation = name;

		var ampSlider = $(".op-slider[name='"+ currentOperation +"']");
		if(ampSlider.length > 0) ampSlider.focus();
		else{
			ampSlider = $('<input/>').addClass('op-slider').attr('type', 'range').attr('name', currentOperation).attr('min', -1).attr('max', 1).attr('step', 0.01)
			.change(function(){
				var val = $(this).val();
				if(currentOperation != $(this).attr('name') && !generateMode){
					activeDataStream.saveState();
				}
				currentOperation = $(this).attr('name');
				
				activeDataStream.applyToOperation(val);
			});
			$(this).parent().append(ampSlider);
		}
	}
	
	
	var node = GUINode(container, name, icon, functionality, true);
	container.append(node).css({
		width: '94%',
		padding: '3%',
		background:  'rgba(225, 225, 225, 0.8)'
	});

}

/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/

function Region(exfab, regid, faces, pca){
	this.exfab = exfab;

	this.regid = regid;
	this.color = colors[regid];
	this.faces = faces;
	this.pca = [];
	for(var i in pca) this.pca.push(new THREE.Vector3(pca[i][0], pca[i][1], pca[i][2]));

	this.vertices = [];
	for(var faceIndex in faces){
		var face = this.exfab.object.geometry.faces[faces[faceIndex]];
		vertexIndex = face['a']; this.vertices.push(vertexIndex);
		vertexIndex = face['b']; this.vertices.push(vertexIndex);
		vertexIndex = face['c']; this.vertices.push(vertexIndex);
	}
	this.centroid = [];

		
	var mask = this.vertices;
	this.centroid = new THREE.Vector3(0, 0, 0);
	for(var j in this.vertices) this.centroid.add(this.exfab.original[this.vertices[j]]);
	this.centroid.divideScalar(mask.length);
	
	this.highlight(false);

	this.pca_lines = buildAxesPCA(this.centroid, this.pca); 
}

Region.prototype.highlight = function (active){
	var c = !active ? this.color['default'] : this.color['active'];
	for(var faceIndex in this.faces){
		var face = this.exfab.object.geometry.faces[this.faces[faceIndex]];
		face.vertexColors[0] = c;
		face.vertexColors[1] = c;
		face.vertexColors[2] = c;
	}
	this.exfab.object.geometry.colorsNeedUpdate = true; render();
}

Region.prototype.addGUI = function(container){
	var node = GUINode(container, "R" + this.regid, this.color['default'].getStyle(), null, false);
	var that = this;
	node.attr('data-n', this.exfab.id).attr('data-r', this.regid).addClass('region').click(function(){
			var id = parseInt($(this).attr('data-n'));
			var regid = parseInt($(this).attr('data-r'));

			var meshcolor = colors[regid];

			var obj = that.exfab.object;
			var reg = that.exfab.regions;
	
			if(! ($(this).hasClass('selected'))){
				that.highlight(true);
				$(this).addClass('selected');
				scene.add(that.pca_lines);

			}

			else{
				that.highlight(false);
				$(this).removeClass('selected');
				scene.remove(that.pca_lines);

			}

			selectedRegions = $('.selected');
			
			render();	
		});

	container.append(node).css({
		width: '94%',
		padding: '3%',
		background: 'rgba(225, 225, 225, 0.8)'
	});
	render();
}

Region.prototype.transform = function(ds, amp){
	var original = this.exfab.current;
	var vprime = this.exfab.object.geometry.vertices;
	var mask = this.vertices;//selectedMask(original,  this.exfab.object);
	cl(this);
	var xaxis = this.pca[0].clone();
	var yaxis = this.pca[1].clone();
	var zaxis = this.pca[2].clone();
	
	if(currentOperation == 'Xscale'){
		scale(original, mask, vprime, this.centroid, xaxis, ds, amp);
		// addNormal(new THREE.Vector3(0, 0, 0), xaxis, false);
	}
	else if (currentOperation == 'Yscale'){
		scale(original, mask, vprime, this.centroid, yaxis, ds, amp);
		// addNormal(new THREE.Vector3(0, 0, 0), yaxis, false);
	}
	else if (currentOperation == 'Translate'){
		scale(original, mask, vprime, this.centroid, zaxis, ds, amp);
		// addNormal(new THREE.Vector3(0, 0, 0), zaxis, false);
	}
	this.exfab.object.geometry.verticesNeedUpdate = true;
	render();
}	


/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/

function GUINode(container, name, icon, functionality, isimage){
	this.name = name;
	this.icon = icon;
	this.functionality = functionality;

	cl(this.icon);
	var thumb;
	if(isimage)
		thumb = $('<img/>').attr('src', this.icon)
				.css({
					width: '100%',
					margin: 0
				});
	else
		thumb = $('<span><span/>').attr('name', 'swatch')
				.css({
					background: this.icon,
					width: '100%',
					margin: 0,
					padding: 0,
					display: 'block',
					'min-height':  40
				});
			
	var name = $('<span></span>').html(this.name)
			.css({
				width: '90%',
				margin: '5%',
				overflow: 'hidden',
				color: 'black',
				display: 'none',
				'font-size': '6pt'
			});
	var op = $('<div></div>').addClass('op')
			.append(thumb)
			.append(name)
			.addClass('list-button')
			.attr('title', this.name)
			.css({
				height: 45,
				width : '20%',
				overflow: 'hidden',
				margin: '1%',
				// padding: '1%',
				float: 'left',
				display: 'inline-block'
			})
			.click(this.functionality);
	if(isimage) op.addClass('icon-button');
	else op.addClass('swatch-button');
	return op;
}


/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/

var datastreams = {};
function DataStream(data, name, subject, desc){
	this.data = data;
	this.desc = desc;
	this.subject = subject;
	this.name = name;
	var self = this;
	this.saveState = function(){
		var data = scene.userData.objects[0];
		for(var i in data.object.geometry.vertices)
			data.current[i] = data.object.geometry.vertices[i].clone();
 	}
 	this.restoreState = function(){
 		var data = scene.userData.objects[0];
		for(var i in data.original){
			data.object.geometry.vertices[i] = data.original[i].clone();
			data.current[i] = data.original[i].clone();
		}
		data.object.geometry.verticesNeedUpdate = true;
		render();
 	}
	this.applyToOperation = function(val){
		var data = scene.userData.objects[0];
		if(currentOperation != null){
			//cl(data);
			var sr = selectedRegions2(data);
			for(var i in sr) data.regions[sr[i]].transform(this.data, val);
		}
		//if(currentOperation != null)
		//	currentOperation.transform(data, null, this.data, val);
	}
	this.container = $('<span></span>').css( { width: '40%', 'display' : 'inline', 'float' : 'left'});


	this.constructGUI = function(subject, desc, isactive, index){
			var item = $('<div name="ds-'+ index +'" class="mesh-item"></div>')
						.click(function(){
							$('.mesh-item').removeClass('mesh-item-unread');
							$(this).addClass('mesh-item-unread');
							activeDataStream = self;
							activeDataStream.applyToOperation(0.1);
						});
			// var avatar = '<div class="pure-u">\\
			// 		<img class="mesh-avatar" src="'+ avatar +'" height="64" width="64">\\
			// 		</div>';

			var info = $('<div> \
					<p class="mesh-desc">'+ this.name +'</p> \
				</div>').css( { width: '60%', 'display' : 'inline', 'float': 'left'});

			item.append(this.container).append(info).append(clearfix.clone());
			if(isactive) item.removeClass('mesh-item-unread');
			return item;
		}
		datastreams[this.name] = this;
}

DataStream.prototype.add = function(container){
	$(container).append(this.constructGUI(true, 1));
}

DataStream.prototype.load = function() { 
	this.container.sparkline(this.data, {
    	type: 'line'}
    );
};



