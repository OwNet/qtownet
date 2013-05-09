define( function (require) {

	var Backbone = require("backbone")
	, ActivityModel = require('share/models/ActivityModel')

	var Activities = Backbone.Collection.extend({

		model: ActivityModel, 

		comparator: function(item) {
	       var str = item.get("date_created");
  str = str.toLowerCase();
  str = str.split("");
  str = _.map(str, function(letter) { 
    return String.fromCharCode(-(letter.charCodeAt(0)));
  });
  return str;
	    },

		url: '/api/activities',

		_params: null,

		setParams: function(obj /*{user,group,type}*/ ) {
			this._params = obj
		},

		fetch: function(options) {
			options = options || {};
			options.data = options.data ?  _.extend(options.data, this._params) : this._params
			//opts.reset = false
			//opts.update = true
			var collection = this,
            success = options.success;
        options.success = function(resp, status, xhr) {
            _(collection.parse(resp, xhr)).each(function(item) {
                if (!collection.get(item.id)) {
                    collection.add(item, {silent:true});
                }
            });
            if (!options.silent) collection.trigger('reset', collection, options);
            if (success) success(collection, resp);
        };
        return (this.sync || Backbone.sync).call(this, 'read', this, options);

		//	return Activities.__super__.fetch.call(this, opts)
		},

		fetchPageCount: function() {
			var self = this

			return $.ajax({
				type: 'GET',
				url: this.url + '/pagesCount',
				data: _.omit( this._params || {}, 'page'),
				success: function(data) { self.pages = data.pages },
			})
		}

	})

	return Activities
})
