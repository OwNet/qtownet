define(["jade"],function(jade){return function anonymous(locals, attrs, escape, rethrow, merge) {
attrs = attrs || jade.attrs; escape = escape || jade.escape; rethrow = rethrow || jade.rethrow; merge = merge || jade.merge;
var __jade = [{ lineno: 1, filename: "templates-client\\recommendationspager.jade" }];
try {
var buf = [];
with (locals || {}) {
var interp;
var __indent = [];
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
__jade.unshift({ lineno: 1, filename: __jade[0].filename });
 for (var page=1; page<=action.pages; page++)
{
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('<a');
buf.push(attrs({ 'href':("#/recommendations"), 'data-filter':("" + (filter) + ""), 'data-id':("" + (page) + ""), 'name':("pager") }, {"href":true,"data-filter":true,"data-id":true,"name":true}));
buf.push('>');
__jade.unshift({ lineno: undefined, filename: __jade[0].filename });
__jade.unshift({ lineno: 2, filename: __jade[0].filename });
buf.push('' + escape((interp = page) == null ? '' : interp) + '');
__jade.shift();
__jade.shift();
buf.push('</a>');
__jade.shift();
__jade.shift();
}
__jade.shift();
__jade.shift();
}
return buf.join("");
} catch (err) {
  rethrow(err, __jade[0].filename, __jade[0].lineno);
}
}});