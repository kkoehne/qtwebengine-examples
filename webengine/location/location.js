(function() {

    function success(position) {
        console.log("Success: " + position);
        var s = document.querySelector('#status');

        if (s.className === 'success') {
            // not sure why we're hitting this twice in FF, I think it's to do with a cached result coming back
            return;
        }

        s.innerHTML = "found you!";
        s.className = 'success';

        var mapcanvas = document.createElement('div');
        mapcanvas.id = 'mapcanvas';
        mapcanvas.style.height = '400px';
        mapcanvas.style.width = '560px';

        document.querySelector('article').appendChild(mapcanvas);

        var latlng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
        var myOptions = {
            zoom: 15,
            center: latlng,
            mapTypeControl: false,
            navigationControlOptions: {style: google.maps.NavigationControlStyle.SMALL},
            mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        var map = new google.maps.Map(document.getElementById("mapcanvas"), myOptions);

        var marker = new google.maps.Marker({
                                                position: latlng,
                                                map: map,
                                                title:"You are here! (at least within a "+position.coords.accuracy+" meter radius)"
                                            });
    }

    function error(msg) {
        console.log("error: " + msg);
        var s = document.querySelector('#location-status');
        s.innerHTML = typeof msg == 'string' ? msg : "failed";
        s.className = 'fail';
    }

    navigator.geolocation.getCurrentPosition(success, error);
})();
