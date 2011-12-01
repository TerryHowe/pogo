require 'scheduler'
require 'web'
require 'strategy'
require 'tactic_factory'

class Strategy <Scheduler
  def initialize()
    super(AppConfig.tick)
    @hsh = {}
  end

  def open
    @thread = Thread.new do
      start()
    end
  end

  def implementation
    puts '----------------------------'
    favorites = Company.find(:all)
    favorites.each { |record|
      if (@hsh[record.ticker] == nil)
        @hsh[record.ticker] = TacticFactory.creator(record)
      end
      if (@hsh[record.ticker].process() == false)
        @hsh[record.ticker] = nil
      end
    }
    puts '----------------------------'
  end
end
