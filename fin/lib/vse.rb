class Vse < Broker
public
  attr_accessor :orderDate, :orderTransaction, :orderSymbol
  attr_accessor :orderQuantity, :orderStatus

  def initialize(user, password, name, subaccount)
    super(AppConfig.vse, user, password, name, subaccount)
    start()
  end

  def save_context()
    save_response()
    vs = extractValue('<input [^>]*name="__VIEWSTATE" [^>]*value="([^"]*)"')
    if vs.nil?
      puts @logname + 'ERROR: View state is nil'
    else
      if (vs != '')
        @viewstate = vs
        puts @logname + '**viewstate saved'
      end
    end
  end

  def open()
    @path = AppConfig.vse_login
    return getit('open')
  end

  def login()
    @path = AppConfig.vse_login
    @headers['Referer'] = @url.to_s + @path
    @postdata = { 'LoginCenter1$_Email' => @user,
                  'LoginCenter1$_Password' => @password,
                  'LoginCenter1$_LoginButton.x' => '44',
                  'LoginCenter1$_LoginButton.y' => '12',
                  '__VIEWSTATE' => @viewstate
                }
    if (postit('login') == false)
      val= extractValue('<div id="LoginCenter1_ProblemSummary1" [^>]*(>[^<]*)')
      if (val != '')
        puts @logname + 'Loging error: ' + val
      end
      return false;
    end

    if (getit('loginConfirm') == false)
      puts @logname + 'Error getting home page'
      return false
    end

    displayName = extractValue('<span id="LoginCorner1__DisplayName">([^<]*)')
    if (displayName != @name)
      puts @logname + 'Error display name <' + displayName + '> does not equal <' + @name + '> in database'
      return false
    end

    return true
  end

  def trade(order, symbol, quantity, price, duration, pricing)
    @path = AppConfig.vse_trade
    getit('traderead')

    @headers['Referer'] = @url.to_s + @path
    @postdata = { '__EVENTTARGET' => '',
                  '__EVENTARGUMENT' => '',
                  '__VIEWSTATE' => @viewstate
                }
    @postdata['_TradeControl$EnterSymbol$_GameDropDownList'] = @subaccount
    @postdata['_TradeControl$EnterSymbol$_SymbolTextBox'] = symbol
    @postdata['_TradeControl$EnterSymbol$LinkButton1'] = '2'
    if (postit('tradesymbol') == false)
      getit('tradefail')
      return nil
    end
    getit('tradesymbolok')
    @postdata.delete('_TradeControl$EnterSymbol$LinkButton1')

    @postdata['__VIEWSTATE'] = @viewstate
    case order
    when Order.TypeBuy
      @postdata['_TradeControl$ViewQuote$_Type'] = 'Buy'
    when Order.TypeSell
      @postdata['_TradeControl$ViewQuote$_Type'] = 'Sell'
    when Order.TypeSellShort
      @postdata['_TradeControl$ViewQuote$_Type'] = 'Sell Short'
    when Order.TypeBuyCover
      @postdata['_TradeControl$ViewQuote$_Type'] = 'Buy to Cover'
    else
      @postdata['_TradeControl$ViewQuote$_Type'] = 'Sell'
    end
    @postdata['_TradeControl$ViewQuote$_NumShares'] = quantity.to_s
    case duration
    when DurationDay
      @postdata['_TradeControl$ViewQuote$_Term'] = 'Day Order'
    when DurationGTC
      @postdata['_TradeControl$ViewQuote$_Term'] = 'Good till Cancelled'
    else
      @postdata['_TradeControl$ViewQuote$_Term'] = 'Day Order'
    end
    case pricing
    when PricingMarket
      @postdata['_TradeControl_ViewQuote__PriceType'] = 'Market'
    when PricingLimit
      @postdata['_TradeControl_ViewQuote__PriceType'] = 'Limit'
      @postdata['_TradeControl$ViewQuote$_StopOrLimitPrice'] = price.to_s
    when PricingStop
      @postdata['_TradeControl_ViewQuote__PriceType'] = 'Stop'
      @postdata['_TradeControl$ViewQuote$_StopOrLimitPrice'] = price.to_s
    end
    @postdata['_TradeControl$ViewQuote$_Preview.x'] = '2'
    @postdata['_TradeControl$ViewQuote$_Preview.y'] = '3'
    if postit('tradepreview') == false
      getit('tradefail')
      return nil
    end
    @postdata.delete('_TradeControl$ViewQuote$_Preview.x')
    @postdata.delete('_TradeControl$ViewQuote$_Preview.y')
    getit('tradepreviewok')

    @postdata['__VIEWSTATE'] = @viewstate
    @postdata['_TradeControl$PreviewOrder$_SubmitOrder.x'] = '3'
    @postdata['_TradeControl$PreviewOrder$_SubmitOrder.y'] = '2'
    if postit('tradeconfirm') == false
      getit('tradefail')
      return nil
    end
    getit('tradeconfirmok')
    @postdata.delete('_TradeControl$PreviewOrder$_SubmitOrder.x')
    @postdata.delete('_TradeControl$PreviewOrder$_SubmitOrder.y')
    number = extractValue('The order is <strong>#([^<]*)')
    puts @logname + number

    return number
  end

  def buy(symbol, quantity, price, duration=DurationGTC, pricing=PricingMarket)
    return trade(Order.TypeBuy, symbol, quantity, price, duration, pricing)
  end

  def sell(symbol, quantity, price, duration=DurationGTC, pricing=PricingMarket)
    return trade(Order.TypeSell, symbol, quantity, price, duration, pricing)
  end

  def read()
    @path = AppConfig.vse_read
    if (getit('read') == false)
      getit('readfail')
      return false
    end
    eventTarget =   extractValue('<a [^>]*href="javascript:__doPostBack\(\'([^\']*)\'[^>]*>' + @subaccount + '</a>')
    eventArgument = extractValue('<a [^>]*href="javascript:__doPostBack\(\'[^\']*\',\'([^\']*)\'[^>]*>' + @subaccount + '</a>')
puts @logname + 'TARGET: ' + eventTarget
    eventTarget.gsub!(/_MyGame$/, '_MyPortfolio')
    @postdata = { '__EVENTTARGET' => eventTarget,
                  '__EVENTARGUMENT' => eventArgument,
                  '__VIEWSTATE' => @viewstate
                }
puts @logname + '@path=' + @path
    if (postit('readpost') == false)
      getit('readpostfail')
      return false
    end

    if (getit('readoverview') == false)
      getit('readoverviewfail')
      return false
    end
  end

  def extractPrepare
    @data.gsub!(/[\t\n\r\f]/," ")
    @data.gsub!(/ +/," ")
    @data.gsub!(/<p>/,"")
    @data.gsub!(/<p [^>]*>/,"")
    @data.gsub!(/<\/p>/,"")
    @data.gsub!(/<\/a>/,"")
    @data.gsub!(/<\/td>/,"")
    @data.gsub!(/<tr[^>]*>/,"<tr>")
    @data.gsub!(/<td[^>]*>/,"<td>")
    @data.gsub!(/>[ \t\n\r\f]+/,">")
    @data.gsub!(/[ \t\n\r\f]+</,"<")
  end

  def extractMatch(reggie)
    re = Regexp.new(reggie, Regexp::IGNORECASE)
    if ((md = re.match(@data)) == nil)
      return nil
    end
    return md
  rescue => e
    puts @logname + 'ERROR: Cannot find ' + e.to_s
    return nil
  end

  def extractOrder(orderNumber)
    reggie='<tr><td>' + orderNumber + '<td>([^<]*)<td>([^<]*)<td>([^<]*)<td>([^<]*)<td>([^<]*)<td>([^<]*)<td>([^<]*)</tr>'
    @data.gsub!(/<a [^>]*>/,"")
    extractPrepare()
    if ((md = extractMatch(reggie)) == nil)
      return nil
    end
    if (md.size != 8)
      return nil
    end
    @orderDate = md[1]
    @orderDuration = md[2]
    @orderTransaction = md[3]
    @orderSymbol = md[4]
    @orderQuantity = md[5]
    @orderType = md[6]
    @orderStatus = md[7]
    puts @logname + orderNumber + ' ' + @orderDate + ' ' + @orderTransaction + ' ' + @orderSymbol + ' ' + @orderQuantity + ' ' + @orderStatus
    return @orderStatus
  end

  def extractCancel(orderNumber)
    extractPrepare()
    puts @logname + ' done prepare'
    reggie='<tr><td>' + orderNumber + '<td>[^<]*<td>[^<]*<td>[^<]*<td><a[^<]*<td>[^<]*<td>[^<]*<td><a[^>]*href="[^"]*\&quot;([^\&]*)&quot'
    reggie='<tr><td>' + orderNumber + '<td>[^<]*<td>[^<]*<td>[^<]*<td><a[^<]*<td>[^<]*<td>[^<]*<td>[^<]*<a[^>]*href="[^\&]*\&quot;([^\&]*)&quot'

# <a onclick="return confirm('Are you sure you want to cancel this order?');" id="_OrdersDataGrid_ctl02_CancelLink" href="javascript:WebForm_DoPostBackWithOptions(new WebForm_PostBackOptions(&quot;_OrdersDataGrid$ctl02$CancelLink&quot;, &quot;&quot;, true, &quot;&quot;, &quot;&quot;, false, true))">cancel</a>

    puts @logname + ' done reggie'
    re = Regexp.new(reggie, Regexp::IGNORECASE)
    if ((md = extractMatch(reggie)) == nil)
    puts @logname + ' nil'
      return nil
    end
    puts @logname + md.size.to_s
    if (md.size != 2)
      return nil
    end
    puts @logname + md[1]
    return md[1]
  end

  def check(orderNumber)
    read()
    orderState = extractOrder(orderNumber)
    case orderState
    when 'Pending cancel'
      return Broker.OrderStatePending
    when 'CancelPending'
      return Broker.OrderStateCancelPending
    when 'Cancelled'
      return Broker.OrderStateCancelled
    when 'Complete'
      return Broker.OrderStateComplete
    end
  end

  def cancel(orderNumber)
    read()
    puts @logname + ' calling extract cancle'
    eventTarget = extractCancel(orderNumber)
    @postdata = { '__EVENTTARGET' => eventTarget,
                  '__EVENTARGUMENT' => '',
                  '__VIEWSTATE' => @viewstate
                }
puts @logname + '@path=' + @path
    if (postit('readpost') == false)
      getit('readpostfail')
      return false
    end
    return true
  end

  def getPrice()
    web = Web.new()
    hsh = web.readDetail(@company.ticker, @company.googid)
    unless hsh.nil?
      puts @company.ticker + ": " + hsh[:price]
      @company.updater(hsh[:price])
    end
    return hsh
  end
end
