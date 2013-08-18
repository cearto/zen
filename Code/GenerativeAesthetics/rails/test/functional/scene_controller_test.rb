require 'test_helper'

class SceneControllerTest < ActionController::TestCase
  test "should get creator" do
    get :creator
    assert_response :success
  end

end
