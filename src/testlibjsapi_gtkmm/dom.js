var document = {
    getElementById: function(id) { return builder.getWidget(id); }
}

// pretend we are jQuery (just a little)
var $ = document.getElementById;