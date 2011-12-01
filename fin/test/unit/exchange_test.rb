require 'test_helper'

class ExchangeTest < ActiveSupport::TestCase
  def test_truthiness
    timezone = 'Eastern Time (US & Canada)'
    zo = ActiveSupport::TimeZone.new(timezone);
    now = zo.now()
    x = Exchange.find(:first)
    mini = (((now.hour * 60) + now.min) * 60) + now.sec
    openMini = ((9 * 60) + 30) * 60
    closeMini = (16 * 60) * 60
    if ((mini >= openMini) && (mini <= closeMini))
      assert_equal true, x.isOpen()
    else
      assert_equal false, x.isOpen()
    end
  end
end
