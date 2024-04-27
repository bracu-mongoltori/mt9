var CONFIG_ROS_server_URI = 'localhost';

var ros = new ROSLIB.Ros({
	url : 'ws://'+ CONFIG_ROS_server_URI +':9090'
});

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

function rotateArrow(angle) {
    // const images = document.getElementsByTagName("img");
    // var arrow = "";

    // for (let i = 0; i < images.length; i++) {
    //     if (images[i].src == "file:///home/ayat/sbg_ROS_leaflet_gps/assets/forward.png") {
    //         arrow = images[i];
    //     }
        
    // }
    // if (arrow) {
    //     arrow.style.transform = `rotate(${angle}deg)`;
	//     if (angle >= 360) {
	// 	    angle = 0; // Reset angle to 0 after 360 degrees
	//     }   
    // }
	
}

quaternion_topic.subscribe((message) => {

	var euler = euler_from_quaternion(message.quaternion.x, message.quaternion.y, message.quaternion.z, message.quaternion.w);
	var yaw = euler[2]; //yaw is returned in radians
	
    rotateArrow( (yaw >= 0) ? yaw * (180 / Math.PI) : 360 + yaw * (180 / Math.PI) );
	//console.log( (yaw >= 0) ? yaw * (180 / Math.PI) : 360 + yaw * (180 / Math.PI) );
})



