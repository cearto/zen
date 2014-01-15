var datastreams = [];
var operations = [];
var clearfix = $('<br/>').addClass('clearfix');

function MeshGUI(){
	this.canvas = $('canvas')
	this.message = $('#error');
	this.messageText = $('#error h2');
	this.messageSubText = $('#error-msg');
	this.message.hide();
	this.message.children('.x').click(function(){
		$(this).parent().fadeOut();
	});
	this.gallery = new Gallery();
	this.generateImage = function(){
		var img    = this.canvas[0].toDataURL("image/png");
		this.gallery.add(img);
	}
}

function Gallery(){
	this.container = $('#gallery');
	this.wrapper = $('#gallery-wrapper');
}
Gallery.prototype.add = function(img, box){
	var node = $('<div></div>').addClass('gallery-node');
	cl(box);
	var nodeImg = $('<img/>').attr('src', img).css({
		width: '100%',
		margin: "-50px 0 0 -10px"
	});
	
	// var canvas = $('<div></div>');
	// var context = canvas.getContext('2d');
	// var imageObj = new Image();
	// imageObj.onload = function() {

	// 	// draw cropped image
	// 	var sourceX = 0;
	// 	var sourceY = 0;
	// 	var sourceWidth = WIDTH;
	// 	var sourceHeight = HEIGHT;
	// 	if(typeof box !== "undefined"){
	// 		cl(box);
	// 		sourceWidth =  box.size().x;
	// 		sourceHeight = box.size().y;
	// 		sourceX = (WIDTH/2) - box.min.x;
	// 		sourceY = (HEIGHT/2) - box.min.y;
	// 		cl(sourceWidth + " " + sourceHeight);
	// 		sourceWidth = 200;
	// 		sourceHeight = 200;
	// 		sourceX = 200;
	// 		sourceY = 200;
	// 	}
	// 	var destWidth = sourceWidth;
	// 	var destHeight = sourceHeight;
	// 	var destX = node.width() / 2 - destWidth / 2;
	// 	var destY = node.height() / 2 - destHeight / 2;
	// 	destX = 0;
	// 	destY = 0;
	// 	destWidth = node.width();
	// 	destHeight = node.height();
	// 	console.log(sourceX, sourceY, sourceWidth, sourceHeight, destX, destY, destWidth, destHeight)
	

	// 	context.drawImage(imageObj, sourceX, sourceY, sourceWidth, sourceHeight, destX, destY, destWidth, destHeight);
	// };
	// imageObj.src = img;
    node.append(nodeImg);
	this.container.append(node);
}
MeshGUI.prototype.displayUIMessage = function(msg, smsg, persist){
	this.messageText.html(msg);
	this.messageSubText.html(smsg);
	if(persist)	this.message.show().delay(2000);//.fadeOut();
	else	this.message.show().delay(2000).fadeOut();
}

function GUIFile(fileinfo){
	var bar = $('#info-bar .pure-u-1-2');
	bar.children('.mesh-content-title').html(fileinfo['name']);
	
	var subtitle = "From <a>"+ fileinfo['author'] +"</a> at <span>"+ fileinfo['mtime'] +", "+ fileinfo['date'] + "</span>";
	bar.children('.mesh-content-subtitle').html(subtitle);
	GUIFileNav(fileinfo.name);
	//GUIMatSelect();
}
function GUIStreams(){
	
	var heading = $('<h3></h3>').html('Streams').addClass('ui-heading');
	var container = $('<div></div>').attr('id', 'streams').append(heading);
	var data = scene.userData.objects;
	$('#nav').append(container);
	render();
}
var dna;
function GUIMatSelect( currentFile ){
	var heading = $('<h3></h3>').html('View').addClass('ui-heading');
	var container = $('<div></div>').attr('id', 'matview').append(heading);
	var n = parseInt(getURLParameter('n'));
	
	var wireMat = $('<a></a>').html('Wireframe').attr('href', 'segment?w=1&n=' + (n)).addClass('ui-link');
	var solidMat = $('<a></a>').html('Solid').attr('href', 'segment?w=0&n=' + (n)).addClass('ui-link');

	if(getURLParameter('w') == 1)
		wireMat.addClass('selected-mat');
	else
		solidMat.addClass('selected-mat');
	container.append(wireMat).append(solidMat);
	container.append(clearfix.clone());
	$('#nav').append(container);
}
function GUIFileNav( currentFile ){
	var heading = $('<h3></h3>').html('Object View').addClass('ui-heading');
	var container = $('<div></div>').attr('id', 'objnav').append(heading);
	var n = parseInt(getURLParameter('n'));

	var linkPrev = $('<a></a>').html('&larr;').attr('href', 'segment?n=' + (n - 1));
	var linkNext = $('<a></a>').html('&rarr;').attr('href', 'segment?n=' + (n + 1));
	if(n == 0) linkPrev.attr('disable', true);
	if(n > 280) linkNext.attr('disable', true);

	var fileName = $('<span></span>').addClass('ui-span');
	file = $('<a></a>').html(currentFile).addClass('filename');
	fileName.html(file);
	container.append(linkPrev).append(fileName).append(linkNext).append('<h3></h3>');
	var wireMat = $('<a></a>').html('Wireframe').attr('href', 'segment?w=1&n=' + (n)).addClass('ui-link');
	var solidMat = $('<a></a>').html('Solid').attr('href', 'segment?w=0&n=' + (n)).addClass('ui-link');
	dna = new DNA();
	var generator = $('<a></a>').html('Generate').click(function(){
		dna.generate();
	}).addClass('ui-link');
	
	if(getURLParameter('w') == 1)
		wireMat.addClass('selected-mat');
	else
		solidMat.addClass('selected-mat');
	container.append(wireMat).append(solidMat).append(generator);
	container.append(clearfix.clone());
	$('#nav').append(container);
}
function GUIRegions(exfab){
	var heading = $('<h3></h3>').html('Regions').addClass('ui-heading');
	var container = $('<div></div>').attr('id', 'regions').append(heading);
	for(var i in exfab.regions) exfab.regions[i].addGUI(container);
	container.append(clearfix.clone());
	$('#objnav').after(container);
	render();
}
function GUIOperations(){
	var heading = $('<h3></h3>').html('Operations').addClass('ui-heading');
	var opContainer = $('<div></div>').attr('id', 'operations').append(heading);
	for(var i in operationData)
		operations.push(new Operation(opContainer, operationData[i]['name'], operationData[i]['img']));
	opContainer.append(clearfix);
	$('#nav').append(opContainer);
}
function loadDataStreams(container){
	var u = new DataStream([0.2, 0.2, 0.2, 0.2, 0.2], "Uniform", "Uniform random var", "A uniform data stream.");
	u.add(container);
	u.load();
	datastreams.push(u);

	var ds = new DataStream([5,6,7,9,9,5,3,2,2,4,6,7], "Test", "Random", "A random data stream.");
	ds.add(container);
	datastreams.push(ds);
	ds.load();
	
}



// 
// 

function GUIActivateListeners(){
	$('.slider input[name="zoom"]').change(function(e){
			zoomLevel = $(this).val();
			$(this).siblings('span').children().html(z);
			updateView(Z, theta, phi, zoomLevel);
		});

		$('.slider input[name="rotate-x"]').change(function(e){
			$(this).siblings('span').children().html($(this).val());
			theta = parseFloat($(this).val());
			updateView(Z, theta, phi, zoomLevel);
		});
		
		// $(".list-button").click(toggleList);
		$('.mesh-content-header').delay(500).animate({top: -120}).addClass('up');
		
		$('#pulldown').click(function(){
			var header = $(".mesh-content-header");
			if(header.hasClass('up')){
				header.animate({top: -20}).removeClass('up');
				//$(this).html('&uarr;');
				$(this).children('img').addClass('setting-on');
			}
			else{
				header.animate({top: -120}).addClass('up');
				$(this).children('img').removeClass('setting-on');
				//$(this).html('&darr;');
			}
		});
		$('#revert').click(function(){
			activeDataStream.restoreState();
		});
		$('#save').click(saveFile);

		// DIRECT MANIPULATION HANDLERS
		$('canvas').mousemove(onDocumentOver);
		$('canvas').click(onDocumentMouseClick);
		$('canvas').mouseup(onDocumentMouseUp);
		$('canvas').mousedown(onDocumentMouseDown);
		$('#print').click(clean);
		$('#screenshot').click(function(){
			mainExpFab.mgui.generateImage();
		});
		
		$('#fullscreen').click(function(){
			var gal = mainExpFab.mgui.gallery.wrapper;
			if(gal.hasClass('gallery-sidebar')){
				gal.removeClass('gallery-sidebar').addClass('gallery-fullscreen');
				$(this).html('Sidebar');
			}
			else{
				mainExpFab.mgui.gallery.wrapper.removeClass('gallery-fullscreen').addClass('gallery-sidebar');
				$(this).html('Fullscreen');
			}
		});
}

/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/

function GUINode(container, name, icon, functionality, isimage){
	this.name = name;
	this.icon = icon;
	this.functionality = functionality;

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
			.attr('name', this.name)
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

/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/




function convertColors(){
	for(var i in colors)
		for(var state in colors[i])
			colors[i][state] = new THREE.Color(colors[i][state]);
}

function makeListItem(avatar, name, subject, desc, isactive, index){
	var item = $('<div name="materal-'+ index +'" class="mesh-item pure-g"><div class="pure-u"><img class="mesh-avatar" src="'+ avatar +'" height="64" width="64"></div><div class="pure-u-3-4"><h5 class="mesh-name">'+ name +'</h5> <h4 class="mesh-subject">'+ subject +'</h4><p class="mesh-desc">'+ desc +'</p></div></div>')
				.click(function(){
					scene.children[1].material = materials[index];
					$('.mesh-item').removeClass('mesh-item-unread');
					$(this).addClass('mesh-item-unread');
				});
	if(isactive) item.removeClass('mesh-item-unread');
	$('#list').append(item);
	$('#list').append(item);	
}

function toggleList(){
	$('#list').toggleClass('active');
	for(var i in datastreams) datastreams[i].load();
};

function clean(){
	$('#streams').hide();
	$('#regions').hide();
}

