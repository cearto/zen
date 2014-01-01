require 'test_helper'

class MeshControllerTest < ActionController::TestCase
  test "should get segment" do
    get :segment
    assert_response :success
  end

end
