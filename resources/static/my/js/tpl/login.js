define(["jade"],function(jade){return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client/login.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('\n  <div class="span6 offset3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n    <div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n      <div class="span6 center">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('<img src="img/ownet.png"/>');
__jade.shift();
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n    <div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n      <div class="span6">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('\n        <form name="login-form" class="form-horizontal">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('\n          <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('\n            <label class="control-label">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('Login:');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n            <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('\n              <input type="text" name="login"/>');
__jade.shift();
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('<span class="help-block">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</span>');
__jade.shift();
__jade.shift();
buf.push('\n            </div>');
__jade.shift();
__jade.shift();
buf.push('\n          </div>');
__jade.shift();
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('\n          <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('\n            <label class="control-label">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('Password:');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('\n            <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('\n              <input type="password" name="password"/>');
__jade.shift();
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('<span class="help-block">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</span>');
__jade.shift();
__jade.shift();
buf.push('\n            </div>');
__jade.shift();
__jade.shift();
buf.push('\n          </div>');
__jade.shift();
__jade.unshift({ lineno: 24, filename: __jade[0].filename });
buf.push('\n          <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 25, filename: __jade[0].filename });
buf.push('\n            <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 30, filename: __jade[0].filename });
__jade.shift();
__jade.unshift({ lineno: 30, filename: __jade[0].filename });
buf.push('\n              <button id="login" type="button" name="submit" class="btn">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 30, filename: __jade[0].filename });
buf.push('Log In');
__jade.shift();
__jade.shift();
buf.push('</button>');
__jade.shift();
__jade.shift();
buf.push('\n            </div>');
__jade.shift();
__jade.shift();
buf.push('\n          </div>');
__jade.shift();
__jade.shift();
buf.push('\n        </form>');
__jade.shift();
__jade.shift();
buf.push('\n      </div>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
buf.push('\n</div>');
__jade.shift();
__jade.shift();
}
return buf.join("");
} catch (err) {
  rethrow(err, __jade[0].filename, __jade[0].lineno);
}
}});