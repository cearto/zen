var datastreams = [];
var operations = [];
var clearfix = $('<br/>').addClass('clearfix');
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

colors = [{'default': 0xffc94c, 'active':0xffd87f}, //yellow
			{'default':0x41ccb4, 'active': 0x69d7c4}, // light green
			{'default':0x40c365, 'active': 0x67cf84}, // green
			{'default': 0xDB4090, 'active':0xe36ba9}, // pink
			{'default':0x00a8e1, 'active': 0x62d7ff}, // blue
			{'default':0xd82727, 'active': 0xe05252}, // red
			{'default':0xebb114, 'active': 0xefc143}, // orange
			{'default':0x542c54, 'active': 0x542c54}, // purple
			{'default': 0xffc94c, 'active':0xffd87f}, //yellow
			{'default':0x41ccb4, 'active': 0x69d7c4}, // light green
			{'default':0x40c365, 'active': 0x67cf84}, // green
			{'default': 0xDB4090, 'active':0xe36ba9}, // pink
			{'default':0x00a8e1, 'active': 0x62d7ff}, // blue
			{'default':0xd82727, 'active': 0xe05252}, // red
			{'default':0xebb114, 'active': 0xefc143}, // orange
			{'default':0x542c54, 'active': 0x542c54} // purple
];

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

