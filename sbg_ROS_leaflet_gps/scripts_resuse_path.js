
// The name of the GPS publisher name by default
var CONFIG_default_gps_topic_name = '/sbg/gps_pos'; // '/kitti/oxts/gps/fix';

var CONFIG_default_pose_topic_name = '/sbg/gps_pose' //'/loop_fusion_node/pose_graph_path';

// The number of cycles between every marker position reload
var CONFIG_cycles_number = 1;

// If the input camera pose frame coordinate system is z_up or z_forward
var z_up = true;


var CONFIG_tile_source = 'server';

// If you use local tiles, set here the path to it
var CONFIG_tile_local_path = 'assets/output_2.png';

// Network address to ROS server (it can be localhost or an IP)
var CONFIG_ROS_server_URI = 'localhost';

//===> Global variables
var map;
var selectionMode;
var carIcon = L.icon({
    iconUrl: 'assets/car1.png',
    iconSize: [15, 15], //[22, 22],
    // iconAnchor: [10, 20],
    // popupAnchor: [0, 0],
});
var dirIcon = L.icon({
    iconUrl: 'assets/forward.png',
    iconSize: [30, 30],
    iconAnchor: [10, 20],
    popupAnchor: [0, 0],
	rotationAngle : 90,
});
 
var dirIcon2 = L.icon({
    iconUrl: 'assets/backward.png',
    iconSize: [30, 30],
    iconAnchor: [10, 20],
    popupAnchor: [0, 0],
	rotationAngle : 0
});


var markerPosition = L.marker([0,0], {icon: carIcon});
var markerPosition2 = L.marker([2, 2], { icon: dirIcon,})


var rotateVector = function(vec, ang)
{
    // ang = -ang * (Math.PI/180);
    var cos = Math.cos(ang);
    var sin = Math.sin(ang);
    return new Array(Math.round(10000*(vec[0] * cos - vec[1] * sin))/10000, Math.round(10000*(vec[0] * sin + vec[1] * cos))/10000);
};




// ===> mapInit() : init the map
function mapInit() {
	
	//===> Var init

	// Fetch tiles
	if(CONFIG_tile_source == 'local')
		var tileUrl = CONFIG_tile_local_path;
	if(CONFIG_tile_source == 'server')
		var tileUrl = 'http://{s}.tile.osm.org/{z}/{x}/{y}.png';

	// Set attrib (always !)
	var attrib = 'Map data © OpenStreetMap contributors'; 

	//===> Map loading
	map = L.map('map');
	var osm = L.tileLayer(tileUrl, {
		minZoom: 11, 
		maxZoom: 19,
		attribution: attrib,
		noWrap: true
	}); 
	osm.addTo(map);


	// map.addControl(new L.Control.LinearMeasurement({
	// 	unitSystem: 'metric',
	// 	color: '#FF0080'
	// }));

	L.control.scale ({maxWidth:240, metric:true, imperial:false, position: 'bottomleft'}).addTo (map);
	let polylineMeasure = L.control.polylineMeasure ({
							position:'topleft', unit:'metres', showBearings:false, 
							clearMeasurementsOnStop: false, showClearControl: true})
	polylineMeasure.addTo (map);

	L.easyButton('glyphicon glyphicon-refresh', function(btn, map){
		window.location.reload();
		loadedMap = false;
        loadedMap2 = false;
	}).addTo(map);

	return map;
}

// ============================= SCRIPT
var bounds;
var zoomLevel = 19;
var loadedMap = false;
var loadedMap2 = false;
var i = 0;
var listenerGPS;
var listenerPose;


//===> ROS connexion
var ros = new ROSLIB.Ros({
	url : 'ws://'+ CONFIG_ROS_server_URI +':9090'
});

swal({
	title: "Connecting to ROS...",
	showConfirmButton: false,
	closeOnConfirm: false,
	showLoaderOnConfirm: true,
	allowOutsideClick: false,
	allowEscapeKey: false
});

ros.on('connection', function() {
	console.log('Connected to websocket server.');
	swal({
		title: "Waiting...",
		text: "The navigation module can't work without the GPS. Launch the GPS and the module will start automatically.",
		type: "info",
		confirmButtonText: "Reload",
		closeOnConfirm: false,
		allowOutsideClick: false,
		allowEscapeKey: false
	},
	function(){
		window.location.reload();
	});
});

ros.on('error', function(error) {
	console.log('Error connecting to websocket server: ', error);
	swal({
		title: "Error connecting the ROS server",
		text: "Unable to reach ROS server. Is rosbridge launched ?",
		type: "error",
		confirmButtonText: "Retry",
		closeOnConfirm: false,
		allowOutsideClick: false,
		allowEscapeKey: false
	},
	function(){
		window.location.reload();
	});
});

ros.on('close', function() {
	console.log("Connexion closed.");
	swal({
		title: "Error connecting the ROS server",
		text: "Unable to reach ROS server. Is rosbridge launched ?",
		type: "error",
		confirmButtonText: "Retry",
		closeOnConfirm: false,
		allowOutsideClick: false,
		allowEscapeKey: false
	},
	function(){
		window.location.reload();
	});
});

//===> Init the map
mapInit();
console.log(map)


// var input_coordinates = [0.001, 0.001];
// dest = new L.marker({lat : input_coordinates[0], lng : input_coordinates[1]})
// dest.addTo(map);


//===> Set the GPS listener
//  => Create param with initial value
var paramTopicNameValue = CONFIG_default_gps_topic_name;
var paramTopicNamePose = CONFIG_default_pose_topic_name;
var paramNbCyclesValue = CONFIG_cycles_number;

//  => Init the ROS param
var paramTopicName = new ROSLIB.Param({ros : ros, name : '/panel/gps_topic'});
var paramTopicPose_Name = new ROSLIB.Param({ros : ros, name : '/panel/pose_topic'});
var paramNbCycles = new ROSLIB.Param({ros : ros, name : '/panel/nb_cycles'});
// z_up = new ROSLIB.Param({ros : ros, name : 'z_up'});
console.log("z_up?: ", z_up);
var path_ = [];
// var firstloc;
var polyline_;
var polyline2_;
var quaternion_;
var translation_;

var quaternion0;
var translation0;
var imu02enu = new THREE.Matrix4();
var scale = new THREE.Vector3 (1, 1, 1);
var quaternion_cam2imu = new THREE.Quaternion( 0.5, -0.5, 0.5, -0.5 );
var quaternion_imu2cam = new THREE.Quaternion( 0.5, -0.5, 0.5, 0.5 );
var cam2imu = new THREE.Matrix4();
cam2imu.makeRotationFromQuaternion(quaternion_cam2imu);

//  => Set the value

var bearingTopic = new ROSLIB.Param({ros : ros, name : '/panel/gps_topic'});

var yaw = 0;

function euler_from_quaternion(x, y, z, w){
        
	// Convert a quaternion into euler angles (roll, pitch, yaw)
	// roll is rotation around x in radians (counterclockwise)
	// pitch is rotation around y in radians (counterclockwise)
	// yaw is rotation around z in radians (counterclockwise)
	
	var t0 = +2.0 * (w * x + y * z)
	var t1 = +1.0 - 2.0 * (x * x + y * y)
	var roll_x = Math.atan2(t0, t1)
 
	var t2 = +2.0 * (w * y - z * x)
	var t2 = (t2 > +1.0) ? +1.0 : t2;   // +1.0 if t2 > +1.0 else t2
	var t2 = (t2 < -1.0) ? -1.0 : t2    //-1.0 if t2 < -1.0 else t2
	var pitch_y = Math.asin(t2)
 
	var t3 = +2.0 * (w * z + x * y)
	var t4 = +1.0 - 2.0 * (y * y + z * z)
	var yaw_z = Math.atan2(t3, t4)
 
	return [roll_x, pitch_y, yaw_z] // in radians
}

var quaternion_topic = new ROSLIB.Topic({
		ros : ros,
		name : "/sbg/ekf_quat",
		messageType : 'sbg_driver/SbgEkfQuat'
	});

quaternion_topic.subscribe((message) => {

	var euler = euler_from_quaternion(message.quaternion.x, message.quaternion.y, message.quaternion.z, message.quaternion.w);
	yaw = euler[2]; //yaw is returned in radians
	yaw = yaw * (180 / Math.PI)
	//yaw = (yaw >= 0) ? yaw * (180 / Math.PI) : 360 + yaw * (180 / Math.PI);
	
	//rotateArrow( (yaw >= 0) ? yaw * (180 / Math.PI) : 360 + yaw * (180 / Math.PI) );
	//console.log( (yaw >= 0) ? yaw * (180 / Math.PI) : 360 + yaw * (180 / Math.PI) );
})

paramTopicName.get(function(value) { 
	// If the param isn't created yet, we keep the default value
	if(value != null) 
		paramTopicNameValue = value; 
	else
		paramTopicName.set(paramTopicNameValue);
	
	paramNbCycles.get(function(value) {
		// If the param isn't created yet, we keep the default value
		if(value != null) 
			paramNbCyclesValue = value; 
		else
		paramNbCycles.set(paramNbCyclesValue);


		// Set the listener information
		listenerGPS = new ROSLIB.Topic({
			ros : ros,
			name : paramTopicNameValue,
			// messageType : 'sensor_msgs/NavSatFix'
			messageType : 'sbg_driver/SbgGpsPos'
		});

		// Set the callback function when a message from /gps is received

		var i1 = 0;
		var utm0;

		
marker = null;
listenerGPS.subscribe(function(message) {

    var x_ = 1//message.pose.pose.position.x;
    var y_ = 1//message.pose.pose.position.y;

    quaternion_ = new THREE.Quaternion(
        1,//message.pose.pose.orientation.x,
        1,//message.pose.pose.orientation.y,
        1,//message.pose.pose.orientation.z,
        1,//message.pose.pose.orientation.w
    );
    //translation_ = new THREE.Vector3(x_, y_, message.pose.pose.position.z);

    if (loadedMap == false) {
        swal.close();
        utm0 = L.utm(x_, y_, 48, 'N', false);
        var ll0 = {lat: 23.780821, lng: 90.407864}//{lat : message.latitude, lng : message.longitude};
        if (ll0) {
            map.setView(ll0, zoomLevel);
        }
        markerPosition.addTo(map);

        markerPosition2.addTo(map);
        //translation0 = translation_;
        quaternion0 = quaternion_;
      //  imu02enu.compose(translation0, quaternion0, scale);
        path_.push(ll0);
        polyline_ = L.polyline(path_, { color: 'red' }, { weight: 1 }).addTo(map);
        loadedMap = true;
    }


	if (i1 % paramNbCyclesValue == 0) {
		utm0 = L.utm(x_, y_, 48, 'S','N' ,false);
		ll0 = {lat: 23.780821, lng: 90.407864}//{lat : message.latitude, lng : message.longitude};//utm0.latLng();
		if (ll0) {
		// 	// Calculate the movement direction based on orientation
		// 	var movementDirection = Math.atan2(translation_.y - translation0.y, translation_.x - translation0.x);
	
		// 	// Calculate new latitude and longitude based on movement direction
		// 	var stepDistance = 0.0001; // Adjust this value based on the step distance you want
		// 	var newLat = ll0.lat + stepDistance * Math.sin(movementDirection);
		// 	var newLng = ll0.lng + stepDistance * Math.cos(movementDirection);
		// 	var rotationAngle = movementDirection * (180 / Math.PI);

        // // Rotate dirIcon based on the calculated rotation angle
        // dirIcon.options.rotationAngle = rotationAngle;
        

	
		// 	// Update the position of markerPosition2 based on the calculated latitude and longitude
		// 	var newLatLng = L.latLng(newLat, newLng);
		// 	markerPosition2.setLatLng(newLatLng);
	
			// Update the position of markerPosition
			markerPosition.setLatLng(ll0);

			var iconOptions = {
				iconUrl : "assets/backward_transparent.png",
				iconSize : [50, 50],
			}
			
			var customIcon = L.icon(iconOptions);
			
			var markerOptions = {
				icon : customIcon,
				rotationAngle : yaw,
			   // rotationOrigin : {lat : ll0.lat , lng : ll0.lng},//[15, 15],
				//iconAnchor: ll0, //[15, 15],
			}

			if (marker != null) {
				map.removeLayer(marker)
			}
			
			// var dot = L.circle({lat : ll0.lat + 0.0001, lng : ll0.lng}, {
			// 	radius: 2, // Adjust the radius as needed
			// 	color: 'red', // Set the dot color
			// 	fill: true,
			// 	fillOpacity: 1,
			//   }).addTo(map);

			marker = new L.marker({lat : ll0.lat + 0.00004, lng : ll0.lng}, markerOptions)
			marker.addTo(map);

	
			
			//markerPosition2.setLatLng(ll0);


			polyline_.addLatLng(ll0);

			
		}
		document.getElementById("lat").innerHTML = "Latitude: " + ll0.lat;
		document.getElementById("long").innerHTML = "Longitude: " + ll0.lng;
	}
	
    i1++;
});


	});
});

// var x_ = 1//message.pose.pose.position.x;
// var y_ = 1//message.pose.pose.position.y;

// quaternion_ = new THREE.Quaternion(
// 	1,//message.pose.pose.orientation.x,
// 	1,//message.pose.pose.orientation.y,
// 	1,//message.pose.pose.orientation.z,
// 	1,//message.pose.pose.orientation.w
// );
// //translation_ = new THREE.Vector3(x_, y_, message.pose.pose.position.z);

// if (loadedMap == false) {
// 	swal.close();
// 	utm0 = L.utm(x_, y_, 48, 'N', false);
// 	var ll0 = {lat: 23.780821, lng: 90.407864}//{lat : message.latitude, lng : message.longitude};
// 	if (ll0) {
// 		map.setView(ll0, zoomLevel);
// 	}
// 	markerPosition.addTo(map);

// 	markerPosition2.addTo(map);
// 	//translation0 = translation_;
// 	quaternion0 = quaternion_;
// 	//  imu02enu.compose(translation0, quaternion0, scale);
// 	path_.push(ll0);
// 	polyline_ = L.polyline(path_, { color: 'red' }, { weight: 1 }).addTo(map);
// 	loadedMap = true;
// }

// var iconOptions = {
// 	iconUrl : "assets/backward_transparent.png",
// 	iconSize : [50, 50],
// }

// var customIcon = L.icon(iconOptions);
// yaw = 45;
// var markerOptions = {
// 				icon : customIcon,
// 				rotationAngle : yaw,
// 				// rotationOrigin : {lat : ll0.lat , lng : ll0.lng},//[15, 15],
// 				//iconAnchor: ll0, //[15, 15],
// 			}


// // var dot = L.circle({lat : ll0.lat + 0.0001, lng : ll0.lng}, {
// // 	radius: 2, // Adjust the radius as needed
// // 	color: 'red', // Set the dot color
// // 	fill: true,
// // 	fillOpacity: 1,
// //   }).addTo(map);
// markerPosition.addTo(map);
// markerPosition.setLatLng(ll0);

// marker = new L.marker({lat : ll0.lat + 0.00004, lng : ll0.lng}, markerOptions)
// marker.addTo(map);
	
