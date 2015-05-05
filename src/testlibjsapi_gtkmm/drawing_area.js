builder.addFromFile('drawing_area.glade');

var window = builder.getWindow('window1');
window.show();

var area = builder.getWidget('drawingarea1');

var img = area.createImageSurface(window.getWidth(), window.getHeight());

for (var i = 0, size = img.width * img.height; i < size; ++i) {
    var ix = i * 4;
    img.data[ix + 3] = 255;
    img.data[ix + 0] = 127;
}

area.putImageData(img, 0, 0);

app.run(window);