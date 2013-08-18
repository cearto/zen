function GML(){
    var components = {};
    var features = {'Cube' : ['height', 'width', 'depth']}

    /* Accessor functions */
    this.addComponent = function(voxel){components[voxel.name] = voxel;}
    this.getComponents = function(){ return components;}
    this.getComponent = function(name){ return components[name];}
    this.removeComponent = function(name){ delete components[name];}
    this.getNumComponents = function(){ return Object.size(components);}
    this.removeComponents = function(){ return components = {};}

    this.loadJSON = function(data){
        console.log(data);
        var elem, geom;
        for(var i in data){
            geom = new THREE.CubeGeometry(data[i].geometry.width, data[i].geometry.height, data[i].geometry.depth );
            elem = new Voxel(data[i].position, geom, data[i].intersect, data[i].type);
            console.log(elem);
            this.addComponent(elem);
        }
    }
    this.toJSON = function(pretty){
        structure = []

        for(var i in components){
            intersect = new Object();
            intersect.object = new Object();
            intersect.object.name = components[i].attachedTo;
            intersect.object.faceIndex = components[i].attachedToFace;

            var elem = {
            'name': components[i].name,
            'type': components[i].type,
            'geometry': components[i].geometry,
            'intersect': intersect,
            'position' :components[i].position,
            }

           // for(var j in components[i].variables)
           //      elem[components[i].variables[j]] = components[i].geometry[components[i].variables[j]];

            structure.push(elem)
        }
        if(pretty)
            return JSON.stringify(structure, null, "\t"); 
        else   
            return JSON.stringify(structure); 
    }
}
function setHex(color, a){a=Math.floor(a);color.r=(a>>16&255)/255;color.g=(a>>8&255)/255;color.b=(a&255)/255;return color}

function Voxel(position, geometry, intersect, type){
    this.type = type;
    this.geometry = geometry;
    this.position = position;
    this.variables = ['width', 'height', 'depth'];
    this.attachedTo = [intersect.object.name]
    this.attachedToFace = [intersect.object.faceIndex]
    

    /* Color faces */
    for ( var i = 0; i < geometry.faces.length; i ++ )
    // setHex(geometry.faces[ i ].color, 0x00A8e1 );
       geometry.faces[ i ].color.setHex( 0x00A8e1 );
      // geometry.faces[ i ].color.setHex( 0x00A8e1 );
    
    /* Apply matte material */
    var material = new THREE.MeshLambertMaterial( { vertexColors: THREE.FaceColors } );
    this.mesh = new THREE.Mesh( geometry, material );

    /* Snap to grid */ 
    this.mesh.position.x = Math.floor( position.x / 50 ) * 50 + 25;
    this.mesh.position.y = Math.floor( position.y / 50 ) * 50 + 25;
    this.mesh.position.z = Math.floor( position.z / 50 ) * 50 + 25;
    this.mesh.matrixAutoUpdate = false;
    this.mesh.updateMatrix();

    /* Meta data */
    this.name = "Cube " + gml.getNumComponents();
    this.mesh.name = this.name;
    this.geometry.name = this.mesh.name;
}


 