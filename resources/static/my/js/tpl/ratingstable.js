define(["jade"],function(jade){return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client\\ratingstable.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
// iterate activities
;(function(){
  if ('number' == typeof activities.length) {

    for (var $index = 0, $$l = activities.length; $index < $$l; $index++) {
      var activity = activities[$index];

__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
if ( (activity.type == 1))
{
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n  <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('' + escape((interp = activity.content) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 6, filename: __jade[0].filename });
buf.push('\n  <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n  <div class="offset">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('' + escape((interp = new Date(activity.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('' + escape((interp = activity.user_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Rating');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.shift();
buf.push('\n</div>');
__jade.shift();
__jade.shift();
    }

  } else {
    for (var $index in activities) {
      var activity = activities[$index];

__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
if ( (activity.type == 1))
{
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n  <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('' + escape((interp = activity.content) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 6, filename: __jade[0].filename });
buf.push('\n  <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n  <div class="offset">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('' + escape((interp = new Date(activity.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('' + escape((interp = activity.user_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Rating');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.shift();
buf.push('\n</div>');
__jade.shift();
__jade.shift();
   }
  }
}).call(this);

__jade.shift();
__jade.shift();
}
return buf.join("");
} catch (err) {
  rethrow(err, __jade[0].filename, __jade[0].lineno);
}
}});