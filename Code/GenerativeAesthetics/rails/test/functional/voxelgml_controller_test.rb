require 'test_helper'

class VoxelgmlControllerTest < ActionController::TestCase
  test "should get view" do
    get :view
    assert_response :success
  end

end
