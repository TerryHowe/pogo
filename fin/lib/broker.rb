class Broker
  DurationDay = 0
  DurationGTC = 1

  PricingMarket = 0
  PricingLimit = 1
  PricingStop = 2

  def self.OrderStatePending
    return 0;
  end
  def self.OrderStateCancelPending
    return 1;
  end
  def self.OrderStateCancelled
    return 2;
  end
  def self.OrderStateComplete
    return 3;
  end

  ContentType = 'Content-Type'
  ContentTypeForm = 'application/x-www-form-urlencoded'

  attr_accessor :postdata, :path, :headers, :data

  def initialize(urlStr, user, password, name, subaccount)
    @url = URI.parse(urlStr)
    @user = user
    @password = password
    @name = name
    @subaccount = subaccount
    @headers = {}
    @postdata = {}
  end

  def start()
    @http = Net::HTTP.start(@url.host, @url.port)
  end

  def dumpResponse(fileName)
    fileName = "#{RAILS_ROOT}/log/" + fileName + ".html"
    begin
      File.unlink(fileName)
    rescue
    end
    f = File.open(fileName, 'w')
    f.write(@data.to_s)
    f.close
  end

  def packdata()
    str = ''
    @postdata.each { |key, value|
      if str != ''
        str += '&'
      end
      str += CGI::escape(key) + '=' + CGI::escape(value)
    }
    return str
  end

  def extractValue(reggie)
    re = Regexp.new(reggie, Regexp::IGNORECASE)
    md = re.match(@response.body)
    unless md.nil?
      return md[1]
    end
    return ''
    rescue => e
      puts @logname + 'ERROR: Cannot find ' + reggie
      return 'ERROR: Cannot find ' + reggie
  end

  def save_response()
    unless @response.response['Location'].nil?
      @path=@response.response['Location']
      puts @logname + '==Redirect: ' + @path
    end
    unless @response.response['set-cookie'].nil?
      puts @logname + '**saving cookie'
      @headers['Cookie'] = @response.response['set-cookie']
    end
  end

  def getit(name)
    @logname = name + ': '
    @response, @data = @http.get(@path, @headers)
    save_context()
    dumpResponse(name)
    @response.body.gsub!("\n","\s")

    case @response
    when Net::HTTPSuccess, Net::HTTPRedirection
      puts @logname + @response.to_s
      return true
    end
    puts @logname + 'ERROR Getting ' + @path
    return false
  end

  def postit(name)
    @logname = name + ': '
    @headers[ContentType] = ContentTypeForm
    @response, @data = @http.request_post(@path, packdata(), @headers)
    save_context()
    dumpResponse(name)
    @response.body.gsub!("\r\n","\s")

    case @response
    when Net::HTTPSuccess, Net::HTTPRedirection
      puts @logname + ': ' + @response.to_s
      return true
    end
    puts 'ERROR Posting to ' + @path
    return false
  end

  def save_context()
    raise 'Not implemented'
  end

  def open()
    raise 'Not implemented'
  end

  def login()
    raise 'Not implemented'
  end

  def buy(symbol, quantity, price, duration = DurationGTC, order = OrderMarket)
    raise 'Not implemented'
  end

  def sell(symbol, quantity, price, duration = DurationGTC)
    raise 'Not implemented'
  end

  def check(order)
    raise 'Not implemented'
  end

  def cancel(order)
    raise 'Not implemented'
  end

  def getPrice(symbol)
    raise 'Not implemented'
  end
end
