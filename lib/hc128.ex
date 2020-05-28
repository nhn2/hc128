defmodule HC128 do
  @compile {:autoload, false}
  @on_load {:init, 0}

  def init() do
    case load_nif() do
      :ok ->
        :ok
      _ ->
        raise """
        An error occurred when loading HC128. Make sure you have a C compiler and Erlang 20 installed.
        """
    end
  end

  defp load_nif() do
    path = :filename.join(:code.priv_dir(:hc128), 'hc128_nif')
    :erlang.load_nif(path, 0)
  end

  ## Export API
  def alloc_context(), do: alloc_context_nif(self(), System.system_time(:millisecond))
  def print_context(context), do: print_context_nif(context)
  def count_context(), do: count_context_nif()
  def decode_stream(context, input), do: decode_stream_nif(context, input)
  def set_key_iv(context, key, iv) when byte_size(key) == 16 and byte_size(iv) == 16 do
    set_key_iv_nif(context, key, iv)
  end
  def set_key_iv(_context, _key, _iv) do
    {:error, :invalid_key_iv}
  end
  ## NIF API
  defp alloc_context_nif(pid, created_at)
  defp alloc_context_nif(_pid, _created_at), do: :erlang.nif_error(:not_loaded)

  defp print_context_nif(context)
  defp print_context_nif(_context), do: :erlang.nif_error(:not_loaded)

  defp count_context_nif()
  defp count_context_nif(), do: :erlang.nif_error(:not_loaded)

  defp set_key_iv_nif(context, key, iv)
  defp set_key_iv_nif(_context, _key, _iv), do: :erlang.nif_error(:not_loaded)

  defp decode_stream_nif(context, input)
  defp decode_stream_nif(_context, _input), do: :erlang.nif_error(:not_loaded)

end
