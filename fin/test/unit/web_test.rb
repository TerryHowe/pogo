require 'test_helper'

class WebTest < ActiveSupport::TestCase
  def test_truthiness
    web = Web.new()
    assert_equal 'http://www.google.com/', web.url.to_s
    detail = web.readDetail('T', '33312')
    assert_not_nil detail
    assert_equal 'T', detail.symbol
    assert_equal '28.25', detail.price
    assert_equal '(-3.98%)', detail.percent
    assert_equal '', detail.afterPrice
    assert_equal '', detail.afterPercent

    detail = web.readDetail('Bogus', '33312')
    assert_not_nil detail
    assert_equal 'Bogus', detail.symbol
    assert_equal '', detail.price
    assert_equal '', detail.percent
    assert_equal '', detail.afterPrice
    assert_equal '', detail.afterPercent
  end
end
