require 'test_helper'

class OrderTest < ActiveSupport::TestCase
  def setup
    @orderNumber = 200
    @company = Company.find(:first)
    @acct = Account.new
    @acct.broker = "VSE"
    @acct.user = "brian"
    @acct.password = "bob"
    @acct.name = "bruce"
    @acct.subaccount = "401K"
    @acct.save
  end

  def generate_order()
    ord = Order.new
    ord.number = @orderNumber
    @orderNumber = @orderNumber + 1
    ord.account = @acct
    ord.date = Date.new
    ord.xaction = Order.TypeBuy
    ord.company = @company
    ord.quantity = 777
    return ord
  end

  def test_state
    assert_equal nil, Order.getPendingOrder(@acct)
    assert_equal nil, Order.getLastOrder(@acct)

    ordcomplete = generate_order()
    ordcomplete.ostate = Order.StateComplete
    ordcomplete.save
    assert_equal nil, Order.getPendingOrder(@acct)
    assert_equal ordcomplete.number, Order.getLastOrder(@acct).number.to_i

    ord = generate_order()
    ord.ostate = Order.StateCancelled
    ord.save
    assert_equal nil, Order.getPendingOrder(@acct)
    assert_equal ordcomplete.number, Order.getLastOrder(@acct).number.to_i
    
    ord = generate_order()
    ord.ostate = Order.StatePending
    ord.save
    assert_equal ord.number, Order.getPendingOrder(@acct).number.to_i
    assert_equal ordcomplete.number, Order.getLastOrder(@acct).number.to_i
    ord.ostate = Order.StateComplete
    ord.save
    assert_equal nil, Order.getPendingOrder(@acct)
    assert_equal ordcomplete.number, Order.getLastOrder(@acct).number.to_i

    ord = generate_order()
    ord.ostate = Order.StateCancelPending
    ord.save
    assert_equal ord.number, Order.getPendingOrder(@acct).number.to_i
    assert_equal ordcomplete.number, Order.getLastOrder(@acct).number.to_i
    ord.ostate = Order.StateCancelled
    ord.save
    assert_equal nil, Order.getPendingOrder(@acct)
    assert_equal ordcomplete.number, Order.getLastOrder(@acct).number.to_i
  end
end
