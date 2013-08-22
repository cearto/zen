
var distChoices = ['<img src="/assets/uniform.png" class="dist"/> Uniform','<img src="/assets/normal.png" class="dist"/> Normal'];
var domainChoices = ['Continuous', 'Discrete', 'List'];

function GUI(){}
GUI.loadCanvas = function(){
  if(! $('#viewport-panel.tab-pane').hasClass('active')){
    $('.tab-pane').removeClass('active');
    $('#viewport-panel.tab-pane').addClass('active');
  }
}

GUI.loadJSON = function(){
  console.log(gml.toJSON());
  $('#tab2.tab-pane .well').html('JS' + gml.toJSON());
  if(! $('#tab2.tab-pane').hasClass('active')){
    $('.tab-pane').removeClass('active');
    $('#tab2.tab-pane').addClass('active');
  }
}

GUI.makeFeatureRow = function(name){
  var row = $('<tr></tr>');
  var name = $('<td></td>').html(name);
  var domain = GUI.makeDropDownBtn(domainChoices, 0);
  var dist = GUI.makeDropDownBtn(distChoices, 0);
  row.append(name).append(domain).append(dist);
  $('#feature-list table').append(row);
}


/* Makes a bootstrap dropdown button of choices, def is index to default */
GUI.makeDropDownBtn = function(choices, def){
  var feature = $('<td> </td>');
  var group = $('<div></div>').addClass("btn-group btn-xs");
  var btn = $('<button/>').attr('type', 'button')
                       .attr('data-toggle', 'dropdown')
                       .addClass('btn btn-xs btn-default dropdown-toggle')
                       .html(choices[def] + '<span class="caret"></span>');
  group.append(btn);
  var dropdown = $('<ul class="dropdown-menu"></ul>');
    for(var i in choices){
      var choice = $('<li><a href="#"> '+ choices[i] +'</a></li>');
      dropdown.append(choice);
    }
    group.append(dropdown);
    feature.append(group);
    return feature;
}

GUI.addComponent = function(voxel){
   // GUI
   debug = voxel.name + " " +  voxel.attachedTo;
   name = voxel.name
       var comp = $('<div class="input-group input-group-sm"><span class="input-group-addon"><img src="/assets/voxel2.png"/></span> <input type="text" class="form-control" placeholder="'+ debug +'"></div>')
            .attr('name', name)
            .click(function(){
                $('#feature-list table').html('');
                var vx = gml.getComponent($(this).attr('name'));
                for(var i in vx.variables)
                  GUI.makeFeatureRow(vx.variables[i]);
            });
    $('#component-list').append(comp)
}
GUI.removeComponent = function(voxel){
 console.log(voxel);
   console.log(voxel.name);

  var name = voxel.name;
  $('#component-list div[name="'+ name +'"]').remove();
}

GUI.removeComponents = function(){
  $('#component-list').html('');
}

GUI.loadOpenMenu = function(){
  $('#overlay').show();
  $('#popup').show();
  $('#saved_files').children().not('.active').remove();
    JSONLoader.updateFileCount(function(data){
    var filename, node;
    console.log(data);
    numFiles = data.length;
    for(var i in data){
      filename = data[i].split('/')[1].split('.')[0];
      console.log(filename);
      node = $('<a></a>').addClass('list-group-item')
                        .html(filename)
                        .attr('name', data[i])
                        .click(openMenuClick);
      $('#saved_files').append(node);
    }
  });
    

}
GUI.alert = function(remark, msg){
  $('.alert span').html('<strong>' + remark + '</strong> ' + msg);
  $('.alert').show().delay(1000).fadeOut();
}

GUI.generateCanvas = function(){
  var num = 5;
  var container = $('#preview-panel');
  for(var i = 0; i < num; i++){
    var c = $('<canvas>').addClass('preview');
    container.append(c);
  }
}
/* GENERATIVE GUI HANDLER */
GUI.makeRatingMenu = function(container, image, data, reload, rating, id){
  if(typeof(reload) == "undefined") reload = true;
  console.log(rating);
  var overlay = $('<div></div>')
            .click(function(){
                var p = $(this).parent();
                var rating = p[0].getAttribute('data-rating');

              // toggle class on click
              if(rating == 0) 
               p[0].setAttribute('data-rating', 1);
                else
               p[0].setAttribute('data-rating', 0);

               $(this).toggleClass('good');

            });
  if(rating == 1) overlay.addClass('good');
  var c = $('<div></div>')
            .addClass('sample')
            .attr('data-fv', data.toString())
            .append(image)
            .attr('data-rating', rating)
            .append(overlay)
            .attr('data-id', id);
  
  if(reload){
    var reload = $('<button>').html('Reload').click(function(){
          ojc.rebuildSolid(false, data);
        });
    c.append(reload);
  }
  container.append(c);
}
GUI.extractData = function(sample){
  sample = $(sample);
  //console.log(sample);
  var rating = parseInt(sample.attr('data-rating'));
  var data = sample.attr('data-fv');
  var id = parseInt(sample.attr('data-id'));
  data = JSON.parse("[" + data + "]");
  
  var img = sample.children('img').attr('src');
  return {'id': id, 'fv': data, 'rating': rating, 'img': img}
}