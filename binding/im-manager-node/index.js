global.native_lib = {};

/**
 * get native libray by opts
 * {
 *    os: 'linux',
 *    provider: 'ibus'
 * }
 */
function get_native_lib(opts) {
  var os = opts.os;
  var provider = opts.provider;

  switch (os) {
    case 'macos':
      return require('bindings')('im_manager_node_osx');
    case 'linux':
      if (provider === 'ibus') {
        return require('bindings')('im_manager_node_ibus');
      } else if (provider === 'fcitx') {
        return require('bindings')('im_manager_node_fcitx');
      }
      return null;
    case 'windows':
      return require('bindings')('im_manager_node_win');
    default:
      return null;
  }

}

exports.init = function init(opts) {
  native_lib = get_native_lib(opts);
};

exports.current = function(cb, opts) {

  var lib = opts == null ? native_lib : get_native_lib(opts);
  lib.current(cb);
};

exports.get = function(id, cb, opts) {

  var lib = opts == null ? native_lib : get_native_lib(opts);
  lib.get(id,cb);
};

exports.select = function(id, cb, opts) {

  var lib = opts == null ? native_lib : get_native_lib(opts);
  lib.select(id,cb);
};

exports.list = function(cb, opts) {

  var lib = opts == null ? native_lib : get_native_lib(opts);
  lib.list(cb);
};
