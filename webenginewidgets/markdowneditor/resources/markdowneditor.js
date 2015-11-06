var imageAnalyzer;
var remaining;

function pullList(count) {
    var imglist = document.getElementById('imglist');
    while(imglist.length > 0 && count > 0) {
        imglist.removeChild(imglist.childNodes[0]);
        count--;
    }
}

function setStatus(statusString) {
    document.getElementById('status').innerHTML = statusString;
}

function setResult(color, value) {
    if (!isNaN(value)) {
        value += " %";
    }
    document.getElementById(color).innerHTML = value;
}

function addImage(newimg) {
    var imglist = document.getElementById('imglist');
    var curChildren = imglist.childNodes;
    var newline = document.createElement('option');
    newline.innerHTML = newimg.src.substring(newimg.src.lastIndexOf('/')+1);
    newline.value = newimg.src;
    imglist.appendChild(newline);
}

function analyzeImages() {
    var imglist = document.getElementsByTagName('option');
    if (imglist.length > 0) {
        var stringlist = [];
        for(var i = 0; i < imglist.length; i++) {
            stringlist[i] = imglist[i].value;
        }
        if (!imageAnalyzer.busy) {
            remaining = stringlist.length;
            imageAnalyzer.startAnalysis(stringlist);
        } else {
            alert("Processing, please wait until finished.");
        }
    } else {
        alert('No images selected. Click on one or more images to select them for analysis.');
    }
}

function updateProgress(complete, max)
{
    var oldRemaining = remaining;
    remaining = max - complete;
    pullList(oldRemaining - remaining);
    //Prevent results getting messed up if we don't get signals in order
    if (imageAnalyzer.busy) {
        setStatus('Processing (' + complete + ' of ' + max + ' completed)');
        setResult('redval', 'Calculating');
        setResult('greenval', 'Calculating');
        setResult('blueval', 'Calculating');
    }
}

new QWebChannel(qt.webChannelTransport,
    function(channel) {
        // set up connections
        imageAnalyzer = channel.objects.imageAnalyzer;
        imageAnalyzer.redChanged.connect(
            function(value) { setResult('redval', value.toFixed(2)); }
        );
        imageAnalyzer.greenChanged.connect(
            function(value) { setResult('greenval', value.toFixed(2)); }
        );
        imageAnalyzer.blueChanged.connect(
            function(value) { setResult('blueval', value.toFixed(2)); }
        );
        imageAnalyzer.updateProgress.connect(updateProgress);
    }
);
