define(["jade"],function(jade){return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client\\groupstable.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
// iterate groups
;(function(){
  if ('number' == typeof groups.length) {

    for (var $index = 0, $$l = groups.length; $index < $$l; $index++) {
      var group = groups[$index];

__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
if ( (filter == "all" || (filter == "my" && group.member == "1") || (filter == "admin" && group.admin == "1") || (filter == "notMy" && group.member == "0")))
{
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n  <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('' + escape((interp = group.name) == null ? '' : interp) + '');
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
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('' + escape((interp = group.description.substr(0,30)) == null ? '' : interp) + '...');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n  <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(group.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
if ( (group.member == "1"))
{
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/showgroup"), 'data-id':("" + (group.id) + ""), 'name':("show") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Show group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.shift();
}
else
{
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group.id) + ""), 'name':("join") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('Join group');
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
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
if ( (group.admin == "1"))
{
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n  <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/editGroup"), 'data-id':("" + (group.id) + ""), 'name':("edit") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('Edit group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group.id) + ""), 'name':("delete") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
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
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
if ( (group.admin == "0" && group.member == "1"))
{
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
buf.push('\n  <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group.id) + ""), 'name':("leave") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
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
__jade.shift();
buf.push('\n</div>');
__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.shift();
    }

  } else {
    for (var $index in groups) {
      var group = groups[$index];

__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
if ( (filter == "all" || (filter == "my" && group.member == "1") || (filter == "admin" && group.admin == "1") || (filter == "notMy" && group.member == "0")))
{
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n  <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('' + escape((interp = group.name) == null ? '' : interp) + '');
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
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('' + escape((interp = group.description.substr(0,30)) == null ? '' : interp) + '...');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n  <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(group.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
if ( (group.member == "1"))
{
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/showgroup"), 'data-id':("" + (group.id) + ""), 'name':("show") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Show group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.shift();
buf.push('</div>');
__jade.shift();
__jade.shift();
}
else
{
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group.id) + ""), 'name':("join") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('Join group');
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
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
if ( (group.admin == "1"))
{
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n  <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/editGroup"), 'data-id':("" + (group.id) + ""), 'name':("edit") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('Edit group');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group.id) + ""), 'name':("delete") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
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
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
if ( (group.admin == "0" && group.member == "1"))
{
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
buf.push('\n  <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/groups"), 'data-id':("" + (group.id) + ""), 'name':("leave") }, {"href":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
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
__jade.shift();
buf.push('\n</div>');
__jade.shift();
__jade.shift();
}
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