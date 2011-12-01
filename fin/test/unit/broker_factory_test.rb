require 'test_helper'

class BrokerFactoryTest < ActiveSupport::TestCase
  fixtures :companies, :ones, :twos, :threes, :accounts

  def get_password(name)
    config = YAML.load_file("#{RAILS_ROOT}/../../p.yml")
    return config[name]
  rescue
    log = Logger.new(STDOUT)
    log.warn("Error opening password file")
  end

  def test_stub
    record = Account.find(:first, :conditions => "broker = '" + Account.STUB + "'")
    brok = BrokerFactory.creator(record)
    assert brok.open()
    assert brok.login()
    assert brok.buy('Q', 2638, 3.79)
    assert brok.sell('Q', 2638, 3.79)
    assert_equal 'Complete', brok.check(101)
    assert brok.cancel(101)
  end

  def test_vse
    record = Account.find(:first, :conditions => "broker = '" + Account.VSE + "'")
    record.password = get_password('vse')
    # brok = BrokerFactory.new(record)
    # assert brok.open()
    # assert brok.login()
    # result = brok.buy('Q', 1, 3.79)
    # result = brok.sell('T', 400, 26.00)
    # assert_not_nil result
    # result = 40715638
    # assert_equal Broker.OrderStatePending, brok.check(result.to_s)
    # assert brok.cancel(result.to_s)
    # assert_equal Broker.OrderStateComplete, brok.check(result.to_s)
  end
end
