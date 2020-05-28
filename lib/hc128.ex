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
end
