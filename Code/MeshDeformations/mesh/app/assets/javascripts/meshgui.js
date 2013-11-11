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
};

function updateView(R, theta, phi, zoomFactor){
	// scene.children[2].rotation.y = theta;
	camera.fov = 300 *  zoomFactor;
		camera.updateProjectionMatrix();
	render();
}


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
		
		$(".list-button").click(toggleList);
		makeListItem('', 'Lambertian', "Shiny", "Red", true, 0);
		makeListItem('', 'Basic Material', "Wireframe", "Lines that show outline.", false, 1);
		makeListItem('', 'Phongy', "Blue specular lambertian. ", "Blue and clean.", false, 2);
		
		$('.mesh-content-header').delay(500).animate({top: -90}).addClass('up');
		
		$('#pulldown').click(function(){
			var header = $(".mesh-content-header");
			if(header.hasClass('up')){
				header.animate({top: 0}).removeClass('up');
				$(this).html('&uarr;');
			}
			else{
				header.animate({top: -90}).addClass('up');
				$(this).html('&darr;');
			}
		});
}