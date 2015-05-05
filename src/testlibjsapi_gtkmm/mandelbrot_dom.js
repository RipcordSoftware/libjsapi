builder.addFromFile('mandelbrot.glade');
$('window1').show();
$('window2').show();

var old_getElementById = document.getElementById;

document.getElementById = function(id) {
    if (id == "canvasMandelbrot") {
        return {
            getContext: function() {
                return { 
                    createImageData : function(width, height) {
                        return { data: new Array(width * height * 4) };
                    },
                    putImageData : function(img, x, y) {
                        
                    }
                }
            }
        }
    } else if (id == "canvasControls") {
        return {};
    } else {
        var element = old_getElementById(id);
        if (!element) {
            trace('getElementById() missed on: ' + id);
            element = {};
        }
        
        return element;
    }
}

$ = document.getElementById;

var window = {
    innerWidth: $('window1').getWidth(),
    innerHeight: $('window1').getHeight()
}

var location = { hash: '' };