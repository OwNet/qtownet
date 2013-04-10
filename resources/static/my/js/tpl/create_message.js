define(['jade'], function(jade) { if(jade && jade['runtime'] !== undefined) { jade = jade.runtime; }

return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client/create_message.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
buf.push('\n<form name="create-message-form" class="form-horizontal">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
buf.push('\n  <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n    <label class="control-label">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('Message:');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n    <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n      <textarea name="message">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</textarea>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('<span class="help-block">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</span>');
__jade.shift();
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n      <input');
buf.push(attrs({ 'type':("hidden"), 'id':("shit"), 'name':("group_id"), 'value':("" + (group_id) + "") }, {"type":true,"id":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n      <input type="hidden" name="parent_id" value="0"/>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('\n    <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('\n      <button type="button" name="submit-message" class="btn">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('Send');
__jade.shift();
__jade.shift();
buf.push('</button>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
buf.push('\n</form>');
__jade.shift();
__jade.shift();
}
return buf.join("");
} catch (err) {
  rethrow(err, __jade[0].filename, __jade[0].lineno);
}
}

});