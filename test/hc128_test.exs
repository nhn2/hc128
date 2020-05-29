defmodule HC128Test do
  use ExUnit.Case
  doctest HC128
#
#  test "alloc" do
#    pid = self()
#    created = System.system_time(:millisecond)
#    {:ok, c} = HC128.alloc_context_nif(pid, created)
#    assert 1 == HC128.count_context()
#    {pid2, created2} = HC128.print_context(c)
#    assert pid2 == pid
#    assert created2 == created
#  end
#
#  test "alloc_gc" do
#    spawn(fn ->
#      pid = self()
#      created = System.system_time(:millisecond)
#      {:ok, c} = HC128.alloc_context_nif(pid, created)
#      assert 1 == HC128.count_context()
#      {pid2, created2} = HC128.print_context(c)
#      assert pid2 == pid
#      assert created2 == created
#      IO.puts "count = #{HC128.count_context()}"
#    end)
#    Process.sleep(1000)
#    :erlang.garbage_collect()
#    assert 0 == HC128.count_context()
#  end

  test "decode_stream" do
    {:ok, c} = HC128.alloc_context()
    :ok = HC128.set_key_iv(c,
      <<66, 91, 41, 253, 183, 83, 197, 5, 131, 119, 232, 10, 80, 23, 128, 117>>,
      <<222, 173, 69, 193, 42, 200, 147, 206, 170, 0, 191, 182, 123, 64, 25, 167>>)
    {:ok, o1} = HC128.decode_stream c, "AAAAAAAAAAAAAAAA"
    {:ok, o2} = HC128.decode_stream c, "BBBBBBBBBBBBBBBB"
    assert o1 == <<137, 49, 31, 169, 224, 210, 76, 238, 213, 7, 167, 140, 48, 249, 240, 22>>
    IO.inspect o2
  end
end
