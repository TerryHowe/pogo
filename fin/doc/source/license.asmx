window['pipeline_endpoint'] = 'http://beta.api.marketwatch.com/api/livesuite/broker.asmx';window['pickup_endpoint'] = 'http://beta.api.marketwatch.com/api/livesuite/pickup.asmx';window['pipeline_license'] = '088BB296A0A3BBCCA078C6746FD79C3EFE0421F8F860805E884AB98CCCDE87D45136095A01270696DA94E3FA38777EB26B71EF1485CA2F40D39F38329EB82A85D894CDCDB1E4322CE205D25AD8D3CD972F48B8104275551E391C5C0FFBA57C256ACBA87B';


/**
 * @class LiveNewsGateway manages the relationship between news updates and page control while
 * applying filters to the news feed. Do not call directly. Call LiveNewsGateway.Instance()
 * @type {LiveNewsGateway}
 * @constructor 
 */
function LiveNewsGateway ()
{
	var _serviceName = "LiveNews";
	var _pipeline = PipelineProtocol.GetPipeline(); 
	var _subscriptions = new Array();  
    var _pendingBodyRequests = new Array();
    var _storyCache = new NewsItemCollection();
	var _storyCount = 0;
    var _feedID = -1;
    
	this.SubscribeToNews = SubscribeToNews;
	this.ReSubscribeToNews = ReSubscribeToNews;
	this.UnSubscribeToNews = UnSubscribeToNews;
    this.SearchNews = SearchNews;
    this.GetStory = getStoryBody;
    this.GetFeedByID = GetFeedByID;
    this.getStoryByID = getStoryByID;
	
	/**
	 * Get a full {NewsItem} object from cache if available.
	 * 
	 * @param {Guid} storyID ID of the story to retrieve
	 */
	function getStoryByID(storyID)
	{
		return _storyCache.Items[storyID];
	}
	    
    /**
     * @return Total count of stories received.
     * @type {int}
     */
    this.getStoryCount = function() { return _storyCount; }
    
	function feedCallback(newsUpdate)
	{
	    if (newsUpdate.Status != null)
	    {
	        if (newsUpdate.Status == "stopped")
	        {
	            for (var subIdx = 0; subIdx < _subscriptions.length; subIdx++)
	            {
	                _pipeline.subscribeToFeed(_serviceName, _subscriptions[subIdx].Criteria);
	            }
	        }
	    }
	    else
	    {
	        // Subscribe response
	        if (newsUpdate.headlines != null)
	        {
				_storyCount += newsUpdate.headlines.length;
				_storyCache.addHeadlines(newsUpdate.headlines);
				
	            for (var i = 0; i < _subscriptions.length; i++)
	            {
                    if (_subscriptions[i].getIsActive())
                    {
                        _subscriptions[i].OnUpdate(newsUpdate.headlines);
	                }
	            }
	        }
	    }
	}
	
	function bodyCallback(bodyUpdate)
	{
		var update = _storyCache.addStoryBody(bodyUpdate.Story);
	    _pendingBodyRequests[update.ID](update);
	    _pendingBodyRequests[update.ID] = null;
	}

/**
 * @argument {String} feedID The ID returned by the call to {SubscribeToNews}
 * @type {LiveNewsSubscriber}
 * @return Subscriber object for a given feed ID.
 */	
	function GetFeedByID(feedID)
	{
	    for (var i = 0; i < _subscriptions.length; i++)
	    {
	        if (_subscriptions[i].ID == feedID) return _subscriptions[i];
	    }
	    
	    return null;
	}
	
/**
 * Binds a given callback method to the news feed filtered by the input criteria. The DocGroup of the license used already provides some DocType filtering.
 * 
 * @argument {String} subscriberName Simple name of a given subscriber
 * @argument {function} updateCallback Method to invoke when news matching the criteria is received
 * @argument {Object} criteria Object with the properties (valid values are DocType, Keyword, and Symbols) to be passed to the server for filtering (i.e. { "DocType":"343,106" } would limit the news sent to DocTypes 343 and 106)
 * @argument {String} filter Parseable string for filtering the display on the client
 * @type {int}
 * @return Feed ID
 */	
	function SubscribeToNews(subscriberName, updateCallback, criteria)
	{
        var feedID = _pipeline.subscribeToFeed(_serviceName, new function() { this.criteria = criteria });        
		var newSubscriber = new LiveNewsSubscriber(subscriberName, criteria, feedID, updateCallback);
		
        _subscriptions.push(newSubscriber);
        return newSubscriber;
	}

/**
 * Removes the binding of named subscriber to news.
 * 
 * @argument {String} subscriberName Simple name of a given subscriber
 */	
	function UnSubscribeToNews(subscriberName)
	{
	    var anyActive = false;
	    
	    for (var i = 0; i < _subscriptions.length; i++)
	    {
	        if (_subscriptions[i].Name == subscriberName)
	        {
	            _subscriptions[i].setIsActive(false);
	        }

	        if (_subscriptions[i].getIsActive()) anyActive = true;	        
        }
        
        if (!anyActive)
        {
            _pipeline.unSubscribeToFeed(_serviceName);
        }
	}

/**
 * Reactivates a binding for a previously named subscriber.
 * 
 * @argument {String} subscriberName Simple name of a given subscriber
 */	
	function ReSubscribeToNews(subscriberName)
	{
	    var anyActive = false;
	    var currentSubscriber = null;
	    
	    for (var i = 0; i < _subscriptions.length; i++)
	    {
	        if (_subscriptions[i].getIsActive()) anyActive = true;
	        
	        if (_subscriptions[i].Name == subscriberName)
	        {
	            currentSubscriber = _subscriptions[i];
	            currentSubscriber.setIsActive(true);
	        }
        }
        
        if (!anyActive)
        {
            _pipeline.subscribeToFeed(_serviceName, currentSubscriber.Criteria);
        }
	}

/**
 * @deprecated Search beyond the current feed is not yet available
 */
	function SearchNews(updateCallback, criteria)
	{
		var newSearch = new Action(_serviceName, "search");
		
		newSearch.criteria = criteria;
        _pipeline.sendRequest(newSearch, updateCallback);    
        return newSearch.id;    
	}

/**
 * Returns the HTML of a full story based on the story id and a callback function. 
 * @argument {String} guid ID property of a headline received.
 * @argument {function} callback Method to invoke when the story is returned. Will pass "" if no body exists.
 */		
    function getStoryBody(guid, callback)
    {        
        if (_storyCache.Items[guid].Body != null)
        {
            callback(_storyCache.Items[guid]);
        }
        else
        {
            _pendingBodyRequests[guid] = callback;
            
		    var newSearch = new Action(_serviceName, "detail");
    		
		    newSearch.criteria = new function() { this.id = guid; };
            _pipeline.sendRequest(newSearch, bodyCallback);    
        }        
    }
    
    function registerNewsListener()
    {
        _pipeline.registerService("LiveNews", feedCallback);
        window["lng"] = "LiveNews";
    }
    
    registerNewsListener();
}//end LiveNewsGateway

/**
 * @class LiveNewsSubscriber provide a joining point between a news feed and a control allowing for the addition of a criteria in the subscribe or further filtering on the client. Created internally, but the filter can be updated as desired.
 * @constructor
 * @type {LiveNewsSubscriber}
 * @argument {String} name subscriber name
 * @argument {Object} criteria Object with DocType, Symbols, and/or Keywords to filter news broadcast
 * @argument {String} filter parseable string that will apply filter to news displayed
 * @argument {String} feedID Internally assigned ID
 * @argument {function} subscriberCallback Method to invoke when news matching the filter is received.
 */
function LiveNewsSubscriber(name, criteria, feedID, firstCallback)
{
    var _feedID = feedID;
    var _callbacks = new Array();
    var _name = name;
    var _isActive = true;
   
    this.ID = _feedID;
    this.Name = name;
    this.Criteria = (criteria == null) ? new Object() : criteria;
    
    this.getIsActive = getIsActive;
    this.setIsActive = setIsActive;
    this.OnUpdate = OnUpdate;
    
    if (firstCallback != null)
    {
        _callbacks.push( new function() { this.callback = firstCallback, this.filter = new Filter} );
    }
    
    this.addListener = function(listenerCallback, filterString)
    {
        var filter = new Filter(filterString);

        _callbacks.push( new function() { this.callback = listenerCallback, this.filter = filter} );
        return new function() { this.callback = listenerCallback, this.filter = filter};
    }
    
    this.updateListener = function(listenerCallback, filterString)
    {
        _callbacks[listenerCallback] = null;
    } 
       
    this.stopListener = function(listenerCallback)
    {
        _callbacks[listenerCallback] = null;
    }
    
    /**
     * Returns the "Active" state of the subscriber
     * @type {bool}
     */
    function getIsActive() {  return _isActive; }
    
    /**
     * Update the active status
     * @argument {Boolean} newValue true to false
     */
    function setIsActive(newValue) { _isActive = newValue; }    
    
    function OnUpdate(updateValues, isUpdate)
    {
	    for (var i = 0; i < _callbacks.length; i++)
	    {
	        _callbacks[i].callback(_callbacks[i].filter.findMatches(updateValues));
	        //_callbacks[i].callback(updateValues);
	    }
	    
	    if (updateValues.length > 0)
	    {
	        this.Criteria.LastHeadline = updateValues[updateValues.length-1].ID;
	    }
    }
}

function NewsItem(itemElement)
{
    var _element = itemElement;
    
    this.Headline = _element.HEADLINE;
    this.Time = _element.Time;
    this.ID = _element.ID;
    this.Body = _element.Body;
    this.Abstract = _element.Abstract;
    this.Symbols = _element.Symbols;
    this.DocType = _element.DocType;
    this.Provider = _element.Provider;
	this.Author = _element.Author;
	this.Channels = _element.Channels;
	this.Industries = _element.Industries;
	this.Issues = _element.Issues;
	this.Products = _element.Products;
	this.Keywords = _element.Keywords;
	this.PersonalFinance = _element.PersonalFinance;
	this.Regions = _element.Regions;
	this.URI = _element.URI;
	this.ReferenceStoryGuid = _element.ReferenceStoryGuid;
	
	this.Compare = compareItem;
	
	function compareItem(newItem)
	{
	    return (_element.ID == newItem.ID);
	}
}

/**
 * @class Collection class for managing headlines received.
 */
function NewsItemCollection()
{
    var _headlines = new Array();
    
    /**
     * Returns the date time of the last update
     * @return {Date}
     */
     this.RetrieveDate = new Date();
	 
    /**
     * Returns the list of headlines
     * @type {Array}
     * @return List of headlines indexed by ID
     */    
    this.Items = _headlines;
	
    this.addHeadlines = addHeadlines;
    this.addHeadline = addHeadline;
    this.addStoryBody = addStoryBody;
	
	/**
	 * Function to join original story object with the full text
	 * @param {Story} storyBody Story object returned by call to get story body
	 */    
	function addStoryBody(storyBody)
	{
        if (_headlines[storyBody.ID] != null)
        {
			_headlines[storyBody.ID].Body = storyBody.Body;
			return _headlines[storyBody.ID];
        }		
		else
		{
			_headlines[storyBody.ID] = new NewsItem(storyBody);
			return _headlines[storyBody.ID];
		}
	}
	
    /**
     * Function to add a new headline to the list.
     * 
     * @argument {NewsItem} headlineObject News update received.
     */
    function addHeadline(headlineObject)
    {
	    this.RetrieveDate = new Date();
		
        if (_headlines[headlineObject.ID] == null)
        {
			_headlines[headlineObject.ID] = headlineObject;
        }
    }
    
     /**
     * Function to add a new headline to the list.
     * 
     * @argument {Array} headlineObjects List of {NewsItem} objects received.
     */
     function addHeadlines(headlineObjects)
     {
	    this.RetrieveDate = new Date();

        for (var i = 0; i < headlineObjects.length; i++)
        {
			addHeadline(headlineObjects[i]);
        }
    }    
 }
 
/**
 * Unitary instance of the LiveNewsGateway object
 * Do not call directly - use {LiveNewsGateway.Instance()}
 * if you need to search and bind the DOM instead
 */
LiveNewsGateway._Application = null;

/**
 * Creates the single LiveNewsGateway instance or returns the existing instance.
 * @argument {String} endpoint Full url for system access if default is to be overridden (Not recommended)
 */
LiveNewsGateway.Instance = function() 
{
    if (LiveNewsGateway._Application == null)
        LiveNewsGateway._Application = new LiveNewsGateway();
        
    return LiveNewsGateway._Application;
}///////////////////////////////////////////////////////////
//  LiveQuotesGateway.js
//  Implementation of the Class LiveQuotesGateway
//  Generated by Enterprise Architect
//  Created on:      28-Jun-2006 2:37:56 PM
///////////////////////////////////////////////////////////

/**
* Create a new instance of LiveQuotesGateway.
*
* @class	This class creates a new LiveQuotesGateway to provide for quote reuests and bindings. 
* @return Returns a new LiveQuotesGateway.
* @type {LiveQuotesGateway}
* @constructor	
*/
function LiveQuotesGateway ()
{
	var _serviceName = "LiveQuotes";
	var _pipeline = PipelineProtocol.GetPipeline(); 
	var _bindings = new Array();  
    
	this.BindToQuote = BindToQuote;
	this.BindToExchangeQuote = BindToExchangeQuote;
	this.BindToHybridQuote = BindToHybridQuote;
	this.UnBindToQuote = UnBindToQuote;
	this.UnBindToExchangeQuote = UnBindToExchangeQuote;
    this.UnBindToHybridQuote = UnBindToHybridQuote;
	this.RemoveQuote = RemoveQuote;
    this.RequestQuote = RequestQuote;
    this.Bindings = _bindings;
    
	function feedCallback(QuotesUpdate)
	{
	    if (QuotesUpdate.Status != null)
	    {
	        if (QuotesUpdate.Status == "stopped")
	        {
	            var rebindString = null;
	            
	            for (var ticker in _bindings)
	            {
	                if (typeof(_bindings[ticker]) == "object")
	                {
	                    if (rebindString == null)
	                    {
	                        rebindString = ticker;
	                    }
	                    else rebindString = rebindString.concat(',' + ticker);
	                }
	            }
	            
                if (rebindString.length > 0) _pipeline.subscribeToFeed(_serviceName, new function() { this.ticker = rebindString; this.blank = "nothing"; });  
	        }
	    }
	    else
	    {
            if (QuotesUpdate.Tickers != null)
            {
	            for (var i = 0; i < QuotesUpdate.Tickers.length; i++)
	            {
	                updateBinding(QuotesUpdate.Tickers[i]);
	            }
            }
            else updateBinding(QuotesUpdate.Ticker);
        }
	}
	
	function updateBinding(quoteUpdate)
	{
	    if (quoteUpdate.t == null || quoteUpdate.p == 0) return;
	    
	    var tArray = quoteUpdate.t.split(',');
	    var tempSymbol = quoteUpdate;
	    
	    for (var i = 0; i < tArray.length; i++)
	    {
	        tempSymbol.t = tArray[i];
	        
	        if (_bindings[quoteUpdate.t] != null)
	        {
				_bindings[quoteUpdate.t].setStatus(QuotedSymbol.StatusEnum.Subscribed);
	            _bindings[quoteUpdate.t].OnUpdate(quoteUpdate);
	        }
	        else
	        {
	            for (var bind in _bindings)
	            {
                    if (_bindings[bind]._ticker == quoteUpdate.t)
                    {
                        _bindings[bind].OnUpdate(quoteUpdate);
                    }
	            }
	        }
	    }
	}
	
     /**
      * This function provides a single quote request of one or more ticker symols. 
      * The tickers will NOT update (@see BindToQuote).  
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
 	  * @argument {Object} updateCallback Function to call with quote updates
      */
    function RequestQuote(tickerSymbol, updateCallback)
    {
		var newSearch = new Action(_serviceName, "search");
		
		newSearch.criteria = new function() { this.ticker = tickerSymbol; }
        _pipeline.sendRequest(newSearch, updateCallback);    
        return newSearch.id;    
    }

     /**
     * Overloaded method to support old pages 
     * that have no knowledge of new exchanges
     *
     *
     *
     **/
	 function BindToQuote(tickerSymbol, updateCallback)
	 {
	    var symbolBinding = null;
	    
        symbolBinding = BindToExchangeQuote(tickerSymbol, updateCallback, null);
        
        return symbolBinding;	    
	 }
	
     /**
      * This function provides a quote request of one or more ticker symols. 
      * The tickers WILL update (@see RequestQuote).  
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
 	  * @argument {Object} updateCallback Function to call with quote updates
      */
	function BindToExchangeQuote(tickerSymbol, updateCallback, exchangePrefix)
	{       
	    var symbolBinding = null;
	    var tickerText = tickerSymbol.toUpperCase().split(',');
	    var bindText = new String();
	    
	    for (var i = 0; i < tickerText.length; i++)
	    {
	        if (tickerText[i] == '') continue;
	        if (exchangePrefix != null) tickerText[i] = exchangePrefix + "!" + tickerText[i];
	        symbolBinding = _bindings[tickerText[i]];
	        
            if (symbolBinding == null)
            {
                bindText = (bindText == "") ? bindText.concat(tickerText[i]) : bindText.concat("," + tickerText[i]);

                symbolBinding = new QuotedSymbol(_bindings.length, tickerText[i]);
				symbolBinding.setStatus(QuotedSymbol.StatusEnum.Subscribing);
                _bindings[tickerText[i]] = symbolBinding;
            }
            else
            {
                if (symbolBinding._pickupItem != null && symbolBinding.getStatus() == QuotedSymbol.StatusEnum.Subscribed) 
                {
                    updateCallback(symbolBinding);
                }
                else if (symbolBinding.getStatus() != QuotedSymbol.StatusEnum.Subscribing)
				{ 
					bindText = (bindText == "") ? bindText.concat(tickerText[i]) : bindText.concat("," + tickerText[i]);
				}
            }

            symbolBinding.attach(updateCallback);
        }
        
        if (bindText.length > 0)
        {
            _pipeline.subscribeToFeed(_serviceName, new function() { this.ticker = bindText; this.blank = "nothing"; });  
        }
        
        return symbolBinding;
	}
	
	/**
      * This function provides the ability to subscribe to a Hybrid Symbol. 
      * This requires the symbol you are trying to attach to and there associated
      * Exchanges.  Current valid Exchange Identifiers are NLs, CSK (Comstock lookup)
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
 	  * @argument {Object} updateCallback Function to call with quote updates
 	  * @arguement {String} Comma Seperated list of 3 Character Exchange Codes
      */
     function BindToHybridQuote(tickerSymbol, updateCallback, exchanges)
	 {
	    var symbolBinding = null;
	    var result = CreateHybridExchange(exchanges);
        symbolBinding = BindToExchangeQuote(tickerSymbol, updateCallback, result);
        return symbolBinding;	    
	 }

     /**
      * This function provides the ability to disconnect from an updating ticker. 
      * The tickers will no longer update (@see BindToQuote). If the number of
      * listeners becomes zero, a {RemoveQuote} will be performed.
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
 	  * @argument {Object} updateCallback Function to call with quote updates
      */
	function UnBindToQuote(tickerSymbol, updateCallback)
	{
	    UnBindToExchangeQuote(tickerSymbol, updateCallback, null);       
	}    
	  /*
	  * This function provides the ability to disconnect from an updating ticker. 
      * The tickers will no longer update (@see BindToQuote). If the number of
      * listeners becomes zero, a {RemoveQuote} will be performed.
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
 	  * @argument {Object} updateCallback Function to call with quote updates
      */
	function UnBindToExchangeQuote(tickerSymbol, updateCallback, exchangePrefix)
	{
	    var symbolBinding;
	        var tickerText = tickerSymbol.toUpperCase().split(',');

            for (var i = 0; i < tickerText.length; i++)
            {               
                if(exchangePrefix != null) tickerText[i] = exchangePrefix + "!" + tickerText[i];
                symbolBinding = _bindings[tickerText[i]];
                
                if (symbolBinding != null)
                {
                    symbolBinding.detach(updateCallback);
                    
                    if (symbolBinding._attached.length == 0)
                    {
					    symbolBinding.setStatus(QuotedSymbol.StatusEnum.Unsubscribed);
                        _pipeline.unSubscribeToFeed(_serviceName, new function() { this.ticker = tickerText[i]; });  
                    }
                }
            } 
     }
	
	/**
      * This function provides the ability to disconnect from an updating ticker. 
      * The tickers will no longer update (@see BindToQuote). If the number of
      * listeners becomes zero, a {RemoveQuote} will be performed.
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
 	  * @argument {Object} updateCallback Function to call with quote updates
 	  * @argument {String} exchanges Comma Seperated List of exchanges. 
      */
	function UnBindToHybridQuote(tickerSymbol, updateCallback, exchanges)
	{
	    var result = CreateHybridExchange(exchanges);
	    UnBindToExchangeQuote(tickerSymbol, updateCallback, result);       
	}  
	
	function CreateHybridExchange(exchanges)
	{
	    exchangeText = exchanges.toUpperCase().split(',');
	    exchangeText.sort();
	    var exchangeInput = "";
	    for (var i = 0; i < exchangeText.length; i++)
	    {
	       exchangeInput = ( i == 0 ) ? exchangeInput.concat(exchangeText[i]) : exchangeInput.concat( ">" + exchangeText[i]);
	    }
	    if(exchangeText.length == 1)
	    {
	        exchangeInput.concat(">CSK"); // default to CSK if only one exchange supplied for second exchange
	    }
	        
	    return exchangeInput;
	}
	
     /**
      * This function provides the ability to stop and updating ticker completely. 
      * It does not affect the binding between a page element and the updates.
      * If the ticker is {BindToQuote} is called, all previous elements will reactivate.
      * @argument {String} tickerSymbol Comma separated list of ticker symbols
      */
	function RemoveQuote(tickerSymbol)
	{
	    var symbolBinding;
	    var tickerText = tickerSymbol.toUpperCase().split(',');

        for (var i = 0; i < tickerText.length; i++)
        {
            symbolBinding = _bindings[tickerText[i]];
            
            if (symbolBinding != null)
            {
				symbolBinding.setStatus(QuotedSymbol.StatusEnum.Unsubscribed);
			}
		}
		
        _pipeline.unSubscribeToFeed(_serviceName, new function() { this.ticker = tickerSymbol; });  
	}
		
    function registerQuotesListener()
    {
        _pipeline.registerService(_serviceName, feedCallback);
        //_pipeline.subscribeToFeed(_serviceName);
        window["lqg"] = _serviceName;
    }
    
    registerQuotesListener();
}//end LiveQuotesGateway

// Grafted from livequotes.js 08/08/2006 RJG

/**
* Create a new instance of QuotedSymbol.
*
* @class	This class is the basis for a returned ticker value. Created by {LiveQuotesGateway} when an update is received.
* @argument {String} itemId requested ticker symbol
* @argument {Object} Actual JSON encoded return from the server.
* @return Returns a new QuotedSymbol.
* @type {QuotedSymbol}
* @constructor	
*/
function QuotedSymbol(itemId, ticker)
{
	this._itemId = itemId;
	this._ticker = ticker;
	this._attached = new Array();
};

QuotedSymbol.StatusEnum = new Object();
QuotedSymbol.StatusEnum.Init = "Init";
QuotedSymbol.StatusEnum.Subscribing = "Subscribing";
QuotedSymbol.StatusEnum.Subscribed = "Subscribed";
QuotedSymbol.StatusEnum.Unsubscribed = "Unsubscribed";

 /**
  * This property provides a status feature of type {StatusEnum}.
  */
QuotedSymbol.prototype._status = QuotedSymbol.StatusEnum.Init;

/**
 * This function returns the originally requested ticker.
 */
QuotedSymbol.prototype.getItemId = function() {return this._itemId;};

/**
 * This function returns the actual ticker symbol being tracked.
 */
QuotedSymbol.prototype.getTicker = function() {return this._ticker;};

/**
 * This function returns the status.
 */
QuotedSymbol.prototype.getStatus = function() {return this._status;};

/**
 * This function sets the status.
 */

QuotedSymbol.prototype.setStatus = function(status) {this._status = status;};

/**
 * This function attaches a callback method to a particular symbol already bound. @see {LiveQuotesGateway.BindToQuote}
 * 
 * @argument {function} target The callback method to pass quote updates.
 */
QuotedSymbol.prototype.attach = function(target) 
{
    for (var i = 0; i < this._attached.length; i++)
    {
        if (this._attached[i] == target) return;
    }
    
	this._attached[this._attached.length] = target;
};

/**
 * This function detaches a callback method from a particular symbol already bound. @see {LiveQuotesGateway.UnBindToQuote}
 * 
 * @argument {function} target The callback method to remove from quote updates.
 */
QuotedSymbol.prototype.detach = function(target)
{
	for(var index = 0; index != this._attached.length; ++index)
	{
		if (this._attached[index] == target)
		{
			this._attached[index] = null;
			this._attached.splice(index, 1);
			break;
		}
	}
};

/**
 * This internal function is called by the {LiveQuotesGateway} when news values are received.
 * 
 * @argument {Object} item The server created quote item.
 */
QuotedSymbol.prototype.OnUpdate = function(item)
{
	this._pickupItem = item;
	
	for(var index = 0; index != this._attached.length; ++index)
	{
		var target = this._attached[index];
		if (target != null)
		{
			try
			{
				target(this);
			}
			catch(ex)
			{
    		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":ex, "location":"QuotedSymbol.prototype.OnUpdate" });
			}
		}
	}
};

/**
 * This function returns the time of the last update.
 * 
 */
QuotedSymbol.prototype.Timestamp = function()
{
    var value = new Date(this._pickupItem.T);
    value.format = DateFormat;
    return value;
};


/**
 * This function returns the company name associated with the symbol.
 */
QuotedSymbol.prototype.Name = function()
{
	return this._pickupItem.n;
};

/**
 * This function returns the last price received.
 */
QuotedSymbol.prototype.Price = function()
{
	return this._pickupItem.p;
};

/**
 * This function returns the last volume received.
 */
QuotedSymbol.prototype.Volume = function()
{
	return this._pickupItem.v;
};

/**
 * This function returns the last price change received.
 */
QuotedSymbol.prototype.Change = function()
{
	return this._pickupItem.c;
};

/**
 * This function returns the last price percentage change received.
 */
QuotedSymbol.prototype.PercentChange = function()
{
	return this._pickupItem.P;
};

QuotedSymbol.prototype.ExtendedTimestamp = function()
{
    return this.Timestamp();
};

QuotedSymbol.prototype.ExtendedPrice = function()
{
    return this.Price();
};

QuotedSymbol.prototype.ExtendedVolume = function()
{
    return this.Volume();
};

QuotedSymbol.prototype.ExtendedChange = function()
{
    return this.Change();
};

QuotedSymbol.prototype.ExtendedPercentChange = function()
{
    return this.PercentChange();
};

QuotedSymbol.prototype.MarketHoursTimestamp = function()
{
    return this.Timestamp();
};

QuotedSymbol.prototype.MarketHoursPrice = function()
{
    return this.Price();
};

QuotedSymbol.prototype.MarketHoursVolume = function()
{
    return this.Volume();
};

QuotedSymbol.prototype.MarketHoursChange = function()
{
    return this.Change();
};

QuotedSymbol.prototype.MarketHoursPercentChange = function()
{
    return this.PercentChange();
};


// Flags:
// 1 - PreMarket
// 2 - AfterMarket
// 4 - Realtime
// 8 - Halt
QuotedSymbol.prototype.Flags = function()
{
    return this._pickupItem.f;
};

QuotedSymbol.prototype.HasFlag = function(flag)
{
    return (parseInt(this._pickupItem.f) & flag) == flag;
}

/**
 * The QuotedElement clas binds a quoted symbol to an HTML element with custom HTML attributes.
 * "mwfield"  sets the property to be bound (i.e. Price = {QuotedSymbol.Price})
 * "mwformat" sets the display format (i.e. "Date")
 * 
 * @argument {HTML Object} htmlElement Element to be bound. 
 * @tickerSymbol {QuotedSymbol} ticker returned by {LiveQuotesGateway.BindToQuote} to be bound. 
 */
function QuotedElement(htmlElement, tickerSymbol)
{
    var _lastPrice = 0.0;
    //var _innerSpan = null;
    var _flashTimeout = null;
	var _htmlElement = (typeof(htmlElement) == "string") ? document.getElementById(htmlElement) : htmlElement;
	var _field = _htmlElement.getAttribute("mwfield");
	if (_field == null)_field = "Price";

	var _format = _htmlElement.getAttribute("mwformat");	
	var _baseStyleClass = _htmlElement.className;
	var _lastFlagClass = "";
	var _lastPriceClass = "";
    var _lastFieldClass = "";
    
    this.onPickup = onPickup;
    
    /**
     * Function to return actual HTML element bound.
     */
    this.getHtmlElement = function() 
    {
	    return _htmlElement;
    }

    /**
     * Internal - function called by LQ to update the element.
     */
    function onPickup(data)
    {
//	    if (_innerSpan == null)
//	    {
//		    _innerSpan = document.createElement("span");
//		    _htmlElement.innerHTML = "";
//		    _htmlElement.appendChild(_innerSpan);
//	    }
    	
	    var fieldValue = data[_field]();
	    var priceValue = data.Price();

		var extendedTick = data.HasFlag(1) || data.HasFlag(2);
        if(extendedTick && _field.match(/MarketHours/))  return;
        if(!extendedTick && _field.match(/Extended/))  return;
        
	    if (fieldValue > 0)
	    {
		    _lastFieldClass = "up";
	    }
	    else if (fieldValue < 0)
	    {
		    _lastFieldClass = "down";
	    }
	    else
	    {
		    _lastFieldClass = "unchanged";
	    }
    	
	    if (_lastPrice != null)
	    {
		    if (_lastPrice < priceValue)
		    {
			    _lastPriceClass = " tradeup";
		    }
		    else if (_lastPrice > priceValue)
		    {
			    _lastPriceClass = " tradedown";
		    }
	    }

		if (data.HasFlag(8))
		{
			_lastFlagClass = " halt";
		}
		else if (data.HasFlag(1))
		{
			_lastFlagClass = " pre";
		}
		else if (data.HasFlag(2))
		{
			_lastFlagClass = " after";
		}
		else if (data.HasFlag(4))
		{
			_lastFlagClass = " realtime";
		}
		else
		{
			_lastFlagClass = " delayed";
		}

	    var fieldString = formatValue(fieldValue);
    	
		var mwBind = _htmlElement.getAttribute("mwbind");
	    if(mwBind != null)
	    {
		    _htmlElement.innerHTML = eval(mwBind);			
	    }
        else
        {
	        if (_htmlElement.innerHTML != fieldString)
		        _htmlElement.innerHTML = fieldString;
	        if (_htmlElement.className != getClassName())
	        {
		        _htmlElement.className = getClassName();
		        setFlashTimeout();
            }
        }
                
	    _lastPrice = priceValue;
	    //_htmlElement.innerHTML = symbol[_field]().toString();
    }
	
    /**
     * Internal - function called to apply display formatting.
     */
    function formatValue(fieldValue)
    {
	    if (_format == null || _format == "")
		    return fieldValue.toString();
        if (_field == "Flags")
            return "&nbsp;";
            
        if (fieldValue.format != null)
        {
            return fieldValue.format(_format);
        }

	    var leadingPlus = "";
	    var trailingPercent = "";
    	
	    var formatter = new NumberFormat(fieldValue);
	    formatter.setCommas(false);
	    for (var index = 0; index != _format.length; ++index)
	    {
		    var formatChar = _format.charAt(index);
		    if (formatChar == ",")
		    {
			    formatter.setCommas(true);
		    }
		    else if (formatChar == "%")
		    {
			    trailingPercent = "%";
		    }
		    else if (formatChar == "+")
		    {
			    leadingPlus = "+";
		    }
		    else if (formatChar >= "0" && formatChar <= "9")
		    {
			    formatter.setPlaces(new Number(formatChar));
		    }
	    }
    	
	    if (formatter.toUnformatted() <= 0)
		    leadingPlus = "";
    	
	    return leadingPlus + formatter.toFormatted() + trailingPercent;
    }

    /**
     * Internal - function called to flash/unflash on symbol update.
     */
    function setFlashTimeout()
    {
	    if (_flashTimeout != null)
	    {
		    window.clearTimeout(_flashTimeout);
	    }

	    _flashTimeout = window.setTimeout(
		    function()
		    {
			    _flashTimeout = null;
			    _lastPriceClass = "";
			    _htmlElement.className = getClassName();
		    }, 900);
    };

    /**
     * Returns last update type (i.e. tradeup.up)
     */
    function getClassName()
    {
	    return (_baseStyleClass == null) ? _lastFieldClass + _lastPriceClass + _lastFlagClass : _baseStyleClass + " " + _lastFieldClass + _lastPriceClass + _lastFlagClass;
    }
}

/**
 * Unitary instance of the LiveQuotesGateway object
 * Do not call directly - use {LiveQuotesGateway.Instance()} or {LiveQuotesGateway.setup()}
 * if you need to search and bind the DOM instead
 */
LiveQuotesGateway._Application = null;

/**
 * Creates the single LiveQuotesGateway instance or returns the existing instance.
 * @argument {String} endpoint Full url for system access if default is to be overridden (Not recommended)
 */
LiveQuotesGateway.Instance = function() 
{
    if (LiveQuotesGateway._Application == null)
        LiveQuotesGateway._Application = new LiveQuotesGateway();
        
    return LiveQuotesGateway._Application;
}

/**
 * exchange is an optional string to use to bind to a specific exchange during setup
 */
LiveQuotesGateway.setup = function(exchange)
{
	var app = new LiveQuotesGateway.Instance();
	var spanElements = document.getElementsByTagName("span");
	
	var blockedTickers = {"us:djia":0, "us:indu":0, "djia":0, "indu":0 };
	
	for(var index = 0; index != spanElements.length; ++index)
	{
		var spanElement = spanElements[index];
		var spanElementClassName = spanElement.className;

		if (spanElementClassName.toLowerCase() == "mwlivequotes")
		{
			var ticker = spanElement.getAttribute("mwsymbol");
			
			if( ticker != null && blockedTickers[ticker.toLowerCase()] == 0)
			    continue;
			
			if (ticker != null)
			{
			    var quotedElement = new QuotedElement(spanElement);
			    
			    if (exchange && exchange.length > 0)
			    {
			        if( exchange.indexOf(',') > 0 )
			        {
			            app.BindToHybridQuote(ticker, quotedElement.onPickup, exchange);
			        }
			        else
			        {
			            app.BindToExchangeQuote(ticker, quotedElement.onPickup, exchange);
			        }
				}
				else
					app.BindToQuote(ticker, quotedElement.onPickup);
			}
		}
	}
	
	return app;
};

function NumberFormat(num, inputDecimal)
{
this.VERSION = 'Number Format v1.5.4';
this.COMMA = ',';
this.PERIOD = '.';
this.DASH = '-'; 
this.LEFT_PAREN = '('; 
this.RIGHT_PAREN = ')'; 
this.LEFT_OUTSIDE = 0; 
this.LEFT_INSIDE = 1;  
this.RIGHT_INSIDE = 2;  
this.RIGHT_OUTSIDE = 3;  
this.LEFT_DASH = 0; 
this.RIGHT_DASH = 1; 
this.PARENTHESIS = 2; 
this.NO_ROUNDING = -1 
this.num;
this.numOriginal;
this.hasSeparators = false;  
this.separatorValue;  
this.inputDecimalValue; 
this.decimalValue;  
this.negativeFormat; 
this.negativeRed; 
this.hasCurrency;  
this.currencyPosition;  
this.currencyValue;  
this.places;
this.roundToPlaces; 
this.truncate; 
this.setNumber = setNumberNF;
this.toUnformatted = toUnformattedNF;
this.setInputDecimal = setInputDecimalNF; 
this.setSeparators = setSeparatorsNF; 
this.setCommas = setCommasNF;
this.setNegativeFormat = setNegativeFormatNF; 
this.setNegativeRed = setNegativeRedNF; 
this.setCurrency = setCurrencyNF;
this.setCurrencyPrefix = setCurrencyPrefixNF;
this.setCurrencyValue = setCurrencyValueNF; 
this.setCurrencyPosition = setCurrencyPositionNF; 
this.setPlaces = setPlacesNF;
this.toFormatted = toFormattedNF;
this.toPercentage = toPercentageNF;
this.getOriginal = getOriginalNF;
this.moveDecimalRight = moveDecimalRightNF;
this.moveDecimalLeft = moveDecimalLeftNF;
this.getRounded = getRoundedNF;
this.preserveZeros = preserveZerosNF;
this.justNumber = justNumberNF;
this.expandExponential = expandExponentialNF;
this.getZeros = getZerosNF;
this.moveDecimalAsString = moveDecimalAsStringNF;
this.moveDecimal = moveDecimalNF;
this.addSeparators = addSeparatorsNF;
if (inputDecimal == null) {
this.setNumber(num, this.PERIOD);
} else {
this.setNumber(num, inputDecimal); 
}
this.setCommas(true);
this.setNegativeFormat(this.LEFT_DASH); 
this.setNegativeRed(false); 
this.setCurrency(false); 
this.setCurrencyPrefix('$');
this.setPlaces(2);
}
function setInputDecimalNF(val)
{
this.inputDecimalValue = val;
}
function setNumberNF(num, inputDecimal)
{
if (inputDecimal != null) {
this.setInputDecimal(inputDecimal); 
}
this.numOriginal = num;
this.num = this.justNumber(num);
}
function toUnformattedNF()
{
return (this.num);
}
function getOriginalNF()
{
return (this.numOriginal);
}
function setNegativeFormatNF(format)
{
this.negativeFormat = format;
}
function setNegativeRedNF(isRed)
{
this.negativeRed = isRed;
}
function setSeparatorsNF(isC, separator, decimal)
{
this.hasSeparators = isC;
if (separator == null) separator = this.COMMA;
if (decimal == null) decimal = this.PERIOD;
if (separator == decimal) {
this.decimalValue = (decimal == this.PERIOD) ? this.COMMA : this.PERIOD;
} else {
this.decimalValue = decimal;
}
this.separatorValue = separator;
}
function setCommasNF(isC)
{
this.setSeparators(isC, this.COMMA, this.PERIOD);
}
function setCurrencyNF(isC)
{
this.hasCurrency = isC;
}
function setCurrencyValueNF(val)
{
this.currencyValue = val;
}
function setCurrencyPrefixNF(cp)
{
this.setCurrencyValue(cp);
this.setCurrencyPosition(this.LEFT_OUTSIDE);
}
function setCurrencyPositionNF(cp)
{
this.currencyPosition = cp
}
function setPlacesNF(p, tr)
{
this.roundToPlaces = !(p == this.NO_ROUNDING); 
this.truncate = (tr != null && tr); 
this.places = (p < 0) ? 0 : p; 
}
function addSeparatorsNF(nStr, inD, outD, sep)
{
nStr += '';
var dpos = nStr.indexOf(inD);
var nStrEnd = '';
if (dpos != -1) {
nStrEnd = outD + nStr.substring(dpos + 1, nStr.length);
nStr = nStr.substring(0, dpos);
}
var rgx = /(\d+)(\d{3})/;
while (rgx.test(nStr)) {
nStr = nStr.replace(rgx, '$1' + sep + '$2');
}
return nStr + nStrEnd;
}
function toFormattedNF()
{	
var pos;
var nNum = this.num; 
var nStr;            
var splitString = new Array(2);   
if (this.roundToPlaces) {
nNum = this.getRounded(nNum);
nStr = this.preserveZeros(Math.abs(nNum)); 
} else {
nStr = this.expandExponential(Math.abs(nNum)); 
}
if (this.hasSeparators) {
nStr = this.addSeparators(nStr, this.PERIOD, this.decimalValue, this.separatorValue);
} else {
nStr = nStr.replace(new RegExp('\\' + this.PERIOD), this.decimalValue); 
}
var c0 = '';
var n0 = '';
var c1 = '';
var n1 = '';
var n2 = '';
var c2 = '';
var n3 = '';
var c3 = '';
var negSignL = (this.negativeFormat == this.PARENTHESIS) ? this.LEFT_PAREN : this.DASH;
var negSignR = (this.negativeFormat == this.PARENTHESIS) ? this.RIGHT_PAREN : this.DASH;
if (this.currencyPosition == this.LEFT_OUTSIDE) {
if (nNum < 0) {
if (this.negativeFormat == this.LEFT_DASH || this.negativeFormat == this.PARENTHESIS) n1 = negSignL;
if (this.negativeFormat == this.RIGHT_DASH || this.negativeFormat == this.PARENTHESIS) n2 = negSignR;
}
if (this.hasCurrency) c0 = this.currencyValue;
} else if (this.currencyPosition == this.LEFT_INSIDE) {
if (nNum < 0) {
if (this.negativeFormat == this.LEFT_DASH || this.negativeFormat == this.PARENTHESIS) n0 = negSignL;
if (this.negativeFormat == this.RIGHT_DASH || this.negativeFormat == this.PARENTHESIS) n3 = negSignR;
}
if (this.hasCurrency) c1 = this.currencyValue;
}
else if (this.currencyPosition == this.RIGHT_INSIDE) {
if (nNum < 0) {
if (this.negativeFormat == this.LEFT_DASH || this.negativeFormat == this.PARENTHESIS) n0 = negSignL;
if (this.negativeFormat == this.RIGHT_DASH || this.negativeFormat == this.PARENTHESIS) n3 = negSignR;
}
if (this.hasCurrency) c2 = this.currencyValue;
}
else if (this.currencyPosition == this.RIGHT_OUTSIDE) {
if (nNum < 0) {
if (this.negativeFormat == this.LEFT_DASH || this.negativeFormat == this.PARENTHESIS) n1 = negSignL;
if (this.negativeFormat == this.RIGHT_DASH || this.negativeFormat == this.PARENTHESIS) n2 = negSignR;
}
if (this.hasCurrency) c3 = this.currencyValue;
}
nStr = c0 + n0 + c1 + n1 + nStr + n2 + c2 + n3 + c3;
if (this.negativeRed && nNum < 0) {
nStr = '<font color="red">' + nStr + '</font>';
}
return (nStr);
}
function toPercentageNF()
{
nNum = this.num * 100;
nNum = this.getRounded(nNum);
return nNum + '%';
}
function getZerosNF(places)
{
var extraZ = '';
var i;
for (i=0; i<places; i++) {
extraZ += '0';
}
return extraZ;
}
function expandExponentialNF(origVal)
{
if (isNaN(origVal)) return origVal;
var newVal = parseFloat(origVal) + ''; 
var eLoc = newVal.toLowerCase().indexOf('e');
if (eLoc != -1) {
var plusLoc = newVal.toLowerCase().indexOf('+');
var negLoc = newVal.toLowerCase().indexOf('-', eLoc); 
var justNumber = newVal.substring(0, eLoc);
if (negLoc != -1) {
var places = newVal.substring(negLoc + 1, newVal.length);
justNumber = this.moveDecimalAsString(justNumber, true, parseInt(places));
} else {
if (plusLoc == -1) plusLoc = eLoc;
var places = newVal.substring(plusLoc + 1, newVal.length);
justNumber = this.moveDecimalAsString(justNumber, false, parseInt(places));
}
newVal = justNumber;
}
return newVal;
} 
function moveDecimalRightNF(val, places)
{
var newVal = '';
if (places == null) {
newVal = this.moveDecimal(val, false);
} else {
newVal = this.moveDecimal(val, false, places);
}
return newVal;
}
function moveDecimalLeftNF(val, places)
{
var newVal = '';
if (places == null) {
newVal = this.moveDecimal(val, true);
} else {
newVal = this.moveDecimal(val, true, places);
}
return newVal;
}
function moveDecimalAsStringNF(val, left, places)
{
var spaces = (arguments.length < 3) ? this.places : places;
if (spaces <= 0) return val; 
var newVal = val + '';
var extraZ = this.getZeros(spaces);
var re1 = new RegExp('([0-9.]+)');
if (left) {
newVal = newVal.replace(re1, extraZ + '$1');
var re2 = new RegExp('(-?)([0-9]*)([0-9]{' + spaces + '})(\\.?)');		
newVal = newVal.replace(re2, '$1$2.$3');
} else {
var reArray = re1.exec(newVal); 
if (reArray != null) {
newVal = newVal.substring(0,reArray.index) + reArray[1] + extraZ + newVal.substring(reArray.index + reArray[0].length); 
}
var re2 = new RegExp('(-?)([0-9]*)(\\.?)([0-9]{' + spaces + '})');
newVal = newVal.replace(re2, '$1$2$4.');
}
newVal = newVal.replace(/\.$/, ''); 
return newVal;
}
function moveDecimalNF(val, left, places)
{
var newVal = '';
if (places == null) {
newVal = this.moveDecimalAsString(val, left);
} else {
newVal = this.moveDecimalAsString(val, left, places);
}
return parseFloat(newVal);
}
function getRoundedNF(val)
{
val = this.moveDecimalRight(val);
if (this.truncate) {
val = val >= 0 ? Math.floor(val) : Math.ceil(val); 
} else {
val = Math.round(val);
}
val = this.moveDecimalLeft(val);
return val;
}
function preserveZerosNF(val)
{
var i;
val = this.expandExponential(val);
if (this.places <= 0) return val; 
var decimalPos = val.indexOf('.');
if (decimalPos == -1) {
val += '.';
for (i=0; i<this.places; i++) {
val += '0';
}
} else {
var actualDecimals = (val.length - 1) - decimalPos;
var difference = this.places - actualDecimals;
for (i=0; i<difference; i++) {
val += '0';
}
}
return val;
}
function justNumberNF(val)
{
newVal = val + '';
var isPercentage = false;
if (newVal.indexOf('%') != -1) {
newVal = newVal.replace(/\%/g, '');
isPercentage = true; 
}
var re = new RegExp('[^\\' + this.inputDecimalValue + '\\d\\-\\+\\(\\)eE]', 'g');	
newVal = newVal.replace(re, '');
var tempRe = new RegExp('[' + this.inputDecimalValue + ']', 'g');
var treArray = tempRe.exec(newVal); 
if (treArray != null) {
var tempRight = newVal.substring(treArray.index + treArray[0].length); 
newVal = newVal.substring(0,treArray.index) + this.PERIOD + tempRight.replace(tempRe, ''); 
}
if (newVal.charAt(newVal.length - 1) == this.DASH ) {
newVal = newVal.substring(0, newVal.length - 1);
newVal = '-' + newVal;
}
else if (newVal.charAt(0) == this.LEFT_PAREN
&& newVal.charAt(newVal.length - 1) == this.RIGHT_PAREN) {
newVal = newVal.substring(1, newVal.length - 1);
newVal = '-' + newVal;
}
newVal = parseFloat(newVal);
if (!isFinite(newVal)) {
newVal = 0;
}
if (isPercentage) {
newVal = this.moveDecimalLeft(newVal, 2);
}
return newVal;
}///////////////////////////////////////////////////////////
//  PipelineMessage.js
//  Implementation of the Class PipelineMessage
//  Generated by Enterprise Architect
//  Created on:      30-Jun-2006 2:54:19 PM
///////////////////////////////////////////////////////////

var isIE = navigator.appVersion.match(/\bMSIE\b/);

/**
 * isDebug is a global variable that can be set to true to receive debug messages.
 */
var isDebug = window.location.search.substring(1);

/**
* MESSAGE_ACTIONS is an enumeration for the actionType of an Action
*
* @type {Array}
*/
var MESSAGE_ACTIONS = {
    pickup : "pickup",
	subscribe : "subscribe",
    unsubscribe : "unsubscribe",
    detail : "detail",
    search : "search",
    entitlements : "entitlements",
    health : "health"
};

/**
* Create a new instance of Header.
*
* @class	This class creates a new Header. Internal- should not be necessary to access directly.
* @return Returns a new Header.
* @type {Header}
* @constructor	
*/
function Header()
{
	/**
	 * @type {Client} Client object
	 */
    //this.client;
    /**
     * Update frequency allowed by license
     */
    //this.backoff = 2000;
}
/**
* Create a new instance of {Client}.
*
* @class	This class creates a new Client. Internal- should not be necessary to access directly.
* @return Returns a new Client object.
* @type {Client}
* @constructor	
*/
function Client()
{
    this.proof;
    this.license;
    this.isvalid = true;
}
/**
* Create a new instance of Action.
*
* @class	This class creates a new Action. Adding an action to the pipeline submits
* a syncronous request for a feature.
* @argument {String} serviceName Name of the service called by the action
* @argument {MESSAGE_ACTIONS} actionType  Type of request - refer to MESSAGE_ACTION enum
* @return Returns a new Action.
* @type {Action}
* @constructor	
*/
function Action(serviceName /* = 'pipeline' */, actionType /* = 'pickup' */)
{
	/**
	 * Action to perform
	 * @type {MESSAGE_ACTIONS}
	 */
    this.action = (actionType == null) ? MESSAGE_ACTIONS.pickup : actionType;
    /**
     * Name of service as specified in entitlements
     * @type {String} 
     */
    this.service = (serviceName == null) ? "pipeline" : serviceName;
    /**
     * Internally generated message id
     * @type {int} 
     */
    this.id = Math.round(Math.random() * 10000);
    /**
     * Parameters to be passed for the operation.
     * @type {Criteria}
     */
    this.criteria;
}

/**
 * Exposes responseData property on response from a given service action. Internal- created by the server.
 * Each service will return properties for that type.
 * @type {Response}
 * @constructor Create a new Response.
 */
function Response()
{
	/**
	 * either request id or feed id for pickup
	 * @type {String}
	 */
    this.id = "";
	/**
	 * name of the service (i.e. LiveNews)
	 */
	this.service = "";
	/**
	 * Number of updates contained in the response
	 */
	this.count = 0;
}
/**
* Create a new instance of Body. Exposes 'actions' and 'response' arrays for submission
* and receipt of messages to the server.
*
* @class This class creates a new Body. Internal- should not be necessary to access directly.
* @return Returns a new Body.
* @type {Body}
* @constructor	
*/
function Body()
{
	/**
	 * List of {Action} objects
	 * @type {Array}
	 */
    this.actions = new Array();
	/**
	 * List of {Response} objects
	 * @type {Array}
	 */
    //this.response = new Array();
}
/**
* Create a new instance of PipelineMessage. Exposes 'header' and 'body' objects for submission
* and receipt of messages to the server.
*
* @class PipelineMessage is the message encapsulation. 
* @return Returns a new PipelineMessage.
* @type {PipelineMessage}
* @constructor	
*/

function PipelineMessage()
{
	/**
	 * @type {Header}
	 */
    this.header = new Header();
	/**
	 * @type {Body}
	 */
    this.body = new Body(); // body is  a collection of Action(s) or Responses

    this.addHeader = addHeader;
    this.addAction = addAction;

/**
 * Method to add a {Header} value like 'proof' to a {PipelineMessage}. Primarily managed internally.
 * 
 * @member PipelineMessage
 * @argument {String} headerName
 * @argument {String} headerValue 
 */        
    function addHeader(headerName, headerValue)
    {
        this.header[headerName] = headerValue; 
    }
    
/**
 * Method to add a actions to the {Body} of a {PipelineMessage} Primarily managed internally. Use {PipelineProtocol}.{sendRequest}
 * 
 * @argument {Action} actionObject
 */        
    function addAction(actionObject)
    {
        this.body.actions.push(actionObject); 
    }
}

    
/**
 * PipelineProtocol is the central message manager for the LiveSuite products.
 * @class PipelineProtocol is the central message manager for the LiveSuite products.
 * @argument {String} endpoint passed in as an override if the default location is not desired
 * @constructor
 */
function PipelineProtocol(endpoint)
{
    var _failedPickups = 0;
    var _statusObject = new Object();
    var _streamActive = false;
    var _streamInProcess = false;
    var _actionInProcess = false;
    var _client = new Client();
    var _subscribers = new Array();
	var _src = (endpoint == null) ? "broker.asmx" : endpoint;
    var _Request = create(window["pipeline_endpoint"]);
    var _Stream = create(window["pickup_endpoint"]);    
    var _actionCallbacks = new Array(); 
    var _actionQueue = new Array();
    var _lastActionSent = new Date();
	var _lastStreamSent = new Date();
	
    /**
     * Method to retrieve the visible entitlements for each service licensed.
     * @member {PipelineProtocol}
     * @argument {function} entitlementCallback callback method that will pass the entitlement array
     */
    this.getEntitlement = function(entitlementCallback)
    {
        var serviceAction = new Action("system", MESSAGE_ACTIONS.entitlements);
        this.sendRequest(serviceAction, entitlementCallback);
    }
    
    this.getHealth = function(healthCallback)
    {
        var serviceAction = new Action("system", MESSAGE_ACTIONS.health);
        this.sendRequest(serviceAction, healthCallback);
    }
    
    /**
     * Method to create the communication link.
     * @member {PipelineProtocol}
     * @private
     * @argument {String} endpoint full url
     */
    function create(endpoint)
    {
	    if (endpoint.indexOf("http") == 0)
	    {
		    return new ScriptSrc_Gateway(endpoint);
	    }
	    else
	    {
		    return new XmlHttp_Gateway(endpoint);
	    }
    }
    
    /**
     * Method to initiate a pickup request
     * @member {PipelineProtocol}
     * @private
     */
    function sendStreamRequest()
    {
        if (_streamInProcess) return;
        
//        var serviceAction = new Action();
//        var pipelineMessage = new PipelineMessage();
        
		try
		{
//		    pipelineMessage.addAction(serviceAction);
//		    pipelineMessage.header.client = _client;
//		    PipelineProtocol.On_DebugMessage([pipelineMessage.toJSONString()]);
            
            _streamInProcess = true;
			_lastStreamSent = new Date();
	        _Stream.sendRequestResponse(_client.proof, streamCallback, _statusObject);
		}
		catch(ex)
		{
		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":ex, "location":"PipelineProtocol.sendStreamRequest" });
		}
    }
    
    /**
     * Method to receive a pickup response
     * @member {PipelineProtocol}
     * @private
     * @argument {PipelineMessage} responseMessage Response message
     */
    function streamCallback(responseMessage)
    {
        var nexttimeout = window.setTimeout(sendStreamRequest, PipelineProtocol.Backoff);

		if (_subscribers["System"] != null) _subscribers["System"].OnUpdate(responseMessage);
		
        if (responseMessage == null || responseMessage.Exception != null || responseMessage.body.response == null)
        {
			window.clearTimeout(nexttimeout);

            if (responseMessage.Exception != null)
            {
    		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":responseMessage.Exception, "location":"PipelineProtocol.streamCallback" } );
            }
            else
            {
    		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":"Empty pickup received", "location":"PipelineProtocol.streamCallback" } );
            }

            if (++_failedPickups > 25)
            {
    		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":"System unavailable or license invalid" } );
            }
            else if (++_failedPickups > 5)
            {
    		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":"Multiple pickup errors - resubscribing", "location":"PipelineProtocol.streamCallback" } );
                _streamActive = false;
                
                _client.isvalid = false;
                
                for (var sub in _subscribers)
                {
                    if (_subscribers[sub].OnUpdate != null)
                    {
                        _subscribers[sub].OnUpdate({"Status":"stopped"});
                    }
                }
            }
	        else window.setTimeout(sendStreamRequest, PipelineProtocol.Backoff);
        }
        else
        {
    	    _failedPickups = 0;
            _client.isvalid = true;

            try
            {
                if (responseMessage.header.backoff > PipelineProtocol.Backoff)
                {
                    PipelineProtocol.Backoff = responseMessage.header.backoff;
                }
                
                _client.proof = responseMessage.header.client.proof;
                
                PipelineProtocol.TotalCount = responseMessage.header.itemcount;
                
                for (var i = 0; i < responseMessage.body.response.length; i++)
                {
                    if (_subscribers[responseMessage.body.response[i].service] != null)
                    {
						if (!isIE)	
					{
						var updateData = _toJSONString(responseMessage.body.response[i]); 
						var sub = responseMessage.body.response[i].service;

                        window.setTimeout("PipelineProtocol._Application.callUpdate('" + sub + "'," + updateData + ")", (PipelineProtocol.Backoff / responseMessage.body.response.length)*i);
						}
                        else window.setTimeout(new function(){var sub = _subscribers[responseMessage.body.response[i].service]; sub.OnUpdate(responseMessage.body.response[i]); }, 250*i);
                    }
                }
                
    		    PipelineProtocol.On_StatusUpdate({ "Status":"success", "Message":"Responses processed", "location":"PipelineProtocol.streamCallback" } );
            }
            catch (msg)
            {
    		    PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":msg, "location":"PipelineProtocol.streamCallback" } );
            }
        }

        _streamInProcess = false;
    }
        
	this.callUpdate = function(subName, updateData)
	{
		var sub = _subscribers[subName]; 
		
		if (sub != null)
		{sub.OnUpdate(updateData);
		}
		else alert(subName);
		 
	}
	
    /**
     * Method to initiate an action request with all actions in the queue
     * @member {PipelineProtocol}
     * @private
     */
    function sendActionRequest()
    {      
        if (_actionInProcess) return;
            
        
		var action;
		var idx = 0;
        var requestMessage = new PipelineMessage();
        
		try
		{
	        while (action = _actionQueue.pop())
	        {
	        	if (action == null) continue;
	        	
	        	requestMessage.addAction(action);
	        	if (idx++ > 8) break;
	        }

            if (requestMessage.body.actions.length > 0)
            {
		        requestMessage.header.client = _client;

				_lastActionSent = new Date();
                _actionInProcess = true;
	            _Request.sendRequestResponse(_toJSONString( requestMessage ), actionCallback, _statusObject);
            }
		}
		catch(ex)
		{
    		PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":ex, "location":"PipelineProtocol.sendActionRequest" } );
		}
    }
    
/**
 * Method invoked when an action response is received
 * @member {PipelineProtocol}
 * @private
 */    
    function actionCallback(responseMessage)	
	{
		if (_subscribers["System"] != null) _subscribers["System"].OnUpdate(responseMessage);
				
        if (responseMessage == null)
        {
            _failedPickups++;
    		PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":"Empty action response received", "location":"PipelineProtocol.actionCallback" } );
		   //if (!_streamActive) window.setTimeout(sendStreamRequest, 30);
        }
	    else if (responseMessage.Exception != null)
	    {
            _failedPickups++;
    		PipelineProtocol.On_StatusUpdate({ "Status":"exception", "Exception":responseMessage.Exception, "location":"PipelineProtocol.actionCallback" } );
		   //if (!_streamActive) window.setTimeout(sendStreamRequest, 30);
	    }
        else
        {
            _client.proof = responseMessage.header.client.proof;
            _client.license = "";
    	    _failedPickups = 0;
            _client.isvalid = true;

	        for (var i = 0; i < responseMessage.body.response.length; i++)
	        {
	    	    actionResponse = responseMessage.body.response[i];
	    	    actionToProcess = _actionCallbacks[actionResponse.id];
		    	
	    	    if (actionToProcess != null)
	    	    {
	    		    actionToProcess(actionResponse);	    		
	    		    actionToProcess = null;
	    	    }
	        }
		}

		_actionInProcess = false;
    	if (!_streamActive)
    	{
    	    _streamActive = true;
    	    sendStreamRequest();
    	}
		sendActionRequest();
    }
    
    this.sendRequest = function(requestAction, requestCallback)
    {
        _actionQueue.push(requestAction)
    	_actionCallbacks[requestAction.id] = requestCallback;
    	
        sendActionRequest();
    }
    
    this.subscribeToFeed = function(serviceName, criteria)
    {
        if (_subscribers[serviceName] != null)
        {
	        var serviceAction = new Action(serviceName, "subscribe");
	        
	        serviceAction.criteria = criteria;
	        _actionCallbacks[serviceAction.id] = _subscribers[serviceName].OnUpdate;
            _actionQueue.push(serviceAction);	        
            sendActionRequest();
	        return serviceAction.id;
        }
    }
    
    this.unSubscribeToFeed = function(serviceName, criteria)
    {
        if (_subscribers[serviceName] != null)
        {
	        var serviceAction = new Action(serviceName, "unsubscribe");
	        
	        serviceAction.criteria = criteria;
	        _actionCallbacks[serviceAction.id] = _subscribers[serviceName].OnUpdate;
            _actionQueue.push(serviceAction);	        
            
	        sendActionRequest();
	        return serviceAction.id;
        }
    }
    
    this.registerService = function(serviceName, serviceCallback)
    {
        var sub = new PipelineSubscriber(serviceName, serviceCallback);

        _subscribers[serviceName] = sub;
    }

    function setWindowTimeout(timeoutName, interval)
    {
    	var windowTimeout = window.setTimeout(function()
		{
			if (window[cb] != null)
			{
				var response = new Object();
				response.Status = "exception";
				response.Exception = new Object();
				response.Exception.message = "request timeout";
				window[cb](response);
			}
		}, 35000);
    }
    
	//_client.proof = window['pipeline_proof'];
	_client.license = window['pipeline_license'];
    PipelineProtocol.On_StatusUpdate({ "Status":"success", "Message":"Pipeline initialized", "location":"PipelineProtocol" } );
}

/**
 * Unitary instance of the PipelineProtocol object
 * Do not call directly - use {PipelineProtocol.GetPipeline} instead
 */
PipelineProtocol._Application = null;

/**
 * Total number of updates received
 */
PipelineProtocol.TotalCount = 0;

/**
 * Seconds from the last response system will wait prior to the next request.
 * This can be set higher than license value, but a lower value
 * will be reset to the license value as a floor.
 */
PipelineProtocol.Backoff = 2000;

/**
 * Creates the single protcol instance or returns the existing instance.
 * @argument {String} endpoint Full url for system access if default is to be overridden (Not recommended)
 */
PipelineProtocol.GetPipeline = function(endpoint) 
{
    if (PipelineProtocol._Application == null)
        PipelineProtocol._Application = new PipelineProtocol(endpoint);
        
    return PipelineProtocol._Application;
}

/**
 * Implement this function to received system status messages
 * @argument {Status} statusObject The 'status' property will provide the message text. The 'Exception' property will provide any error information.
 */
PipelineProtocol.On_StatusUpdate = function(statusObject) {}

/**
 * Implement this function to received debug messages. Call this method to publish messages.
 * 
 * @argument {String} messageItem Name of debug item - should be unique for a location
 * @argument {String} messageText Debug message
 * @argument {String} sourceLocation [optional] specific class and method information 
 */
PipelineProtocol.On_DebugMessage = function(messageItem, messageText, sourceLocation) {}

/**
 * Creates a PipelineSubscriber
 * @return {PipelineSubscriber}
 * @argument {String} serviceName Name of service to receive messages
 * @argument {function} subscriberCallback Method to call on message received from service
 * @class Pipeline subscribers are internal classes that manage subscribing services and callback methods
 * @constructor
 */
function PipelineSubscriber(serviceName, subscriberCallback)
{
    var _serviceName = serviceName;
    var _callback = subscriberCallback;

    this.OnUpdate = OnUpdate;
    
    function OnUpdate(updateValues)
    {
        _callback(updateValues);
    }
}

/*
 * Class to hold individual Filter properties parsed from a filter string
 * 
 * @argument {String} propName
 * @argument {String} initialValue
 * @argument {String} operation Joins the name and value (i.e. '=')
 */
function FilterProperty(propName, initialValue, operation, notFlag)
{
    this.Name = propName;
    var _notFlag = notFlag;
    var _groupFlag = (operation == 'contains' || operation == 'in');
    var _Value = (initialValue == null) ? new Array() : initialValue.toLowerCase().split(',');
    var _Operation = (operation == null) ? "contains" : operation;
    
    this.addValue = addValue;
    this.removeValue = removeValue;
    this.testValue = testValue;
    this.ToString = ToString;
	
	function ToString()
	{
		//return "(" + this.Name + " " + _Operation + " " + _Value.join(',') + ")";	
		if (_notFlag)
		{
		    return this.Name + " !" + _Operation + " " + _Value.join(',');	
		}
		else return this.Name + " " + _Operation + " " + _Value.join(',');	
	}
	
    function addValue(valueName)
    {
        _Value.push(valueName.toLowerCase());
    }
    
    function removeValue(valueName)
    {
        for (var i = 0; i < _Value.length; i++)
        {
            if (_Value[i] == valueName) _Value[i] = null;
        }
    }
    
    /**
     * Method called to evaluate new data against the filter.
     * @argument {Object} valueName Array, String, or Class to run through the filter 
     */    
    function testValue(valueName)
    {
        var testValue;
        
        if (valueName instanceof Array)
        {
            testValue = valueName.join(',');
        }
        else if (typeof(valueName) == "string")
        {
            testValue = valueName;
        }
        else
        {
            testValue = eval('valueName.' + this.Name);
            
            if (testValue == null) return true;

            if (testValue instanceof Array)
            {
                testValue = testValue.join(',');
            }
        }
        
        if (_groupFlag)
            return testGroupValue(testValue);
        else
            return testSingleValue(testValue);
    }
    
    function testSingleValue(valueValue)
    {        
        for (var i = 0; i < _Value.length; i++)
        {
            switch (_Operation)
            {
                case "=":
                    if (valueValue.toLowerCase() == _Value[i]) return !_notFlag;
                    break;
                case ">":
                    if (valueValue > _Value[i]) return !_notFlag;
                    break;
                case "<":
                    if (valueValue < _Value[i]) return !_notFlag;
                    break;
            }
        }
        
        return false;
    }
    
    /**
     * Method called to evaluate new data against a group filter (i.e. 'in' or 'contains').
     * @argument {Object} valueValue single value to run through the filter 
     */    
    function testGroupValue(valueValue)
    {       
        for (var i = 0; i < _Value.length; i++)
        {
            if (_notFlag)
            {
                //if (_Value[i].indexOf(valueValue.toLowerCase()) != -1) return !;
                if (valueValue.toLowerCase().indexOf(_Value[i]) != -1) return false;
            }
            else
            {
                if (valueValue.toLowerCase().indexOf(_Value[i]) != -1) return true;
            }
        }
        
        return _notFlag;
    }
}

/*
 * Class to evaluate individual filter properties parsed from a filter string
 * 
 * @argument {FilterProperty} baseProperty Filter to evaluate
 */
function FilterCondition(baseProperty)
{
    var _baseProperty = baseProperty;
    var _joinProperty = null;
    var _join = null;
    
    this.satisfiesCondition = satisfiesCondition;
    this.addProperty = addProperty;
    this.ToString = ToString;
	
	function ToString()
	{
		var expr = "";
		
        if (_joinProperty == null)
        {
            return (_baseProperty.ToString());
        }
        else
        {
            return _baseProperty.ToString() + " " + _join + " " + _joinProperty.ToString();
        }
		
        return expr;
	}
	
    /**
     * Method called to evaluate new data against the filter.
     * Calls testValue on each individual {FilterProperty} 
     * @argument {Object} newItem Array, String, or Class to run through the filter 
     */
    function satisfiesCondition(newItem)
    {
        if (_joinProperty == null)
        {
            return (_baseProperty.testValue(newItem));
        }
        else
        {
            switch(_join)
            {
                case "and":
                    return _baseProperty.testValue(newItem) && _joinProperty.testValue(newItem);
                case "or":
                    return _baseProperty.testValue(newItem) || _joinProperty.testValue(newItem);
            }
        }
        return true;
    }
    
    function addProperty(filterProperty, joinOperation)
    {
        _join = (joinOperation == null) ? "and" : joinOperation;
        _joinProperty = filterProperty;    
    }    
}

/**
 * @class Overall object for filtering data
 * @constructor
 * @argument {String} filterString Parseable string to apply filters (i.e. symbols=MSFT or headline contains 'tech')
 */
function Filter(filterString)
{
    var _tokenArray = new Array('(', ')', '=', '!', '>', '<');
    var _antiTokenArray = new Array('\"', '\'');
    var _functionArray = new Array('=', '!', '<', '>', 'contains', 'in', 'not');
    var _joinArray = new Array('and', 'or', '||', '&&');
    var _conditionList = new Array();
    var _joinList = new Array();
    
	this.isEmpty = isEmpty;
	this.isMatch = isMatch;
	this.findMatches = findMatches;
	this.reset = resetFilterValues;
	this.add = addFilterValues;
    this.ToString = ToString;
 	
 	this.Condition = filterString;
 	
	function ToString()
	{
	    if (isEmpty()) return "[none]";
		
		var joinIdx = 0;
		var joinString = new String();
		var expressionString = new String();
		
		for (var i = 0; i < _conditionList.length; i++)
		{
			expressionString += joinString + "(" + _conditionList[i].ToString() + ")";
			joinString = _joinList.length > joinIdx ? " " + _joinList[joinIdx++] + " " : " or ";
		}
		
	    return expressionString;
	}

	function addFilterValues(filterString)
	{
        parseFilterString(filterString)
	}
    
	function resetFilterValues()
	{
        _conditionList = new Array();
	}

	function isEmpty()
	{
        return _conditionList.length == 0;
	}

	function isMatch(newItem)
	{
	    for (var i = 0; i < _conditionList.length; i++)
	    {
            if(_conditionList[i].satisfiesCondition(newItem)) return true;
        }
        
        return (_conditionList.length == 0);
	}
	
	function findMatches(newItemArray)
	{
	    if (isEmpty()) return newItemArray;
	    
	    var matchArray = new Array();
	    
	    for (var key = 0; key < newItemArray.length; key++)
	    {
            if (isMatch(newItemArray[key]))
            {
                matchArray.push(newItemArray[key]);
            }
        }
        
        return matchArray;
	}

// ((country=us,uk or symbol=msft) and (headline contains Microsoft))
	function parseFilterString(filterString)
	{
        if (filterString == null || filterString.length == 0) return;
        
	    var filterArray = tokenizeString(filterString);
	    
	    var depth = 0;
	    var hasNot = false;
	    var currentOperation;
	    var currentJoin;
	    var startString;
	    var afterString;
	    var currentCondition;
	    var token;
	    
	    for (var tokenidx in filterArray)
	    {
	        if (typeof(filterArray[tokenidx]) == 'string') 
	             token = filterArray[tokenidx];
	        else continue;
	        
	        if (token == '(')
	        {
	            depth++;
	        }
	        else if (token == ')')
	        {
	            depth--;
	            if (currentCondition != null)
	            {
	                _conditionList.push(currentCondition);
	                currentCondition = null;
	                currentJoin = startString = afterString = currentOperation = null;
                    hasNot = false;
	            }
	        }
	        else if (_joinArray.contains(token))
	        {
                if (currentCondition == null)
                {
                    if (startString != null)
                    {
                        currentCondition = new FilterCondition(new FilterProperty(startString, afterString, currentOperation, hasNot));   
                        startString = afterString = currentOperation = null;
                        currentJoin = token;
                    }
                    else if (depth > 0)
                    {
                        currentJoin = token;
                    }
                    else _joinList.push(token);
                }
                else currentJoin = token;
	        }
	        else if (_functionArray.contains(token))
	        {
	            if (token == '!' || token == 'not')
	                hasNot = true;
	            else currentOperation = token;
	        }
	        else
	        {
	            if (currentOperation != null)
	            {
	                if (currentJoin == null || depth == 0)
	                {
	                    currentCondition = new FilterCondition(new FilterProperty(startString, token, currentOperation, hasNot));   
                        startString = afterString = currentOperation = null;
                        hasNot = false;
	                }
	                else
	                {
	                    currentCondition.addProperty(new FilterProperty(startString, token, currentOperation), currentJoin, hasNot);   
	                    currentJoin = startString = afterString = currentOperation = null;
                        hasNot = false;
                    }
	            }
	            else 
	            {
	                if (currentCondition != null && currentJoin == null)
	                {
                        _conditionList.push(currentCondition);
                        currentCondition = null;
	                }

                    if (startString == null)
                        startString = token;
                    else afterString = token;
	            }
	        }
	    }

        if (currentCondition != null)
        {
            _conditionList.push(currentCondition);
        }
        
        this.Condition = _conditionList;
	}

    function tokenizeString(filterString)
    {        
	    var tempString = new String();
        var tokenArray = new Array();
        
        for (var i = 0; i < filterString.length; i++)
        {
            if (_tokenArray.contains(filterString.charAt(i)))
            {
                if (tempString.length > 0) 
                {
                    tokenArray.push(tempString);
                    tempString = "";
                }
                
                tokenArray.push(filterString.charAt(i));
            }
            else if (filterString.charAt(i) == ' ')
            {
                if (tempString.length > 0) 
                {
                    tokenArray.push(tempString);
                    tempString = "";
                }
            }
            else if (_antiTokenArray.contains(filterString.charAt(i)))
            {
                // eliminate quotes
            }
            else
            {
                tempString += filterString.charAt(i);
            }
             
        }
        
        if (tempString.length > 0) tokenArray.push(tempString);
        return tokenArray;
    }

    function parseFilterObject(filterObject)
    {
        for (var o in filterObject)
        {
            if (filterObject[o] instanceof String)
            {
            
            }
            else if (filterObject[o] instanceof Array)
            {
            }
            else
            {
            }
        }
    }
    
    if (typeof(filterString) == "string") 
        parseFilterString(filterString);
    else parseFilterObject(filterString);
}//end Filter

/**
 * @argument {String} elementId ID of HTML location element to bind
 * @argument {String} templateId ID of HTML template element to base binding
 * @argument {int} itemCount Maximum number of visible items for the control (e.g. # of Template elements cloned)
 * @constructor
 * @class Base control for template based controls
 * @type {TemplateExpansionItem}
 */
function TemplateExpansion(elementId, templateId, itemCount)
{
    var _baseElement = document.getElementById(elementId);
    var _templateElement = document.getElementById(templateId);
    var _itemCount = (itemCount == null) ? 0 : itemCount;
    var _listItems = new Array();
    var _paused = false;
    
  	// Here we insert an empty span element where the template element was to keep
	// track of where we need to insert new items.  Then, we remove the template node    
    var _templateElement = document.getElementById(templateId);
    //var _parentElement = _templateElement.parentNode;
    var _insertPlaceholder = document.createElement("span");
    
    //_parentElement.insertBefore(_insertPlaceholder, _templateElement);
    //_parentElement.removeChild(_templateElement);

    this.clearItems = clearItems;
    this.callback = updateIncomingHeadlineItems;
    this.setPaused = setPaused;
    this.getPaused = getPaused;
    
    /**
     * @type {bool}
     * @return Returns whether the control is or is not paused
     */
    function getPaused()
    {
        return _paused;
    } 
    
    /**
     * Sets the pause state of the control
     * @argument {bool} isPaused true = pause
     */
    function setPaused(isPaused)
    {
        _paused = isPaused;
    }
    
    /**
     * Clears visible templates
     */
    function clearItems()
    {
        while(_listItems.length > 0)
        {
            var listItem = _listItems.shift();
            listItem.removeData(updateIncomingHeadlineItems);
            _baseElement.removeChild(listItem.baseElement);
        }
    }

    function updateIncomingHeadlineItem(dataElement, listElement)
    {
        _baseElement.insertBefore(dataElement, _insertPlaceholder.nextSibling);

        _listItems.push(listElement);

        while(_listItems.length > _itemCount)
        {
            var listItem = _listItems.shift();
            
            listItem.removeData(updateIncomingHeadlineItems);
            _baseElement.removeChild(listItem.baseElement);
        }
    }
        
    function updateIncomingHeadlineItems(dataArray)
    {
		if (_baseElement == null) return true;
		
        if (_paused)
        {
            return;
        }
        
        if (_itemCount == 0) return updateStaticTemplate(dataArray);
        
	    var newElem = null;
	    var newListItem = null;
	    
	    if (dataArray.length == null)
	    {
            newElem = _templateElement.cloneNode(true);
	        newElem.id = "";
            newListItem = new TemplateExpansionItem(newElem);
            newListItem.newData(dataArray);
            updateIncomingHeadlineItem(newElem, newListItem);
	    }
	    else
	    {
	        for (var idx = 0; idx < dataArray.length; idx++)
	        {
                newElem = _templateElement.cloneNode(true);
    	        newElem.id = "";
	            newListItem = new TemplateExpansionItem(newElem);
                newListItem.newData(dataArray[idx]);
                updateIncomingHeadlineItem(newElem, newListItem);
            }
        }
    }
    
    function updateStaticTemplate(dataArray)
    {
        var newListItem;
        
        if (_listItems.length == 0)
        {
            var newElem = _templateElement.cloneNode(true);
	        newElem.id = "";
            _listItems[0] = new TemplateExpansionItem(newElem);
        }
        
	    newListItem = _listItems[0];
	    
	    if (dataArray.length == null)
	    {
            newListItem.newData(dataArray);
	    }
	    else
	    {
	        for (var idx = 0; idx < dataArray.length; idx++)
	        {
                newListItem.newData(dataArray[idx]);
                //window.setTimeout(function() {newListItem.newData(dataArray[idx]);}, idx * 1000);
            }
        }

        _baseElement.appendChild(newListItem.baseElement);
    }
}

/**
 * @argument {HTML Element} element Actual HTML element to bind
 * @constructor Internal - called by {TemplateExpansion}
 * @class Cloned template to be bound and displayed
 * @type {TemplateExpansionItem}
 */
function TemplateExpansionItem(baseElement)
{
    var element = baseElement;
    var data = null;
    
    this.newData = newData;
    this.removeData = removeData;
    this.baseElement = element;
    
    function getData()
    {
        return this.data;
    }

    function newData(dataItem)
    {
        data = dataItem;
        
        if(dataItem != null)
        {
		    updateTemplateItems(element, "mwbind");
		    if (element.getAttribute("mwonupdate") != null)
		    {
		        eval(element.getAttribute("mwonupdate"));
		    }
		}
    }
    
    function removeData(dataCallback)
    {
	    var mwBind = element.getAttribute( "mwunbind");
	    if(mwBind != null)
	    {
		    element.innerHTML = eval(mwBind);			
	    }
	    
	    element.data = null;
	}
    
    function updateTemplateItems(node, attributeName)
    {
	    for(var i = 0; i < node.childNodes.length; ++i)
	    {
		    var child = node.childNodes[i];
    	
		    if(child.nodeType == 1)
		    {	
			    if(child.childNodes.length > 0)
				    updateTemplateItems(child, attributeName);
    				
			    var mwBind = child.getAttribute(attributeName);
			    if(mwBind != null)
			    {
				    child.innerHTML = eval(mwBind);			
			    }
			    
			    //child.data = data;
		    }
	    }	    
    }
}

/**
 * @private
 */
function ScriptSrc_Gateway(endpoint)
{
    ScriptSrc_Gateway._nonce = 1;
    var _endpoint = endpoint + '?';
     
    this.sendRequestResponse = function(src, callbackMethod, callbackState)
    {
	    var nonce = ++ScriptSrc_Gateway._nonce;
	    var cb = "_ls_cb" + nonce;
    	
	    var scriptElement = document.createElement("script");
    //	scriptElement.id = id;
	    scriptElement.type = "text/javascript";
	    scriptElement.src = _endpoint + src + "&cb=" + cb;

	    var appendComplete = false;
	    var callbackComplete = false;
	    var windowTimeout = null;
    	var callTiming = new Date();
    	
	    window[cb] = function(response)
	    {
		    if (windowTimeout != null)
		    {
			    window.clearTimeout(windowTimeout);
			    windowTimeout = null;
		    }

		    callbackComplete = true;
		    callTiming = new Date() - callTiming;
		    
		    if (appendComplete)
		    {
			    document.body.removeChild(scriptElement);
			    window[cb] = null;
		    }		
    		
		    try
		    {
		        if (response == null)
		        {
		            response = new Object();
		            response.Status = "exception";
		            response.Exception = new Object();
		            response.Exception.message = "null response received";
		        }

                response.Duration = callTiming;
                
    		    PipelineProtocol.On_StatusUpdate({ "Status":"success", "Response":_toJSONString( response )});
			    callbackMethod(response, callbackState);
		    }
		    catch(ex)
		    {
			    var callException = new Object();
			    callException.Exception = new Object();
			    callException.Exception.description = "callback error";
			    callException.Exception.message = ex.Message;
			    callbackMethod(callException, callbackState);
		    };
	    };

	    windowTimeout = window.setTimeout(function()
		    {
			    if (window[cb] != null)
			    {
				    var response = new Object();
				    response.Status = "exception";
				    response.Exception = new Object();
				    response.Exception.message = "request timeout";
				    window[cb](response);
			    }
		    }, 35000);

	    document.body.appendChild(scriptElement);
    	
	    appendComplete = true;
	    if (callbackComplete)
	    {
		    document.body.removeChild(scriptElement);
		    window[cb] = null;
	    }
    }
}

/**
 * @private
 */
function XmlHttp_Gateway(endpoint)
{
    XmlHttp_Gateway.nullFunction = function() {} 
    var _endpoint = endpoint + '?';

    this.sendRequestResponse = function(src, callbackMethod, callbackState)
    {
	    var xmlhttp = null;
	    if (window.XMLHttpRequest)
	    {
		    xmlhttp = new XMLHttpRequest();
	    }
	    else if (window.ActiveXObject)
	    {
		    try
		    {
			    xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		    }
		    catch(ex1)
		    {
			    xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
		    };
	    }

	    var windowTimeout = null;

	    xmlhttp.open("GET", _endpoint + src + "&r=" + Math.random(), true);
	    try
	    {
	        xmlhttp.setRequestHeader("Cookie", "none");
	        xmlhttp.setRequestHeader("Cookie", "");
        } catch(ex) {};
    	
	    xmlhttp.onreadystatechange = function() 
	    {
		    if (xmlhttp.readyState==4)
		    {
			    if (windowTimeout != null)
			    {
				    window.clearTimeout(windowTimeout);
				    windowTimeout = null;
			    }

		        xmlhttp.onreadystatechange = XmlHttp_Gateway.nullFunction;
		        var responseText = xmlhttp.responseText;
		        xmlhttp = null;

			    try
			    {
				    var response;
				    try
				    {
            		    PipelineProtocol.On_StatusUpdate({ "Status":"success", "Response":responseText});
				        if (responseText == "")
				        {
				            response = null;
				        }
					    else response = eval('(' + responseText + ')');
				    }
				    catch(parseEx)
				    {
					    response = new Object();
					    response.Exception = new Object();
					    response.Exception.message = "parse error: " + parseEx.message;
					    try
					    {
					        response.Exception.detail = responseText;
					    }catch(ex5){};
				    };
				    callbackMethod(response, callbackState);
			    }
			    catch(ex)
			    {
				    var callException = new Object();
				    callException.Exception = new Object();
				    callException.Exception.message = "callback error";
				    callbackMethod(callException, callbackState);
			    }
		    }
        };

//	    windowTimeout = window.setTimeout(function()
//		    {
//			    if (windowTimeout != null)
//			    {
//				    windowTimeout = null;
//				    try
//				    {
//					    xmlhttp.onreadystatechange = XmlHttp_Gateway.nullFunction;
//					    xmlhttp.abort();
//				    }
//				    catch(ex5)
//				    {
//				    };
//				    xmlhttp = null;

//				    var timeoutResponse = new Object();
//				    timeoutResponse.Exception = new Object();
//				    timeoutResponse.Exception.message = "request timeout";
//				    try
//				    {
//					    callbackMethod(timeoutResponse, callbackState);
//				    }
//				    catch(ex3)
//				    {
//				    };
//			    }
//		    }, 25000);


        xmlhttp.send(null);
    }
}

// from JSON.js

(function () {
    var m = {
            '\b': '\\b',
            '\t': '\\t',
            '\n': '\\n',
            '\f': '\\f',
            '\r': '\\r',
            '"' : '\\"',
            '\\': '\\\\'
        },
        s = {
            array: function (x) {
                var a = ['['], b, f, i, l = x.length, v;
                for (var i = 0; i < l; i += 1) {
                    v = x[i];
                    f = s[typeof v];
                    if (f) {
                        v = f(v);
                        if (typeof v == 'string') {
                            if (b) {
                                a[a.length] = ',';
                            }
                            a[a.length] = v;
                            b = true;
                        }
                    }
                }
                a[a.length] = ']';
                return a.join('');
            },
            'boolean': function (x) {
                return String(x);
            },
            'null': function (x) {
                return "null";
            },
            number: function (x) {
                return isFinite(x) ? String(x) : 'null';
            },
            object: function (x) {
                if (x) {
                    if (x instanceof Array) {
                        return s.array(x);
                    }
                    var a = ['{'], b, f, i, v;
                    for (var i in x) {
                        v = x[i];
                        f = s[typeof v];
                        if (f) {
                            v = f(v);
                            if (typeof v == 'string') {
                                if (b) {
                                    a[a.length] = ',';
                                }
                                a.push(s.string(i), ':', v);
                                b = true;
                            }
                        }
                    }
                    a[a.length] = '}';
                    return a.join('');
                }
                return 'null';
            },
            string: function (x) {
                if (/["\\\x00-\x1f]/.test(x)) {
                    x = x.replace(/([\x00-\x1f\\"])/g, function(a, b) {
                        var c = m[b];
                        if (c) {
                            return c;
                        }
                        c = b.charCodeAt();
                        return '\\u00' +
                            Math.floor(c / 16).toString(16) +
                            (c % 16).toString(16);
                    });
                }
                return '"' + x + '"';
            }
        };

    window._toJSONString = function (obj) {
        return s.object(obj);
    };


/*

    Object.prototype.toJSONString = function () {
        return s.object(this);
    };
    
    Array.prototype.toJSONString = function () {
        return s.array(this);
    };
    
*/

    Array.prototype.contains = function(testValue) {
        for (var t = 0; t < this.length; t++)
        {
            if (this[t] == testValue) return true;
        }
        
        return false;
    };
})();

String.prototype.parseJSON = function () {
    try {
        return !(/[^,:{}\[\]0-9.\-+Eaeflnr-u \n\r\t]/.test(
                this.replace(/"(\\.|[^"\\])*"/g, ''))) &&
            eval('(' + this + ')');
    } catch (e) {
        return false;
    }
};



// a global month names array
var gsMonthNames = new Array(
'January',
'February',
'March',
'April',
'May',
'June',
'July',
'August',
'September',
'October',
'November',
'December'
);

// a global day names array
var gsDayNames = new Array(
'Sunday',
'Monday',
'Tuesday',
'Wednesday',
'Thursday',
'Friday',
'Saturday'
);

// the date format prototype
function DateFormat(f)
{
    if (!this.valueOf())
        return '&nbsp;';

    //var d = this;
  
    function zf(val, places)
    {
        var result = "00" + val.toString();
        return result.substr(result.length - places, places);
    }
    
    var result = new String(f);
    result = result.replace("yyyy", this.getFullYear());
    result = result.replace("mmmm", gsMonthNames[this.getMonth()]);
    result = result.replace("mmm", gsMonthNames[this.getMonth()].substr(0, 3));
    result = result.replace("mm", zf((this.getMonth() + 1),2));
    result = result.replace("dddd", gsDayNames[this.getDay()]);
    result = result.replace("ddd", gsDayNames[this.getDay()].substr(0, 3));
    result = result.replace("dd", zf(this.getDate(),2));
    result = result.replace("hh", zf(((h = this.getHours() % 12) ? h : 12),2));
    result = result.replace("h", ((h = this.getHours() % 12) ? h : 12));
    result = result.replace("nn", zf(this.getMinutes(),2));
    result = result.replace("sss", zf(this.getMilliseconds(),3));
    result = result.replace("ss", zf(this.getSeconds(),2));
    result = result.replace("a/p", this.getHours() < 12 ? 'a' : 'p');
    result = result.replace("A/P", this.getHours() < 12 ? 'A' : 'P');
    
    return result;

    /*
    return f.replace(/(yyyy|mmmm|mmm|mm|dddd|ddd|dd|hh|nn|ss|a\/p)/gi,
        function($1)
        {
            switch ($1.toLowerCase())
            {
            case 'yyyy': return d.getFullYear();
            case 'mmmm': return gsMonthNames[d.getMonth()];
            case 'mmm':  return gsMonthNames[d.getMonth()].substr(0, 3);
            case 'mm':   return (d.getMonth() + 1).zf(2);
            case 'dddd': return gsDayNames[d.getDay()];
            case 'ddd':  return gsDayNames[d.getDay()].substr(0, 3);
            case 'dd':   return d.getDate().zf(2);
            case 'hh':   return ((h = d.getHours() % 12) ? h : 12).zf(2);
            case 'nn':   return d.getMinutes().zf(2);
            case 'ss':   return d.getSeconds().zf(2);
            case 'a/p':  return d.getHours() < 12 ? 'a' : 'p';
            }
        }
    );
    */
};

/**
 * Function that returns a number rounded to a specified place.
 * 
 * @param {Object} number Base number to round
 * @param {Object} radian Position to round to (+2 = two places after the decimal, -2 = hundreds place)
 */
    Math.roundTo = function (number, radian) 
	{
		var tempNumber = number;
		
		if (radian > 0)
		{
			tempNumber = number * Math.pow(10, radian);
		}
		else
		{
			tempNumber = number / Math.pow(10, -radian);
		}
		
		tempNumber = Math.pow(10, -radian) * Math.round(tempNumber);
		
        return tempNumber;
    };
