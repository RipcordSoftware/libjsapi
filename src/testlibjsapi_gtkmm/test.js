builder.addFromFile('test.glade');

var msgs = [
    'you suck!!!',
    'get outa here!!!',
    'roger, over and out',
    'dang....',
    'zooooooooooooooooom....',
    'you are now under my control!',
    'buy more stuff!'
];

var label = $('label1');
var button = $('button1');
var entry = $('entry1');
if (label && button && entry) { 
    button.onClick(function(){ 
        var i = Math.random() * msgs.length;
        var msg = msgs[Math.floor(i)];
        label.setText(msg);
        entry.setText(msg);
    }); 
}

var checkButton = $('checkbutton1');
if (checkButton) {
    checkButton.onClick(function() {
       if (entry) {
           checkButton.getActive() ? entry.hide() : entry.show();
       }
    });
}

$('window2').show();
$('drawingarea1').onDraw(function(area, width, height) {
    area.setLineWidth(10.0);
    
    var xc = width / 2;
    var yc = height / 2;

    // draw red lines out from the center of the window
    area.setSourceRgb(0.8, 0.0, 0.0);
    area.moveTo(0, 0);
    area.lineTo(xc, yc);
    area.lineTo(0, height);
    area.moveTo(xc, yc);
    area.lineTo(width, yc);
    area.stroke();
});

var window = $('window1');
if (window) { 
    window.setTitle('TEST!').show(); 
    app.run(window); 
}