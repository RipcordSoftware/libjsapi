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

var label = builder.getLabel('label1');
var button = builder.getButton('button1');
var entry = builder.getEntry('entry1');
if (label && button && entry) { 
    button.onClick(function(){ 
        var i = Math.random() * msgs.length;
        var msg = msgs[Math.floor(i)];
        label.setText(msg);
        entry.setText(msg);
    }); 
}

var checkButton = builder.getCheckButton('checkbutton1');
if (checkButton) {
    checkButton.onClick(function() {
       if (entry) {
           checkButton.getActive() ? entry.hide() : entry.show();
       }
    });
}

var window = builder.getWindow('window1');
if (window) { 
    window.setTitle('TEST!').show(); 
    app.run(window); 
}