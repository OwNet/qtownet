define(["jade"],function(jade){return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client\\listmembersgroup.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
// iterate action
;(function(){
  if ('number' == typeof action.length) {

    for (var $index = 0, $$l = action.length; $index < $$l; $index++) {
      var user = action[$index];

__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
buf.push('\n  <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('' + escape((interp = user.first_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n  <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 6, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 6, filename: __jade[0].filename });
buf.push('' + escape((interp = user.last_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
if ( (user.status == "member"))
{
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n  <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('member since ' + escape((interp = new Date(user.member_since).toLocaleDateString()) == null ? '' : interp) + '');
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
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
if ( (group[0].admin == "1" && user.isAdmin != "1" && user.status == "member"))
{
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("add-admin") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Add admin');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("delete-member") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('Delete user');
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
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
if ( (group[0].admin == "1" && user.isAdmin != "1" && user.status == "awaiting"))
{
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("approve-member") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('Approve');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("reject-member") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('Reject');
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

  } else {
    for (var $index in action) {
      var user = action[$index];

__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('\n<div class="row">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 3, filename: __jade[0].filename });
buf.push('\n  <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 4, filename: __jade[0].filename });
buf.push('' + escape((interp = user.first_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 5, filename: __jade[0].filename });
buf.push('\n  <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 6, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 6, filename: __jade[0].filename });
buf.push('' + escape((interp = user.last_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
if ( (user.status == "member"))
{
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n  <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
buf.push('member since ' + escape((interp = new Date(user.member_since).toLocaleDateString()) == null ? '' : interp) + '');
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
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
if ( (group[0].admin == "1" && user.isAdmin != "1" && user.status == "member"))
{
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("add-admin") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
buf.push('Add admin');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("delete-member") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
buf.push('Delete user');
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
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
if ( (group[0].admin == "1" && user.isAdmin != "1" && user.status == "awaiting"))
{
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('\n  <div class="span4">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("approve-member") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('Approve');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'data-id':("" + (user.id) + ""), 'data-group':("" + (group[0].id) + ""), 'name':("reject-member") }, {"data-id":true,"data-group":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('Reject');
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