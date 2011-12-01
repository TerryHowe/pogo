require 'test_helper'

class OneTest < ActiveSupport::TestCase
  def test_truth
    rec = One.new
    rec.value = 3.22
    assert_equal true, rec.save
    assert_equal true, One.updater('4.22')
  end
end
