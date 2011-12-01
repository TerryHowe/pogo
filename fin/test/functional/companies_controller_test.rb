require 'test_helper'

class CompaniesControllerTest < ActionController::TestCase
  def test_should_get_index
    get :index
    assert_response :success
    assert_not_nil assigns(:companies)
  end

  def test_should_get_new
    get :new
    assert_response :success
  end

  def test_should_create_company
    assert_difference('Company.count') do
      post :create, :company => { :ticker => 'TST', :account_id => 'VSE',
        :exchange_id => 'TST', :googid => '9999', :table => 'Four',
        :quantity => '2000', :tactic => 'hilo' }
    end

    assert_redirected_to company_path(assigns(:company))
  end

  def test_should_show_company
    get :show, :id => companies(:att).id
    assert_response :success
  end

  def test_should_get_edit
    get :edit, :id => companies(:att).id
    assert_response :success
  end

  def test_should_update_company
    put :update, :id => companies(:att).id, :company => { }
    assert_redirected_to company_path(assigns(:company))
  end

  def test_should_destroy_company
    assert_difference('Company.count', -1) do
      delete :destroy, :id => companies(:att).id
    end

    assert_redirected_to companies_path
  end
end
