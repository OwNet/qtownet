define(['jade'], function(jade) { if(jade && jade['runtime'] !== undefined) { jade = jade.runtime; }

return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client/showgroup.jade" }];
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
if ( (group[0].admin == "1"))
{
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('\n      <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/editGroup"), 'data-id':("" + (group[0].id) + ""), 'name':("edit") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('Edit group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group[0].id) + ""), 'name':("delete") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Delete group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
if ( (group[0].admin == "0" && group[0].member == "1"))
{
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n      <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group[0].id) + ""), 'name':("leave") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('Leave group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n      <div class="span6">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/listmembers"), 'data-id':("" + (group[0].id) + ""), 'name':("listMembers") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('Show members');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('\n      <div id="group_detail" class="span">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</div>');
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
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('\n<div id="group_messages" class="span">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.shift();
}
return buf.join("");
} catch (err) {
  rethrow(err, __jade[0].filename, __jade[0].lineno);
}
}

});