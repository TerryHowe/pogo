require 'rubygems'
require 'rufus/scheduler'
 
class Scheduler
  def initialize(every)
    @every = every
    @scheduler = Rufus::Scheduler.start_new
  end

  def implementation
    raise 'Not implemented error'
  end

  def start()
    @scheduler.every @every do
      implementation()
    end
    @scheduler.join
  end

  def stop()
    @scheduler.stop
  end
end
