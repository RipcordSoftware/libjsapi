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

var button = builder.getButton('button1');
if (button) { 
    button.onClick(function(){ 
        var i = Math.random() * msgs.length;
        var msg = msgs[Math.floor(i)];
        button.setLabel(msg); 
    }); 
}

var window = builder.getWindow('window1');
if (window) { 
    window.setTitle('TEST!').show(); 
    app.run(window); 
}