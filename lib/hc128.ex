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

  ## NIF API
  def alloc_context_nif(id)
  def alloc_context_nif(_id), do: :erlang.nif_error(:not_loaded)

  def release_context_nif(context)
  def release_context_nif(_context), do: :erlang.nif_error(:not_loaded)

  def list_context_nif()
  def list_context_nif(), do: :erlang.nif_error(:not_loaded)

  def set_key_iv_nif(context, key, iv)
  def set_key_iv_nif(_context, _key, _iv), do: :erlang.nif_error(:not_loaded)

  def decode_stream_nif(context, input)
  def decode_stream_nif(_context, _input), do: :erlang.nif_error(:not_loaded)

end
