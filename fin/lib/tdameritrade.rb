require 'net/http'
require 'uri'

class Tdameritrade
public
  attr_reader :url, :reader
private
  class BogusRequest
    attr_accessor :body
    attr_accessor :url
  end

  def getTestPage(tick)
    @res = BogusRequest.new
    @res.body = nil
    file = File.open("#{RAILS_ROOT}/test/data/" + tick + ".html", 'r')
    unless file.nil?
      blob = ''
      while (line = file.gets)
        blob = blob + line
      end
      @res.body = blob
    end
    return true
  rescue
    return false
  end

  def getIt(itUrl)
    if @http.nil?
      @http = Net::HTTP.start(@url.host, @url.port)
    end

    @res = @http.get(AppConfig.tdameritrade_login)
    case @res
    when Net::HTTPSuccess, Net::HTTPRedirection
      return true
    end
    @res.error!
    puts 'Error loading page at #{@url.host}: ' + @res.message
    @http = nil
    return false
    rescue => e
      puts 'Exception loading page: ' + e.message
      @http = nil
      return false
  end


  def extractValue(name)
    reggie = '<span [^>]*id="ref_'+@googid+'_'+name+'"[^>]*>([^<]*)</span>'
    re = Regexp.new(reggie, Regexp::IGNORECASE)
    md = re.match(@res.body)
    unless md.nil?
      return md[1]
    end
    return ''
    rescue => e
      puts 'ERORR: Cannot find ' + name
      return ''
  end

public
  def initialize(user, password, name, subaccount)
    @url = URI.parse(AppConfig.tdameritrade)
    @http = Net::HTTP.start(@url.host, @url.port)
    @user = user
    @password = password
    @subaccount = subaccount
    @name = name
  end

  def readDetail(symbol, googid)
    if RAILS_ENV == "test"
      getTestPage(symbol)
      @res.url = '/finance?q=' + symbol
    elsif ! getIt('/finance?q=' + symbol)
      return nil
    end
    @googid = googid
    return { :symbol => symbol, :price => extractValue('l'), :percent => extractValue('cp'), :afterPrice => extractValue('el'), :afterPercent => extractValue('ecp')}
  end
end
