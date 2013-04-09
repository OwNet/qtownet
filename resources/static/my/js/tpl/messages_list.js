define(['jade'], function(jade) { if(jade && jade['runtime'] !== undefined) { jade = jade.runtime; }

return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client/messages_list.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
// iterate messages
;(function(){
  if ('number' == typeof messages.length) {

    for (var index = 0, $$l = messages.length; index < $$l; index++) {
      var message = messages[index];

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
buf.push('' + escape((interp = message.first_name) == null ? '' : interp) + ' ' + escape((interp = message.last_name) == null ? '' : interp) + '');
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
buf.push('' + escape((interp = message.message) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n  <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(message.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
if ( (user.id == message.user_id))
{
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('\n  <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#"), 'data-id':("" + (message.uid) + ""), 'name':("delete-message"), 'data-group_id':("" + (group_id) + "") }, {"href":true,"data-id":true,"name":true,"data-group_id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('Delete');
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
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
if ( (message.comments))
{
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
// iterate message.comments
;(function(){
  if ('number' == typeof message.comments.length) {

    for (var $index = 0, $$l = message.comments.length; $index < $$l; $index++) {
      var comment = message.comments[$index];

__jade.unshift({ lineno: 13, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n  <div class="row2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('\n    <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.first_name) == null ? '' : interp) + ' ' + escape((interp = comment.last_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n    <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.message) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('\n    <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(comment.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
if ( (user.id == comment.user_id))
{
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('\n    <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#"), 'data-id':("" + (comment.uid) + ""), 'name':("delete-message"), 'data-group_id':("" + (group_id) + "") }, {"href":true,"data-id":true,"name":true,"data-group_id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('Delete');
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
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
    }

  } else {
    var $$l = 0;
    for (var $index in message.comments) {
      $$l++;      var comment = message.comments[$index];

__jade.unshift({ lineno: 13, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n  <div class="row2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('\n    <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.first_name) == null ? '' : interp) + ' ' + escape((interp = comment.last_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n    <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.message) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('\n    <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(comment.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
if ( (user.id == comment.user_id))
{
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('\n    <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#"), 'data-id':("" + (comment.uid) + ""), 'name':("delete-message"), 'data-group_id':("" + (group_id) + "") }, {"href":true,"data-id":true,"name":true,"data-group_id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('Delete');
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
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
    }

  }
}).call(this);

__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.unshift({ lineno: 24, filename: __jade[0].filename });
buf.push('\n  <form');
buf.push(attrs({ 'name':("comment-message-form"), 'id':("comment-" + (index) + ""), "class": ('form-horizontal') }, {"name":true,"id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 26, filename: __jade[0].filename });
buf.push('\n    <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 27, filename: __jade[0].filename });
buf.push('\n      <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 28, filename: __jade[0].filename });
buf.push('\n        <input type="text" name="message" class="comment-message"/>');
__jade.shift();
__jade.unshift({ lineno: 29, filename: __jade[0].filename });
buf.push('<span class="help-block">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</span>');
__jade.shift();
__jade.unshift({ lineno: 30, filename: __jade[0].filename });
buf.push('\n        <input');
buf.push(attrs({ 'type':("hidden"), 'name':("group_id"), 'value':("" + (group_id) + ""), "class": ('group_id') }, {"type":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 31, filename: __jade[0].filename });
buf.push('\n        <input');
buf.push(attrs({ 'type':("hidden"), 'name':("parent_id"), 'value':("" + (message.uid) + ""), "class": ('parent_id') }, {"type":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 32, filename: __jade[0].filename });
buf.push('\n        <input');
buf.push(attrs({ 'type':("hidden"), 'name':("page"), 'value':("" + (page) + ""), "class": ('page') }, {"type":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 33, filename: __jade[0].filename });
buf.push('\n        <button');
buf.push(attrs({ 'type':("button"), 'name':("submit-message"), 'data-id':("" + (index) + ""), "class": ('btn') }, {"type":true,"name":true,"data-id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 33, filename: __jade[0].filename });
buf.push('Comment');
__jade.shift();
__jade.shift();
buf.push('</button>');
__jade.shift();
__jade.shift();
buf.push('\n      </div>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.shift();
buf.push('\n  </form>');
__jade.shift();
__jade.shift();
buf.push('\n</div>');
__jade.shift();
__jade.shift();
    }

  } else {
    var $$l = 0;
    for (var index in messages) {
      $$l++;      var message = messages[index];

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
buf.push('' + escape((interp = message.first_name) == null ? '' : interp) + ' ' + escape((interp = message.last_name) == null ? '' : interp) + '');
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
buf.push('' + escape((interp = message.message) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 7, filename: __jade[0].filename });
buf.push('\n  <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('\n    <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 8, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(message.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n  </div>');
__jade.shift();
__jade.unshift({ lineno: 9, filename: __jade[0].filename });
if ( (user.id == message.user_id))
{
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
__jade.unshift({ lineno: 10, filename: __jade[0].filename });
buf.push('\n  <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#"), 'data-id':("" + (message.uid) + ""), 'name':("delete-message"), 'data-group_id':("" + (group_id) + "") }, {"href":true,"data-id":true,"name":true,"data-group_id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 11, filename: __jade[0].filename });
buf.push('Delete');
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
__jade.unshift({ lineno: 12, filename: __jade[0].filename });
if ( (message.comments))
{
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
__jade.unshift({ lineno: 13, filename: __jade[0].filename });
// iterate message.comments
;(function(){
  if ('number' == typeof message.comments.length) {

    for (var $index = 0, $$l = message.comments.length; $index < $$l; $index++) {
      var comment = message.comments[$index];

__jade.unshift({ lineno: 13, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n  <div class="row2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('\n    <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.first_name) == null ? '' : interp) + ' ' + escape((interp = comment.last_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n    <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.message) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('\n    <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(comment.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
if ( (user.id == comment.user_id))
{
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('\n    <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#"), 'data-id':("" + (comment.uid) + ""), 'name':("delete-message"), 'data-group_id':("" + (group_id) + "") }, {"href":true,"data-id":true,"name":true,"data-group_id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('Delete');
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
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
    }

  } else {
    var $$l = 0;
    for (var $index in message.comments) {
      $$l++;      var comment = message.comments[$index];

__jade.unshift({ lineno: 13, filename: __jade[0].filename });
__jade.unshift({ lineno: 14, filename: __jade[0].filename });
buf.push('\n  <div class="row2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 15, filename: __jade[0].filename });
buf.push('\n    <div class="span1">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 16, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.first_name) == null ? '' : interp) + ' ' + escape((interp = comment.last_name) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 17, filename: __jade[0].filename });
buf.push('\n    <div class="span2">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 18, filename: __jade[0].filename });
buf.push('' + escape((interp = comment.message) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 19, filename: __jade[0].filename });
buf.push('\n    <div class="span3">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('\n      <label>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 20, filename: __jade[0].filename });
buf.push('created ' + escape((interp = new Date(comment.date_created).toLocaleDateString()) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</label>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.unshift({ lineno: 21, filename: __jade[0].filename });
if ( (user.id == comment.user_id))
{
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
__jade.unshift({ lineno: 22, filename: __jade[0].filename });
buf.push('\n    <div class="span5">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#"), 'data-id':("" + (comment.uid) + ""), 'name':("delete-message"), 'data-group_id':("" + (group_id) + "") }, {"href":true,"data-id":true,"name":true,"data-group_id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 23, filename: __jade[0].filename });
buf.push('Delete');
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
buf.push('\n  </div>');
__jade.shift();
__jade.shift();
    }

  }
}).call(this);

__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.unshift({ lineno: 24, filename: __jade[0].filename });
buf.push('\n  <form');
buf.push(attrs({ 'name':("comment-message-form"), 'id':("comment-" + (index) + ""), "class": ('form-horizontal') }, {"name":true,"id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 26, filename: __jade[0].filename });
buf.push('\n    <div class="control-group">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 27, filename: __jade[0].filename });
buf.push('\n      <div class="controls">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 28, filename: __jade[0].filename });
buf.push('\n        <input type="text" name="message" class="comment-message"/>');
__jade.shift();
__jade.unshift({ lineno: 29, filename: __jade[0].filename });
buf.push('<span class="help-block">');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.shift();
buf.push('</span>');
__jade.shift();
__jade.unshift({ lineno: 30, filename: __jade[0].filename });
buf.push('\n        <input');
buf.push(attrs({ 'type':("hidden"), 'name':("group_id"), 'value':("" + (group_id) + ""), "class": ('group_id') }, {"type":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 31, filename: __jade[0].filename });
buf.push('\n        <input');
buf.push(attrs({ 'type':("hidden"), 'name':("parent_id"), 'value':("" + (message.uid) + ""), "class": ('parent_id') }, {"type":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 32, filename: __jade[0].filename });
buf.push('\n        <input');
buf.push(attrs({ 'type':("hidden"), 'name':("page"), 'value':("" + (page) + ""), "class": ('page') }, {"type":true,"name":true,"value":true}));
buf.push('/>');
__jade.shift();
__jade.unshift({ lineno: 33, filename: __jade[0].filename });
buf.push('\n        <button');
buf.push(attrs({ 'type':("button"), 'name':("submit-message"), 'data-id':("" + (index) + ""), "class": ('btn') }, {"type":true,"name":true,"data-id":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 33, filename: __jade[0].filename });
buf.push('Comment');
__jade.shift();
__jade.shift();
buf.push('</button>');
__jade.shift();
__jade.shift();
buf.push('\n      </div>');
__jade.shift();
__jade.shift();
buf.push('\n    </div>');
__jade.shift();
__jade.shift();
buf.push('\n  </form>');
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
}

});