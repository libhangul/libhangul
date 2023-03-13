require 'test/unit'
require 'hangul'

class TestHangulInputContext < Test::Unit::TestCase
  def setup
    @hic = Hangul::InputContext.new("2")
  end
  def test_2bul_string
    input = "fnql gksrmf fkdlqmfjfl xptmxm."
    expected = "루비 한글 라이브러리 테스트."
    buffer = ''
    input.each_byte do |c|
      ret = @hic.process(c)
      buffer << @hic.commit_string.to_s
      buffer << c.chr unless ret
    end
    @hic.flush
    buffer << @hic.commit_string.to_s
    assert_equal expected, buffer.to_s.force_encoding("UTF-8")
  end
end

# vim: set sts=2 sw=2 et:
