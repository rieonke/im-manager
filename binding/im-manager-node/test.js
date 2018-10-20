var imm = require('.');

var input_sourcses = [];

imm.init({os: 'macos'});

imm.list(function(arr = []){

  console.log(arr);

  imm.current(function(c = {}) {
    console.log(c);
  });

  imm.select(arr[0].id,function(status) {
    console.log(status);
  });

  imm.get(arr[0].id,function (im) {
    console.log(im);
  });

});

