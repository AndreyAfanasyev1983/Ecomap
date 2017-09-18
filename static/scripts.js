// global "map" variable
var map = null;
var marker = null;

var infowindow = new google.maps.InfoWindow(
  {
    size: new google.maps.Size(150,50)
  });


  function createMarker(latlng, name, html) {
    var contentString = html;
    var marker = new google.maps.Marker({
        position: latlng,
        map: map
        });

    google.maps.event.addListener(marker, 'click', function() {
        infowindow.setContent(contentString);
        infowindow.open(map,marker);
    });

    google.maps.event.trigger(marker, 'click');
    return marker;
}


// // execute when the DOM is fully loaded
$(function() {

    var myOptions = {
        center: {lat: 50.518750, lng: 30.239783}, // Irpen
        mapTypeControl: true,
        mapTypeControlOptions: {style: google.maps.MapTypeControlStyle.DROPDOWN_MENU},
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 40,
        zoom: 13,
        zoomControl: true
    };
  map = new google.maps.Map(document.getElementById("map_canvas"),
                                myOptions);

  google.maps.event.addListener(map, 'click', function() {
        infowindow.close();
        });

  google.maps.event.addListener(map, 'click', function(event) {
	//call function to create marker
         if (marker) {
            marker.setMap(null);
            marker = null;
         }
	 marker = createMarker(event.latLng, "name", "<b>Location : </b><br>"+
	                                             "Latitude " + Number(event.latLng.lat()).toFixed(5) + '<br>' +
	                                             "Longtitude " + Number(event.latLng.lng()).toFixed(5));
	$("#lat").val(Number(event.latLng.lat()).toFixed(5));
    $("#long").val(Number(event.latLng.lng()).toFixed(5));
  });

});

function checkParams() {
    var Lat = $('#lat').val();
    var Long = $('#long').val();
    var name = $('#name').val();
    var problem = $('#problem').val();
    var Contact = $('#contact').val();

    if(Lat.length != 0 && Long.length != 0 && name.length != 0 && problem.length != 0 && Contact.length != 0) {
        $('#submit').removeAttr('disabled');
    } else {
        $('#submit').attr('disabled', 'disabled');
    }
}
